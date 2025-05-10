#include <algorithm>
#include "puzzle_solver.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int steps = 0;

PuzzleSolver::PuzzleSolver(int size) : gridSize(size) {
    // Ініціалізація сітки
    grid.resize(gridSize, vector<int>(gridSize, 0));
}

bool PuzzleSolver::canBeShaded(int r, int c) {
    if (grid[r][c] == 1) // Якщо клітина вже є частиною петлі
        return false;
        
    if (shadedCells.find(make_pair(r, c)) != shadedCells.end()) {
        return true; // Клітина вже зафарбована
    }
    
    // Перевіряємо всі сусідні клітини по горизонталі та вертикалі
    const int dr[4] = {0, 1, 0, -1};
    const int dc[4] = {1, 0, -1, 0};
    
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (nr >= 0 && nr < gridSize && nc >= 0 && nc < gridSize) {
            if (shadedCells.find(make_pair(nr, nc)) != shadedCells.end()) {
                return false; // Сусідня клітина зафарбована, не можна зафарбовувати цю
            }
        }
    }
    return true;
}

bool PuzzleSolver::hasUnvisitedNeighbor(int r, int c) {
    const int dr[4] = {0, 1, 0, -1};
    const int dc[4] = {1, 0, -1, 0};
    
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (nr >= 0 && nr < gridSize && nc >= 0 && nc < gridSize && 
            grid[nr][nc] != 2 && visited.find(make_pair(nr, nc)) == visited.end()) {
            return true;
        }
    }
    return false;
}

bool PuzzleSolver::canCloseLoop(int r, int c, int startR, int startC) {
    const int dr[4] = {0, 1, 0, -1};
    const int dc[4] = {1, 0, -1, 0};
    
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (nr == startR && nc == startC) {
            return loopPath.size() >= 4 && loopPath.size() == loopCells.size();
        }
    }
    return false;
}

bool PuzzleSolver::backtrack(int r, int c) {
    backtrackSteps++;
    if (backtrackSteps > 5000) return false;
    

    if (loopPath.size() == loopCells.size()) {
        // Перевіряємо, чи можна замкнути петлю
        if (canCloseLoop(r, c, loopPath[0].first, loopPath[0].second)) {
            return true;
        }
        return false;
    }
    
    // Пробуємо кожен напрямок
    const int dr[4] = {0, 1, 0, -1};
    const int dc[4] = {1, 0, -1, 0};
    
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (nr >= 0 && nr < gridSize && nc >= 0 && nc < gridSize && 
            grid[nr][nc] != 2 && visited.find(make_pair(nr, nc)) == visited.end()) {
            
            visited.insert(make_pair(nr, nc));
            loopPath.push_back(make_pair(nr, nc));
            
            if (backtrack(nr, nc)) {
                return true;
            }
            
            // Відкат (backtracking)
            visited.erase(make_pair(nr, nc));
            loopPath.pop_back();
        }
    }
    
    return false;
}

bool PuzzleSolver::loadCluesFromFile(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: Could not open file" << filename << endl;
        return false;
    }
    
    clues.clear(); // Очищаємо старі підказки
    
    string line;
    while (getline(infile, line)) {
        // Пропускаємо порожні рядки та коментарі
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        istringstream iss(line);
        int row, col, count;
        string direction;
        
        if (iss >> row >> col >> count >> direction) {
            // Перевірка коректності даних
            if (row < 0 || row >= gridSize || col < 0 || col >= gridSize) {
                cerr << "Error: Incorrect coordinates (" << row << "," << col << ")" << endl;
                continue;
            }
            
            if (count <= 0) {
                cerr << "Error: Invalid hint number" << count << endl;
                continue;
            }
            
            if (direction != "right" && direction != "left" && direction != "down" && direction != "up") {
                cerr << "Error: Unknown direction" << direction << endl;
                continue;
            }
            
            clues.emplace_back(row, col, count, direction);
        } else {
            cerr << "Error: Incorrect string format: " << line << endl;
        }
    }
    
    infile.close();
    
    cout << "Downloaded " << clues.size() << " clues." << endl;
    return !clues.empty(); // Успіх, якщо завантажена хоча б одна підказка
}

void PuzzleSolver::addClue(int row, int col, int count, const string& direction) {
    clues.emplace_back(row, col, count, direction);
}

void PuzzleSolver::initClues() {
    // Додаємо підказки з головоломки на зображенні (рядок, стовпець, число, напрямок)
    clues.emplace_back(0, 6, 1, "right");
    clues.emplace_back(2, 3, 2, "down");
    clues.emplace_back(3, 4, 1, "up");
    clues.emplace_back(3, 7, 2, "left");
    clues.emplace_back(4, 0, 2, "right");
    clues.emplace_back(4, 2, 2, "up");
    clues.emplace_back(5, 3, 1, "up");
    clues.emplace_back(5, 6, 1, "down");
    clues.emplace_back(5, 5, 2, "left");
    clues.emplace_back(7, 1, 3, "up");
    clues.emplace_back(7, 9, 2, "up");
    clues.emplace_back(9, 0, 3, "up");
    clues.emplace_back(9, 6, 2, "left");
}

bool PuzzleSolver::verify() {
    // Перевіряємо, що всі зафарбовані клітини не дотикаються по горизонталі та вертикалі
    for (const auto& cell : shadedCells) {
        const int dr[4] = {0, 1, 0, -1};
        const int dc[4] = {1, 0, -1, 0};
        
        for (int i = 0; i < 4; ++i) {
            int nr = cell.first + dr[i];
            int nc = cell.second + dc[i];
            if (nr >= 0 && nr < gridSize && nc >= 0 && nc < gridSize) {
                if (shadedCells.find(make_pair(nr, nc)) != shadedCells.end()) {
                    cout << "Error: the colored cells touch each other (" 
                         << cell.first << "," << cell.second << ") і (" 
                         << nr << "," << nc << ")" << endl;
                    return false;
                }
            }
        }
    }
    
    // Перевіряємо, що всі підказки задоволені
    for (const auto& clue : clues) {
        int shadedCount = 0;
        
        // Рахуємо зафарбовані клітини у вказаному напрямку
        if (clue.direction == "right") {
            for (int i = clue.col + 1; i < gridSize; ++i) {
                if (shadedCells.find(make_pair(clue.row, i)) != shadedCells.end()) {
                    shadedCount++;
                }
            }
        } else if (clue.direction == "left") {
            for (int i = clue.col - 1; i >= 0; --i) {
                if (shadedCells.find(make_pair(clue.row, i)) != shadedCells.end()) {
                    shadedCount++;
                }
            }
        } else if (clue.direction == "down") {
            for (int i = clue.row + 1; i < gridSize; ++i) {
                if (shadedCells.find(make_pair(i, clue.col)) != shadedCells.end()) {
                    shadedCount++;
                }
            }
        } else if (clue.direction == "up") {
            for (int i = clue.row - 1; i >= 0; --i) {
                if (shadedCells.find(make_pair(i, clue.col)) != shadedCells.end()) {
                    shadedCount++;
                }
            }
        }
        
        if (shadedCount != clue.count) {
            cout << "Error: clue in (" << clue.row << "," << clue.col 
                 << ") requires " << clue.count << " of painted cells, but found " 
                 << shadedCount << endl;
            return false;
        }
    }
    
    return true;
}

bool PuzzleSolver::determineInitialShadedCells() {
    bool changed = true;
    
    // Продовжуємо ітерації, поки щось змінюється
    while (changed) {
        changed = false;
        
        // Пробуємо задовольнити кожну підказку
        for (const auto& clue : clues) {
            // Визначаємо діапазон клітин, які можуть бути зафарбовані за цією підказкою
            vector<pair<int, int>> potentialCells;
            
            if (clue.direction == "right") {
                for (int i = clue.col + 1; i < gridSize; ++i) {
                    potentialCells.push_back(make_pair(clue.row, i));
                }
            } else if (clue.direction == "left") {
                for (int i = clue.col - 1; i >= 0; --i) {
                    potentialCells.push_back(make_pair(clue.row, i));
                }
            } else if (clue.direction == "down") {
                for (int i = clue.row + 1; i < gridSize; ++i) {
                    potentialCells.push_back(make_pair(i, clue.col));
                }
            } else if (clue.direction == "up") {
                for (int i = clue.row - 1; i >= 0; --i) {
                    potentialCells.push_back(make_pair(i, clue.col));
                }
            }
            
            // Рахуємо вже зафарбовані клітини в цьому напрямку
            int alreadyShaded = 0;
            vector<pair<int, int>> validCandidates;
            
            for (const auto& cell : potentialCells) {
                if (shadedCells.find(cell) != shadedCells.end()) {
                    alreadyShaded++;
                } else if (canBeShaded(cell.first, cell.second)) {
                    validCandidates.push_back(cell);
                }
            }
            
            // Якщо вже зафарбовано достатньо клітин, пропускаємо
            if (alreadyShaded == clue.count) {
                continue;
            }
            
            // Якщо зафарбовано забагато, помилка
            if (alreadyShaded > clue.count) {
                cout << "Error: too many filled cells for hint in (" 
                     << clue.row << "," << clue.col << ")" << endl;
                return false;
            }
            
            // Перевіряємо, чи результат clue.count - alreadyShaded не є від'ємним
            if (clue.count >= alreadyShaded) {
                size_t remainingToShade = static_cast<size_t>(clue.count - alreadyShaded);
                
                // Якщо не вистачає валідних кандидатів, помилка
                if (validCandidates.size() < remainingToShade) {
                    cout << "Error: Not enough valid candidates for hint in (" 
                         << clue.row << "," << clue.col << ")" << endl;
                    return false;
                }
                
                // Якщо є рівно стільки валідних кандидатів, скільки потрібно,
                // зафарбовуємо їх усі
                if (validCandidates.size() == remainingToShade) {
                    for (const auto& cell : validCandidates) {
                        if (shadedCells.find(cell) == shadedCells.end()) {
                            shadedCells.insert(cell);
                            grid[cell.first][cell.second] = 2;
                            changed = true;
                        }
                    }
                }
            }
        }
    }
    
    // Тепер пробуємо всі можливі комбінації для решти підказок
    // (У повному рішенні потрібно було б використовувати пошук з поверненням)
    // Для простоти будемо вважати, що первинний перебір задовольнив усі підказки
    
    return true;
}

bool PuzzleSolver::findLoop() {

    private:
    // Returns number of unshaded and unvisited neighbors for a cell
    int countAvailableNeighbors(int r, int c);
    
    // Finds the best starting point for the loop based on neighbor count
    pair<int, int> findBestStartingPoint();
    
    // New backtracking function with heuristic improvements
    bool backtrackHeuristic(int r, int c, int depth);
    
    // Helper function to check if we can close the loop
    bool canCloseLoopFrom(int r, int c, const pair<int, int>& startCell);
    
    // Greedy path building helper
    bool buildGreedyPath(int r, int c);

// Implementation of the new methods for PuzzleSolver class

int PuzzleSolver::countAvailableNeighbors(int r, int c) {
    const int dr[4] = {0, 1, 0, -1};
    const int dc[4] = {1, 0, -1, 0};
    
    int count = 0;
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (nr >= 0 && nr < gridSize && nc >= 0 && nc < gridSize && 
            grid[nr][nc] != 2 && visited.find(make_pair(nr, nc)) == visited.end()) {
            count++;
        }
    }
    return count;
}

pair<int, int> PuzzleSolver::findBestStartingPoint() {
    // Start by finding cells with minimal neighbors (likely corners)
    vector<pair<int, int>> candidates;
    int minNeighbors = 5; // More than maximum possible (4)
    
    for (const auto& cell : loopCells) {
        int neighbors = countAvailableNeighbors(cell.first, cell.second);
        
        // Skip cells with only one neighbor as they would be dead-ends
        if (neighbors <= 1) continue;
        
        if (neighbors < minNeighbors) {
            minNeighbors = neighbors;
            candidates.clear();
            candidates.push_back(cell);
        } else if (neighbors == minNeighbors) {
            candidates.push_back(cell);
        }
    }
    
    // If no good candidates, use the first available cell
    if (candidates.empty() && !loopCells.empty()) {
        return loopCells[0];
    }
    
    // Choose a corner cell (cell with exactly 2 neighbors) if available
    for (const auto& cell : candidates) {
        if (countAvailableNeighbors(cell.first, cell.second) == 2) {
            return cell;
        }
    }
    
    // Otherwise, use the first candidate
    return candidates.empty() ? loopCells[0] : candidates[0];
}

bool PuzzleSolver::canCloseLoopFrom(int r, int c, const pair<int, int>& startCell) {
    const int dr[4] = {0, 1, 0, -1};
    const int dc[4] = {1, 0, -1, 0};
    
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (nr == startCell.first && nc == startCell.second) {
            return loopPath.size() >= 4 && loopPath.size() == loopCells.size();
        }
    }
    return false;
}

bool PuzzleSolver::buildGreedyPath(int r, int c) {
    // Starting from (r,c), build a path that prefers cells with fewer neighbors
    const int dr[4] = {0, 1, 0, -1};
    const int dc[4] = {1, 0, -1, 0};
    
    pair<int, int> startCell = make_pair(r, c);
    visited.insert(startCell);
    loopPath.push_back(startCell);
    
    pair<int, int> current = startCell;
    bool foundNext = true;
    
    while (foundNext && loopPath.size() < loopCells.size()) {
        foundNext = false;
        int minNeighbors = 5; // More than maximum possible
        pair<int, int> nextCell;
        
        // Find the neighbor with fewest available neighbors
        for (int i = 0; i < 4; ++i) {
            int nr = current.first + dr[i];
            int nc = current.second + dc[i];
            pair<int, int> neighbor = make_pair(nr, nc);
            
            if (nr >= 0 && nr < gridSize && nc >= 0 && nc < gridSize && 
                grid[nr][nc] != 2 && visited.find(neighbor) == visited.end()) {
                
                int neighbors = countAvailableNeighbors(nr, nc);
                
                // If this is the last cell and can close the loop, choose it immediately
                if (loopPath.size() == loopCells.size() - 1 && 
                    canCloseLoopFrom(nr, nc, startCell)) {
                    nextCell = neighbor;
                    foundNext = true;
                    break;
                }
                
                // Skip cells with only one neighbor (would be dead-ends)
                if (neighbors < 1) continue;
                
                if (neighbors < minNeighbors) {
                    minNeighbors = neighbors;
                    nextCell = neighbor;
                    foundNext = true;
                }
            }
        }
        
        if (foundNext) {
            visited.insert(nextCell);
            loopPath.push_back(nextCell);
            current = nextCell;
        }
    }
    
    // Check if we've completed a valid loop
    if (loopPath.size() == loopCells.size()) {
        return canCloseLoopFrom(current.first, current.second, startCell);
    }
    
    return false;
}

bool PuzzleSolver::backtrackHeuristic(int r, int c, int depth) {
    // Check if we've found a solution
    if (loopPath.size() == loopCells.size()) {
        return canCloseLoopFrom(r, c, loopPath[0]);
    }
    
    // Stop if we've exceeded the step limitin
    if (depth > 5000) return false;
    
    // Direction vectors for neighbors
    const int dr[4] = {0, 1, 0, -1};
    const int dc[4] = {1, 0, -1, 0};
    
    // Count available neighbors for priority
    vector<pair<int, pair<int, int>>> neighbors;
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        pair<int, int> neighbor = make_pair(nr, nc);
        
        if (nr >= 0 && nr < gridSize && nc >= 0 && nc < gridSize && 
            grid[nr][nc] != 2 && visited.find(neighbor) == visited.end()) {
            
            // Calculate priority based on number of available neighbors
            // Fewer neighbors = higher priority
            int priority = countAvailableNeighbors(nr, nc);
            neighbors.push_back(make_pair(priority, neighbor));
        }
    }
    
    // Sort neighbors by priority (fewer available neighbors first)
    sort(neighbors.begin(), neighbors.end());
    
    // Try each neighbor in order of priority
    for (const auto& neighbor_pair : neighbors) {
        pair<int, int> neighbor = neighbor_pair.second;
        
        visited.insert(neighbor);
        loopPath.push_back(neighbor);
        
        if (backtrackHeuristic(neighbor.first, neighbor.second, depth + 1)) {
            return true;
        }
        
        // Backtrack
        visited.erase(neighbor);
        loopPath.pop_back();
    }
    
    return false;
}

// Update the findLoop method to use our improved algorithms
bool PuzzleSolver::findLoop() {
    // Clear previous state
    visited.clear();
    loopPath.clear();
    
    if (loopCells.empty()) {
        cout << "No loop cells found." << endl;
        return false;
    }
    
    // Try the greedy approach first
    pair<int, int> startCell = findBestStartingPoint();
    
    // Reset for the greedy attempt
    visited.clear();
    loopPath.clear();
    
    // Try greedy path-building first
    if (buildGreedyPath(startCell.first, startCell.second)) {
        cout << "Found solution using greedy approach!" << endl;
        return true;
    }
    
    // If greedy approach fails, try improved backtracking
    cout << "Greedy approach failed, trying improved backtracking..." << endl;
    
    // Reset for backtracking attempt
    visited.clear();
    loopPath.clear();
    
    // Start with a good corner cell
    startCell = findBestStartingPoint();
    visited.insert(startCell);
    loopPath.push_back(startCell);
    
    return backtrackHeuristic(startCell.first, startCell.second, 0);
}
    // Сброс счётчика шагов
   /* PuzzleSolver::backtrack(0, 0);  // Это трюк не сработает — лучше использовать нестатическую переменную


    if (loopCells.empty()) {
        cout << "No loop cells found." << endl;
        return false;
    }

    pair<int, int> startCell = loopCells[0];
    visited.insert(startCell);
    loopPath.push_back(startCell);

    // Сброс счётчика шагов
    extern int steps;
    steps = 0;

    return backtrack(startCell.first, startCell.second);
}
*/
}


bool PuzzleSolver::solve() {
    backtrackSteps = 0;
    // Очищаємо попередні результати
    shadedCells.clear();
    loopCells.clear();
    loopPath.clear();
    visited.clear();
    grid.assign(gridSize, vector<int>(gridSize, 0));
    
    // Якщо підказок немає, завантажуємо стандартні
    if (clues.empty()) {
        initClues();
    }
    
    // Крок 1: Визначаємо первинні зафарбовані клітини на основі підказок
    if (!determineInitialShadedCells()) {
        cout << "The initial stained cells could not be identified." << endl;
        return false;
    }
    
    // Крок 2: Визначаємо всі незафарбовані клітини, які мають бути частиною петлі
    for (int r = 0; r < gridSize; ++r) {
        for (int c = 0; c < gridSize; ++c) {
            if (grid[r][c] != 2) {  // Якщо не зафарбована
                loopCells.push_back(make_pair(r, c));
                grid[r][c] = 0;  // явно позначаємо як частину петлі
            }
        }
    }
    
    // Крок 3: Намагаємося знайти замкнену петлю
    if (!findLoop()) {
        cout << "Could not find closed loop." << endl;
        return false;
    }
    
    // Крок 4: Перевіряємо, що рішення задовольняє всім вимогам
    if (!verify()) {
        cout << "The solution does not satisfy all requirements." << endl;
        return false;
    }
    
    return true;
}

void PuzzleSolver::printSolution() {
    // Створюємо сітку для візуалізації рішення
    vector<vector<string>> solutionGrid(gridSize, vector<string>(gridSize, " "));

    // Позначаємо клітини з підказками
    for (const auto& clue : clues) {
        string arrow;
        if (clue.direction == "right") {
            arrow = to_string(clue.count) + "R";
        } else if (clue.direction == "left") {
            arrow = to_string(clue.count) + "L";
        } else if (clue.direction == "down") {
            arrow = to_string(clue.count) + "D";
        } else if (clue.direction == "up") {
            arrow = to_string(clue.count) + "U";
        }

        solutionGrid[clue.row][clue.col] = arrow;
    }

    // Позначаємо зафарбовані клітини
    for (const auto& cell : shadedCells) {
        if (solutionGrid[cell.first][cell.second] == " ") {
            solutionGrid[cell.first][cell.second] = "#";
        }
    }

    // Позначаємо шлях петлі
    if (!loopPath.empty()) {
        set<pair<int, int>> pathSet(loopPath.begin(), loopPath.end());

        for (const auto& cell : loopPath) {
            if (solutionGrid[cell.first][cell.second] == " ") {
                solutionGrid[cell.first][cell.second] = "o";
            }
        }
    }

    // Виводимо сітку рішення
    string horizontalLine = "+";
    for (int i = 0; i < gridSize; ++i) {
        horizontalLine += "---+";
    }
    cout << horizontalLine << endl;

    for (int r = 0; r < gridSize; ++r) {
        cout << "|";
        for (int c = 0; c < gridSize; ++c) {
            string cell = solutionGrid[r][c];
            if (cell.length() == 1) {
                cout << " " << cell << " |";
            } else if (cell.length() == 2) {
                cout << cell << "|";
            } else {
                cout << cell << "|";
            }
        }
        cout << endl << horizontalLine << endl;
    }
}


bool PuzzleSolver::exportSolution(const string& filename) {
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "Error: Could not open file" << filename << " for recording" << endl;
        return false;
    }
    
    // Спочатку записуємо розмір сітки
    outfile << gridSize << endl << endl;
    
    // Записуємо підказки
    outfile << "# Clue (row, column, number, direction)" << endl;
    for (const auto& clue : clues) {
        outfile << clue.row << " " << clue.col << " " << clue.count << " " << clue.direction << endl;
    }
    outfile << endl;
    
    // Записуємо зафарбовані клітини
    outfile << "# Colored cells (row, column)" << endl;
    for (const auto& cell : shadedCells) {
        outfile << cell.first << " " << cell.second << endl;
    }
    outfile << endl;
    
    // Записуємо шлях петлі
    outfile <<"# Loop path (row, column)" << endl;
    for (const auto& cell : loopPath) {
        outfile << cell.first << " " << cell.second << endl;
    }
    
    outfile.close();
    cout << "Solution successfully saved to file" << filename << endl;
    return true;
}