
#ifndef SUDOKU_H
#define SUDOKU_H

#include <string>
#include "lib/csp-solver/solver.h"

class Sudoku {
  public:
    CSP problem;
    Variable *cells[9][9]{};
    Sudoku(): problem() {
      // Initialize pointers
      for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
          cells[i][j] = nullptr;
    }

    void loadStringClue(const std::string& puzzle);
    void setConstraints();
    std::vector<Variable *> getBox(int boxRow, int boxCol);
    void setVariables();
    void printBoard();
	void loadCSV(const std::string& fileName);
};
#endif //SUDOKU_H
