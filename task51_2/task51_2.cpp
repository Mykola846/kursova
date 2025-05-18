/*  ----------------------------------------------------------------<Header>-
    Name:       task51_2.cpp
    Title:      Yajilin Puzzle Solver

    Group:      TV-41
    Student:    Stryzhakov M.D.
    Written:    2025-05-18
    Revised:    2025-05-18

    Description: Console program for solving a puzzle with clues that indicate
                 how many black cells should be placed in a specific direction
                 from a clue cell. The program ensures black cells do not touch,
                 and, if possible, builds a single loop through all non-black cells.
                 Input via console or file. Solution is shown as an ASCII grid.
 ------------------------------------------------------------------</Header>-*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::ifstream;
using std::string;

/*  ---------------------------------------------------------------------[<]-
    Enum: CellType
    Synopsis: Types of cells on the puzzle grid.
 ---------------------------------------------------------------------[>]-*/
enum CellType { EMPTY, BLACK, CLUE, NOT_BLACK };

/*  ---------------------------------------------------------------------[<]-
    Struct: Clue
    Synopsis: Represents a clue and its parameters on the grid.
 ---------------------------------------------------------------------[>]-*/
struct Clue
{
    int row, col, count;
    char dir;
};

/*  ---------------------------------------------------------------------[<]-
    Struct: Cell
    Synopsis: Represents a single cell of the grid.
 ---------------------------------------------------------------------[>]-*/
struct Cell
{
    CellType type;
    int clueCount;
    char clueDir;
    Cell() : type(EMPTY), clueCount(0), clueDir('.') {}
};

int n, m;
vector<vector<Cell>> grid, bestGrid;
vector<Clue> clues;

int dx[4] = { -1, 1, 0, 0 }, dy[4] = { 0, 0, -1, 1 };

/*  ---------------------------------------------------------------------[<]-
    Function: inside
    Synopsis: Checks if (x, y) is inside the grid.
 ---------------------------------------------------------------------[>]-*/
bool inside(int x, int y)
{
    return x >= 0 && x < n && y >= 0 && y < m;
}

/*  ---------------------------------------------------------------------[<]-
    Function: checkNoTouch
    Synopsis: Checks that a BLACK cell at (x, y) does not touch another BLACK cell.
 --------------------------------------------------------------------[>]-*/
bool checkNoTouch(const vector<vector<Cell>>& g, int x, int y)
{
    for (int d = 0; d < 4; ++d)
    {
        int nx = x + dx[d], ny = y + dy[d];
        if (inside(nx, ny) && g[nx][ny].type == BLACK)
            return false;
    }
    return true;
}

/*  ---------------------------------------------------------------------[<]-
    Function: checkClues
    Synopsis: Checks if all clues are satisfied in the grid.
 ---------------------------------------------------------------------[>]-*/
bool checkClues(const vector<vector<Cell>>& g)
{
    for (const auto& clue : clues)
    {
        int cnt = 0, r = clue.row, c = clue.col;
        if (clue.dir == 'U')
            for (int i = r - 1; i >= 0; --i) if (g[i][c].type == BLACK) cnt++;
        if (clue.dir == 'D')
            for (int i = r + 1; i < n; ++i) if (g[i][c].type == BLACK) cnt++;
        if (clue.dir == 'L')
            for (int j = c - 1; j >= 0; --j) if (g[r][j].type == BLACK) cnt++;
        if (clue.dir == 'R')
            for (int j = c + 1; j < m; ++j) if (g[r][j].type == BLACK) cnt++;
        if (cnt != clue.count)
            return false;
    }
    return true;
}

/*  ---------------------------------------------------------------------[<]-
    Function: stepLogic
    Synopsis: Applies logic deductions for all clues (direct and forced moves).
 ---------------------------------------------------------------------[>]-*/
bool stepLogic(vector<vector<Cell>>& g)
{
    bool changed = false;
    for (auto& clue : clues)
    {
        int r = clue.row, c = clue.col, want = clue.count, free = 0;
        vector<std::pair<int, int>> cells;
        if (clue.dir == 'U')
            for (int i = r - 1; i >= 0; --i)
            {
                if (g[i][c].type == EMPTY) { free++; cells.emplace_back(i, c); }
                else if (g[i][c].type == BLACK) want--;
            }
        if (clue.dir == 'D')
            for (int i = r + 1; i < n; ++i)
            {
                if (g[i][c].type == EMPTY) { free++; cells.emplace_back(i, c); }
                else if (g[i][c].type == BLACK) want--;
            }
        if (clue.dir == 'L')
            for (int j = c - 1; j >= 0; --j)
            {
                if (g[r][j].type == EMPTY) { free++; cells.emplace_back(r, j); }
                else if (g[r][j].type == BLACK) want--;
            }
        if (clue.dir == 'R')
            for (int j = c + 1; j < m; ++j)
            {
                if (g[r][j].type == EMPTY) { free++; cells.emplace_back(r, j); }
                else if (g[r][j].type == BLACK) want--;
            }
        if (clue.count == 0)
        {
            for (auto [x, y] : cells)
                if (g[x][y].type == EMPTY) { g[x][y].type = NOT_BLACK; changed = true; }
        }
        if (want == free && free > 0)
        {
            for (auto [x, y] : cells)
                if (g[x][y].type == EMPTY && checkNoTouch(g, x, y))
                {
                    g[x][y].type = BLACK; changed = true;
                }
        }
    }
    return changed;
}

/*  ---------------------------------------------------------------------[<]-
    Function: isValidX
    Synopsis: Checks if the grid is valid: all BLACK cells do not touch, all clues are satisfied.
 ---------------------------------------------------------------------[>]-*/
bool isValidX(const vector<vector<Cell>>& g)
{
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (g[i][j].type == BLACK && !checkNoTouch(g, i, j)) return false;
    return checkClues(g);
}

/*  ---------------------------------------------------------------------[<]-
    Function: xSolver
    Synopsis: Recursively tries all placements of black cells (backtracking).
 ---------------------------------------------------------------------[>]-*/
bool xSolver(int idx, vector<vector<Cell>>& g)
{
    if (idx == n * m)
    {
        if (isValidX(g)) { bestGrid = g; return true; }
        return false;
    }
    int x = idx / m, y = idx % m;
    if (g[x][y].type == CLUE || g[x][y].type == BLACK || g[x][y].type == NOT_BLACK)
        return xSolver(idx + 1, g);

    if (xSolver(idx + 1, g)) return true;

    g[x][y].type = BLACK;
    if (checkNoTouch(g, x, y))
    {
        if (xSolver(idx + 1, g)) return true;
    }
    g[x][y].type = EMPTY;
    return false;
}

int cntLoop = 0;
vector<vector<bool>> vis;

/*  ---------------------------------------------------------------------[<]-
    Function: dfsLoop
    Synopsis: Depth-first search to count all reachable EMPTY cells.
 ---------------------------------------------------------------------[>]-*/
void dfsLoop(int x, int y)
{
    vis[x][y] = true; cntLoop++;
    for (int d = 0; d < 4; ++d)
    {
        int nx = x + dx[d], ny = y + dy[d];
        if (inside(nx, ny) && !vis[nx][ny] && bestGrid[nx][ny].type == EMPTY)
            dfsLoop(nx, ny);
    }
}

/*  ---------------------------------------------------------------------[<]-
    Function: canMakeLoop
    Synopsis: Checks if a loop can be made through all EMPTY cells.
 ---------------------------------------------------------------------[>]-*/
bool canMakeLoop()
{
    vis.assign(n, vector<bool>(m, false));
    int sx = -1, sy = -1, total = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (bestGrid[i][j].type == EMPTY)
            {
                total++;
                if (sx == -1) { sx = i; sy = j; }
            }
    if (sx == -1) return false;
    cntLoop = 0;
    dfsLoop(sx, sy);
    return cntLoop == total;
}

/*  ---------------------------------------------------------------------[<]-
    Function: printAsciiField
    Synopsis: Prints the grid as ASCII-art, showing black cells, clues, and empty cells.
 ---------------------------------------------------------------------[>]-*/
void printAsciiField(const vector<vector<Cell>>& g)
{
    cout << "   ";
    for (int j = 0; j < m; ++j) cout << j + 1 << " ";
    cout << endl;
    for (int i = 0; i < n; ++i)
    {
        if (i + 1 < 10) cout << " " << i + 1 << " ";
        else cout << i + 1 << " ";
        for (int j = 0; j < m; ++j)
        {
            if (g[i][j].type == BLACK) cout << "x ";
            else if (g[i][j].type == CLUE) cout << g[i][j].clueCount << g[i][j].clueDir;
            else cout << ". ";
        }
        cout << endl;
    }
}

/*  ---------------------------------------------------------------------[<]-
    Function: main
    Synopsis: Entry point. Loads puzzle, runs solver, prints result and loop status.
 ---------------------------------------------------------------------[>]-*/
int main()
{
    int choice;
    cout << "Select input method:\n";
    cout << "1. Manual input\n";
    cout << "2. Load from file (sample_clues.txt)\n";
    cout << "3. Exit\n";
    cout << "Your choice: ";
    cin >> choice;

    if (choice == 1)
    {
        cout << "Enter grid size (rows columns): ";
        cin >> n >> m;
        cout << "Enter number of clues: ";
        int clueNum; cin >> clueNum;
        clues.resize(clueNum);
        cout << "Enter clues (row col count dir):\n";
        for (int i = 0; i < clueNum; ++i)
        {
            int r, c, cnt; char dir;
            cin >> r >> c >> cnt >> dir;
            clues[i] = { r - 1, c - 1, cnt, dir };
        }
    }
    else if (choice == 2)
    {
        ifstream fin("sample_clues.txt");
        if (!fin.is_open())
        {
            cerr << "Could not open sample_clues.txt!" << endl;
            return 1;
        }
        int clueNum;
        fin >> n >> m >> clueNum;
        clues.resize(clueNum);
        for (int i = 0; i < clueNum; ++i)
        {
            int r, c, cnt; char dir;
            fin >> r >> c >> cnt >> dir;
            clues[i] = { r - 1, c - 1, cnt, dir };
        }
        fin.close();
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

    grid.assign(n, vector<Cell>(m));
    for (const auto& clue : clues)
    {
        grid[clue.row][clue.col].type = CLUE;
        grid[clue.row][clue.col].clueCount = clue.count;
        grid[clue.row][clue.col].clueDir = clue.dir;
    }

    while (stepLogic(grid));
    if (xSolver(0, grid))
    {
        cout << "\nFinal result:\n";
        printAsciiField(bestGrid);
        if (canMakeLoop())
        {
            cout << "A loop through all white cells can be built.\n";
        }
        else
        {
            cout << "No valid loop found (need more advanced algorithm).\n";
        }
    }
    else
    {
        cout << "No solution found!" << endl;
    }
    return 0;
}
