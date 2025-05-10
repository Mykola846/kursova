#include "puzzle_solver.hpp"
#include <iostream>
#include <string>

using namespace std;

void printHelp() {
    cout << "Options:\n";
    cout << "  1: Load default clues\n";
    cout << "  2: Load clues from file\n";
    cout << "  3: Add clue manually\n";
    cout << "  4: Solve puzzle\n";
    cout << "  5: Print solution\n";
    cout << "  6: Export solution to file\n";
    cout << "  0: Exit\n";
    cout << "Select option: ";
}


int main() {
    PuzzleSolver solver(10); // Створюємо головоломку розміром 10x10
    bool solved = false;
    int choice;
    
    cout << "=== Loop Puzzle Solver ===\n";
cout << "Author: [Mykola Stryzhakov]\n\n";

    
    do {
        printHelp();
        cin >> choice;
        
        switch (choice) {
            case 0:
                cout << "Goodbye!" << endl;
                break;
            
            case 1: {
                solver.initClues();
                cout << "Standard tips loaded." << endl;
                break;
            }
            
            case 2: {
                string filename;
                cout << "Enter the file name: ";
                cin >> filename;
                if (solver.loadCluesFromFile(filename)) {
                    cout << "Clues successfully loaded from file." << endl;
                }
                break;
            }
            
            case 3: {
                int row, col, count;
                string direction;
                cout << "Enter a string (0-9): ";
                cin >> row;
                cout << "Enter the column (0-9): ";
                cin >> col;
                cout << "Enter the number of cells to be filled in: ";
                cin >> count;
                cout << "Enter direction (right, left, up, down):";
                cin >> direction;
                
                solver.addClue(row, col, count, direction);
                cout << "Clue added."<< endl;
                break;
            }
            
            case 4: {
                cout << "Solving the puzzle..." << endl;
                if (solver.solve()) {
                    cout << "The puzzle has been successfully solved!" << endl;
                    solved = true;
                } else {
                    cout << "The puzzle could not be solved." << endl;
                }
                break;
            }
            
            case 5: {
                if (solved) {
                    solver.printSolution();
                } else {
                    cout << "First solve the puzzle (option 4)." << endl;
                }
                break;
            }
            
            case 6: {
                if (solved) {
                    string filename;
                    cout << "Enter the name of the file to export:";
                    cin >> filename;
                    solver.exportSolution(filename);
                } else {
                    cout << "First, solve the puzzle (option 4)." << endl;
                }
                break;
            }
            
            default:
                cout << "Incorrect selection. Please try again." << endl;
        }
        
        cout << endl;
        
    } while (choice != 0);
    
    return 0;
}