#ifndef PUZZLE_SOLVER_HPP
#define PUZZLE_SOLVER_HPP

#include <vector>
#include <set>
#include <string>
#include <utility>
#include "clue.hpp"

class PuzzleSolver {
private:
    int backtrackSteps;
    int gridSize;
    // Евристические вспомогательные методы
    int countAvailableNeighbors(int r, int c);
    std::pair<int, int> findBestStartingPoint();
    bool backtrackHeuristic(int r, int c, int depth);
    bool canCloseLoopFrom(int r, int c, const std::pair<int, int>& startCell);
    bool buildGreedyPath(int r, int c);

    std::vector<std::vector<int>> grid; // 0 = пусто, 1 = частина петлі, 2 = зафарбовано
    std::set<std::pair<int, int>> shadedCells; // зафарбовані клітини
    std::vector<Clue> clues; // підказки (числа зі стрілками)
    std::vector<std::pair<int, int>> loopCells; // клітини, які мають бути частиною петлі
    std::vector<std::pair<int, int>> loopPath; // порядок клітин у петлі
    std::set<std::pair<int, int>> visited; // відвідані клітини під час пошуку петлі

    // Перевіряє, чи клітина може бути зафарбована (немає прилеглих зафарбованих клітин)
    bool canBeShaded(int r, int c);

    // Перевіряє, чи клітина невідвіданого сусіда має
    bool hasUnvisitedNeighbor(int r, int c);

    // Перевіряє, чи можна замкнути петлю, повернувшись на початок
    bool canCloseLoop(int r, int c, int startR, int startC);

    // Рекурсивна функція із поверненням для пошуку петлі
    bool backtrack(int r, int c);

    // Визначення початкових зафарбованих клітин
    bool determineInitialShadedCells();

    // Пошук замкнутої петлі
    bool findLoop();

public:
    // Конструктор за замовчуванням
    PuzzleSolver(int size = 10);

    // Завантаження підказок із файлу
    bool loadCluesFromFile(const std::string& filename);

    // Додавання підказки вручну
    void addClue(int row, int col, int count, const std::string& direction);

    // Ініціалізація підказок вручну (для тестування)
    void initClues();

    // Перевірка коректності рішення
    bool verify();

    // Розв'язання головоломки
    bool solve();

    // Висновок рішення
    void printSolution();

    // Експорт рішення у файл
    bool exportSolution(const std::string& filename);
};

#endif 
