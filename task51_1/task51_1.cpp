/*  ----------------------------------------------------------------<Header>-
    Name:       task51_1.cpp
    Title:      Shikaku Puzzle Solver

    Group:      TV-41
    Student:    Stryzhakov M.D.
    Written:    2025-05-18
    Revised:    2025-05-18

    Description: Console program for solving Shikaku puzzles (rectangle division).
                 Supports manual and file input. Outputs solution as a grid with legend.
 ------------------------------------------------------------------</Header>-*/

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::ifstream;
using std::setw;
using std::string;

<<<<<<< HEAD:task51/task51_11.cpp
struct Cell {
    int value;  // Значение в клетке (0 если пусто)
    bool assigned;  // Присвоена ли клетка какому-то прямоугольнику
};

struct Number {
    int row, col;  // Координаты числа (0-based)
    int value;  // Значение числа
=======
/*  ---------------------------------------------------------------------[<]-
    Struct: cell_t
    Synopsis: Represents a single cell of the puzzle grid.
 ---------------------------------------------------------------------[>]-*/
struct cell_t
{
    int value;
    bool assigned;
};

/*  ---------------------------------------------------------------------[<]-
    Struct: number_t
    Synopsis: Represents a clue number and its position in the grid.
 ---------------------------------------------------------------------[>]-*/
struct number_t
{
    int row;
    int col;
    int value;
>>>>>>> a478275 (debug):task51_1/task51_11.cpp
};

/*  ---------------------------------------------------------------------[<]-
    Class: shikaku_solver_t
    Synopsis: Provides functionality to solve Shikaku puzzles by recursive
              rectangle assignment and backtracking.
 ---------------------------------------------------------------------[>]-*/
class shikaku_solver_t
{
private:
<<<<<<< HEAD:task51/task51_11.cpp
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
=======
    int rows;
    int cols;
    vector<vector<cell_t>> grid;
    vector<number_t> numbers;
    vector<vector<int>> solution;

public:
    /*  ---------------------------------------------------------------------[<]-
        Constructor: shikaku_solver_t
        Synopsis: Initializes the solver with a grid of given size.
     ---------------------------------------------------------------------[>]-*/
    shikaku_solver_t(int n_rows, int n_cols)
        : rows(n_rows), cols(n_cols),
          grid(n_rows, vector<cell_t>(n_cols, {0, false})),
          solution(n_rows, vector<int>(n_cols, -1))
    {
    }

    /*  ---------------------------------------------------------------------[<]-
        Method: add_number
        Synopsis: Adds a clue number at a specified position on the grid.
     ---------------------------------------------------------------------[>]-*/
    void add_number(int row, int col, int value)
    {
        int r = row - 1;
        int c = col - 1;
        if (r < 0 || r >= rows || c < 0 || c >= cols)
        {
>>>>>>> a478275 (debug):task51_1/task51_11.cpp
            cerr << "Invalid coordinates: (" << row << ", " << col << ")" << endl;
            return;
        }
        grid[r][c].value = value;
        numbers.push_back({r, c, value});
    }

    /*  ---------------------------------------------------------------------[<]-
        Method: solve
        Synopsis: Attempts to find a valid solution for the puzzle.
        Returns: True if a solution is found, false otherwise.
     ---------------------------------------------------------------------[>]-*/
    bool solve()
    {
        return solve_recursive(0);
    }

<<<<<<< HEAD:task51/task51_11.cpp
    bool isValidRectangle(int numIndex, int startRow, int startCol, int endRow, int endCol) {
        // Проверяем, что прямоугольник содержит нужное количество клеток
        int rectSize = (endRow - startRow + 1) * (endCol - startCol + 1);
        if (rectSize != numbers[numIndex].value)
            return false;

        // Проверяем, что все клетки в прямоугольнике свободны
        for (int r = startRow; r <= endRow; r++) {
            for (int c = startCol; c <= endCol; c++) {
=======
    /*  ---------------------------------------------------------------------[<]-
        Method: is_valid_rectangle
        Synopsis: Checks if a rectangle can be assigned for a given clue correct area, not overlapping with existing rectangles, contains the clue's position.
     ---------------------------------------------------------------------[>]-*/
    bool is_valid_rectangle(int num_index, int start_row, int start_col, int end_row, int end_col)
    {
        int rect_size = (end_row - start_row + 1) * (end_col - start_col + 1);
        if (rect_size != numbers[num_index].value)
        {
            return false;
        }
        for (int r = start_row; r <= end_row; r++)
        {
            for (int c = start_col; c <= end_col; c++)
            {
>>>>>>> a478275 (debug):task51_1/task51_11.cpp
                if (grid[r][c].assigned)
                {
                    return false;
                }
            }
        }
<<<<<<< HEAD:task51/task51_11.cpp

        // Проверяем, что прямоугольник содержит число
        int numRow = numbers[numIndex].row;
        int numCol = numbers[numIndex].col;
        return (startRow <= numRow && numRow <= endRow && 
                startCol <= numCol && numCol <= endCol);
=======
        int nr = numbers[num_index].row;
        int nc = numbers[num_index].col;
        return (start_row <= nr && nr <= end_row && start_col <= nc && nc <= end_col);
>>>>>>> a478275 (debug):task51_1/task51_11.cpp
    }

    /*  ---------------------------------------------------------------------[<]-
        Method: assign_rectangle
        Synopsis: Marks or unmarks a rectangle as assigned to a specific clue.
     ---------------------------------------------------------------------[>]-*/
    void assign_rectangle(int num_index, int start_row, int start_col, int end_row, int end_col, bool assign)
    {
        for (int r = start_row; r <= end_row; r++)
        {
            for (int c = start_col; c <= end_col; c++)
            {
                grid[r][c].assigned = assign;
                if (assign)
                {
                    solution[r][c] = num_index;
                }
                else if (solution[r][c] == num_index)
                {
                    solution[r][c] = -1;
                }
            }
        }
    }

<<<<<<< HEAD:task51/task51_11.cpp
    bool solveRecursive(int numIndex) {
        // Если все числа обработаны, проверяем корректность решения
        if (numIndex >= numbers.size()) {
            // Убедимся, что все клетки назначены
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
=======
    /*  ---------------------------------------------------------------------[<]-
        Method: solve_recursive
        Synopsis: Recursively tries to assign rectangles for all clues and finds
                  a valid solution for the Shikaku puzzle by backtracking.
     ---------------------------------------------------------------------[>]-*/
    bool solve_recursive(int num_index)
    {
        if (num_index >= (int)numbers.size())
        {
            for (int r = 0; r < rows; r++)
            {
                for (int c = 0; c < cols; c++)
                {
>>>>>>> a478275 (debug):task51_1/task51_11.cpp
                    if (!grid[r][c].assigned)
                    {
                        return false;
                    }
                }
            }
            return true;
        }
        int nr = numbers[num_index].row;
        int nc = numbers[num_index].col;
        int val = numbers[num_index].value;

<<<<<<< HEAD:task51/task51_11.cpp
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
=======
        for (int width = 1; width <= cols; width++)
        {
            for (int height = 1; height <= rows; height++)
            {
                if (width * height != val)
                {
                    continue;
                }
                for (int sc = std::max(0, nc - width + 1); sc <= std::min(nc, cols - width); sc++)
                {
                    for (int sr = std::max(0, nr - height + 1); sr <= std::min(nr, rows - height); sr++)
                    {
                        int ec = sc + width - 1;
                        int er = sr + height - 1;
                        if (is_valid_rectangle(num_index, sr, sc, er, ec))
                        {
                            assign_rectangle(num_index, sr, sc, er, ec, true);
                            if (solve_recursive(num_index + 1))
                            {
                                return true;
                            }
                            assign_rectangle(num_index, sr, sc, er, ec, false);
>>>>>>> a478275 (debug):task51_1/task51_11.cpp
                        }
                    }
                }
            }
        }
        return false;
    }

<<<<<<< HEAD:task51/task51_11.cpp
    void printSolution() {
<<<<<<< HEAD:task51/task51_11.cpp
<<<<<<< HEAD:task51/task51_11.cpp
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
=======
    /*  ---------------------------------------------------------------------[<]-
        Method: print_solution
        Synopsis: Prints the grid with assigned rectangles and legend for clues.
     ---------------------------------------------------------------------[>]-*/
    void print_solution()
    {
        cout << "\nSolution:\n    ";
        for (int c = 0; c < cols; c++)
        {
            cout << setw(3) << c + 1 << " ";
        }
        cout << "\n";
        for (int r = 0; r < rows; r++)
        {
            cout << "    ";
            for (int c = 0; c < cols; c++)
            {
                cout << "+---";
            }
            cout << "+\n";
            if (r + 1 < 10)
            {
                cout << "  " << r + 1 << " ";
            }
            else
            {
                cout << " " << r + 1 << " ";
            }
            for (int c = 0; c < cols; c++)
            {
                cout << "| ";
                int id = solution[r][c];
                char ch = ' ';
                if (id >= 0)
                {
                    if (id < 26)
                    {
                        ch = 'A' + id;
                    }
                    else if (id < 52)
                    {
                        ch = 'a' + (id - 26);
                    }
                    else
                    {
                        ch = '0' + (id % 10);
                    }
                    cout << ch << " ";
                }
                else
                {
                    cout << "  ";
                }
            }
            cout << "|\n";
        }
        cout << "    ";
        for (int c = 0; c < cols; c++)
        {
            cout << "+---";
        }
        cout << "+\n";

        cout << "\nLegend:\n";
        for (size_t i = 0; i < numbers.size(); i++)
        {
>>>>>>> a478275 (debug):task51_1/task51_11.cpp
            char ch;
            if (i < 26)
                ch = 'A' + i;
            else if (i < 52)
                ch = 'a' + (i - 26);
            else
                ch = '0' + (i % 10);
            cout << ch << ": value = " << numbers[i].value
                 << " at (" << numbers[i].row + 1 << "," << numbers[i].col + 1 << ")\n";
        }
    }
};

/*  ---------------------------------------------------------------------[<]-
    Function: main
    Synopsis: Entry point. Reads user choice, loads puzzle data, calls solver,
              and prints the solution or error.
 ---------------------------------------------------------------------[>]-*/
int main()
{
    int rows = 0, cols = 0, num_count = 0, choice = 0;
    cout << "Choose input method:\n";
    cout << "1. Manual input\n";
    cout << "2. Load from clues.txt\n";
    cout << "3. Exit\n";
    cout << "Enter your choice (1 or 3): ";
    cin >> choice;

    if (choice == 1)
    {
        cout << "Enter grid size (rows cols): ";
        cin >> rows >> cols;
        shikaku_solver_t solver(rows, cols);

        cout << "Enter number of values: ";
        cin >> num_count;
        cout << "Enter positions and values (row col value, starting from 1):\n";
        for (int i = 0; i < num_count; i++)
        {
            int r, c, v;
            cin >> r >> c >> v;
            solver.add_number(r, c, v);
        }

        if (solver.solve())
        {
            solver.print_solution();
        }
        else
        {
            cout << "No solution found!" << endl;
        }
    }
    else if (choice == 2)
    {
        ifstream clues_file("clues.txt");
        if (!clues_file.is_open())
        {
            cerr << "Error opening clues.txt!" << endl;
            return 1;
        }
        clues_file >> rows >> cols >> num_count;
        shikaku_solver_t solver(rows, cols);
        for (int i = 0; i < num_count; i++)
        {
            int r, c, v;
            if (!(clues_file >> r >> c >> v))
            {
                cerr << "Error reading data from clues.txt!" << endl;
                return 1;
            }
            solver.add_number(r, c, v);
        }
        clues_file.close();

        if (solver.solve())
        {
            solver.print_solution();
        }
        else
        {
            cout << "No solution found!" << endl;
        }
    }
    else if (choice == 3)
    {
        cout << "Exiting program." << endl;
        return 0;
    }
    else
    {
        cout << "Invalid choice!" << endl;
        return 1;
    }
    return 0;
}
