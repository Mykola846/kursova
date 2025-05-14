#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

struct Cell {
    int value;  // Значение в клетке (0 если пусто)
    bool assigned;  // Присвоена ли клетка какому-то прямоугольнику
};

struct Number {
    int row, col;  // Координаты числа (0-based)
    int value;  // Значение числа
};

class ShikakuSolver {
private:
    int rows, cols;
    vector<vector<Cell>> grid;
    vector<Number> numbers;
    vector<vector<int>> solution;  // Для хранения идентификаторов прямоугольников

public:
    ShikakuSolver(int r, int c) : rows(r), cols(c) {
        // Инициализация сетки
        grid.resize(rows, vector<Cell>(cols, {0, false}));
        solution.resize(rows, vector<int>(cols, -1));
    }

    void addNumber(int row, int col, int value) {
        // Преобразование координат из 1-based в 0-based
        int adjustedRow = row - 1;
        int adjustedCol = col - 1;
        
        // Проверка, что координаты находятся в допустимом диапазоне
        if (adjustedRow < 0 || adjustedRow >= rows || adjustedCol < 0 || adjustedCol >= cols) {
            cerr << "Invalid coordinates: (" << row << ", " << col << ")" << endl;
            return;
        }
        
        grid[adjustedRow][adjustedCol].value = value;
        numbers.push_back({adjustedRow, adjustedCol, value});
    }

    bool solve() {
        return solveRecursive(0);
    }

    bool isValidRectangle(int numIndex, int startRow, int startCol, int endRow, int endCol) {
        // Проверяем, что прямоугольник содержит нужное количество клеток
        int rectSize = (endRow - startRow + 1) * (endCol - startCol + 1);
        if (rectSize != numbers[numIndex].value)
            return false;

        // Проверяем, что все клетки в прямоугольнике свободны
        for (int r = startRow; r <= endRow; r++) {
            for (int c = startCol; c <= endCol; c++) {
                if (grid[r][c].assigned)
                    return false;
            }
        }

        // Проверяем, что прямоугольник содержит число
        int numRow = numbers[numIndex].row;
        int numCol = numbers[numIndex].col;
        return (startRow <= numRow && numRow <= endRow && 
                startCol <= numCol && numCol <= endCol);
    }

    void assignRectangle(int numIndex, int startRow, int startCol, int endRow, int endCol, bool assign) {
        for (int r = startRow; r <= endRow; r++) {
            for (int c = startCol; c <= endCol; c++) {
                grid[r][c].assigned = assign;
                if (assign)
                    solution[r][c] = numIndex;
                else if (solution[r][c] == numIndex)
                    solution[r][c] = -1;
            }
        }
    }

    bool solveRecursive(int numIndex) {
        // Если все числа обработаны, проверяем корректность решения
        if (numIndex >= numbers.size()) {
            // Убедимся, что все клетки назначены
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    if (!grid[r][c].assigned)
                        return false;
                }
            }
            return true;
        }

        int numRow = numbers[numIndex].row;
        int numCol = numbers[numIndex].col;
        int value = numbers[numIndex].value;

        // Перебираем все возможные прямоугольники
        for (int width = 1; width <= cols; width++) {
            for (int height = 1; height <= rows; height++) {
                // Пропускаем прямоугольники неправильного размера
                if (width * height != value)
                    continue;

                // Перебираем все возможные положения прямоугольника, содержащего число
                for (int startCol = max(0, numCol - width + 1); startCol <= min(numCol, cols - width); startCol++) {
                    for (int startRow = max(0, numRow - height + 1); startRow <= min(numRow, rows - height); startRow++) {
                        int endCol = startCol + width - 1;
                        int endRow = startRow + height - 1;

                        if (isValidRectangle(numIndex, startRow, startCol, endRow, endCol)) {
                            // Назначаем прямоугольник
                            assignRectangle(numIndex, startRow, startCol, endRow, endCol, true);
                            
                            // Рекурсивно решаем для следующего числа
                            if (solveRecursive(numIndex + 1))
                                return true;
                            
                            // Если не удалось найти решение, отменяем назначение
                            assignRectangle(numIndex, startRow, startCol, endRow, endCol, false);
                        }
                    }
                }
            }
        }

        return false;
    }

    void printSolution() {
<<<<<<< HEAD
=======
<<<<<<< HEAD:task51/task51_11.cpp
<<<<<<< HEAD:task51/task51_11.cpp
>>>>>>> f84e7c3 (fixed the coordinate marking at the top)
        // Вывод сетки с обозначением границ прямоугольников
        cout << "Solution:" << endl;
        for (int r = 0; r < rows; r++) {
            // Вывод горизонтальных линий
            for (int c = 0; c < cols; c++) {
                cout << "+";
                // Проверка, есть ли горизонтальная граница между текущей и верхней клеткой
                if (r > 0 && solution[r][c] != solution[r-1][c])
                    cout << "-";
                else
                    cout << " ";
            }
            cout << "+" << endl;
            
            for (int c = 0; c < cols; c++) {
                // Проверка, есть ли вертикальная граница между текущей и левой клеткой
                if (c > 0 && solution[r][c] != solution[r][c-1])
                    cout << "|";
                else
                    cout << " ";
                
                // Вывод значения, если оно есть
                if (grid[r][c].value > 0)
                    cout << grid[r][c].value;
                else
                    cout << " ";
            }
            cout << "|" << endl;
        }
        
        // Нижняя граница
<<<<<<< HEAD
=======
=======
    cout << "\nSolution:\n   ";
=======
    cout << "\nSolution:\n    ";
>>>>>>> 402a8d5 (fixed the coordinate marking at the top):task51_1/task51_11.cpp
    for (int c = 0; c < cols; c++) {
        cout << setw(3) << c + 1 << " ";
    }
    cout << "\n";


    for (int r = 0; r < rows; r++) {
<<<<<<< HEAD:task51/task51_11.cpp
        cout << "   ";
>>>>>>> b341ea4 (changed printSolution to make the output more intuitive):task51_1/task51_11.cpp
>>>>>>> f84e7c3 (fixed the coordinate marking at the top)
        for (int c = 0; c < cols; c++) {
            cout << "+---";
        }
        cout << "+\n";

        cout << " " << r + 1 << " ";
        for (int c = 0; c < cols; c++) {
            cout << "| ";
            int id = solution[r][c];
            if (id >= 0) {
                char ch;
                if (id < 26) ch = 'A' + id;
                else if (id < 52) ch = 'a' + (id - 26);
                else ch = '0' + (id % 10);
                cout << ch << " ";
            } else {
                cout << "  ";
            }
        }
        cout << "|\n";
    }

    cout << "   ";
=======
    cout << "    ";
>>>>>>> 402a8d5 (fixed the coordinate marking at the top):task51_1/task51_11.cpp
    for (int c = 0; c < cols; c++) {
        cout << "+---";
    }
    cout << "+\n";

    if (r + 1 < 10)
        cout << "  " << r + 1 << " ";
    else
        cout << " " << r + 1 << " ";

    for (int c = 0; c < cols; c++) {
        cout << "| ";
        int id = solution[r][c];
        if (id >= 0) {
            char ch;
            if (id < 26) ch = 'A' + id;
            else if (id < 52) ch = 'a' + (id - 26);
            else ch = '0' + (id % 10);
            cout << ch << " ";
        } else {
            cout << "  ";
        }
    }
    cout << "|\n";
}

cout << "    ";
for (int c = 0; c < cols; c++) {
    cout << "+---";
}
cout << "+\n";

cout << "\nLegend:\n";
for (int i = 0; i < numbers.size(); i++) {
    char ch;
    if (i < 26) ch = 'A' + i;
    else if (i < 52) ch = 'a' + (i - 26);
    else ch = '0' + (i % 10);
    cout << ch << ": value = " << numbers[i].value
         << " at (" << numbers[i].row + 1 << "," << numbers[i].col + 1 << ")\n";
    }
}

};

int main() {
    int rows, cols, numCount;
    int choice;
    cout << "Choose input method:" << endl;
    cout << "1. Manual input" << endl;
    cout << "2. Load from clues.txt" << endl;
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;

    if (choice == 1) {
        // Manual input
        cout << "Enter grid size (rows cols): ";
        cin >> rows >> cols;
        ShikakuSolver solver(rows, cols);

        cout << "Enter number of values: ";
        cin >> numCount;
        cout << "Enter positions and values (row col value, starting from 1):" << endl;
        for (int i = 0; i < numCount; i++) {
            int r, c, v;
            cin >> r >> c >> v;
            solver.addNumber(r, c, v);
        }

        if (solver.solve()) {
            solver.printSolution();
        } else {
            cout << "No solution found!" << endl;
        }

    } else if (choice == 2) {
        // Load from file
        ifstream cluesFile("clues.txt");
        if (!cluesFile.is_open()) {
            cerr << "Error opening clues.txt!" << endl;
            return 1;
        }

        // читаем сначала размеры
        cluesFile >> rows >> cols >> numCount;
        ShikakuSolver solver(rows, cols);

        for (int i = 0; i < numCount; i++) {
            int r, c, v;
            if (!(cluesFile >> r >> c >> v)) {
                cerr << "Error reading data from clues.txt!" << endl;
                return 1;
            }
            solver.addNumber(r, c, v);
        }

        cluesFile.close();

        if (solver.solve()) {
            solver.printSolution();
        } else {
            cout << "No solution found!" << endl;
        }

    } else {
        cout << "Invalid choice!" << endl;
        return 1;
    }

    return 0;
}