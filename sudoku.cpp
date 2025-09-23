
#include "sudoku.h"
#include "lib/csp-solver/solver.h"
#include "lib/csp-solver/constraints.h"

Variable *cells[9][9];
CSP problem;

void printBoard() {
  for (int row = 0; row < 9; row++) {
    printf("|");
    for (int col = 0; col < 9; col++) {
      if (cells[row][col]->isAssigned == false) {
        printf("x");
      } else {
        printf("%d", cells[row][col]->value);
      }
      if (col < 8) {
        printf(", ");
      }
    }
    printf("|\n");
  }
}

void setVariables() {
  std::vector<int> domain = {1,2,3,4,5,6,7,8,9};
  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      std::string name = "Cell" + std::to_string(row*9+col);
      cells[row][col] = problem.addVariable(name, domain);
    }
  }
}

std::vector<Variable *> getBox(int boxRow, int boxCol) {
  std::vector<Variable *> box;
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      box.push_back(cells[boxRow*3 + row][boxCol*3 + col]);
    }
  }
  return box;
}

void setConstraints() {
  for (int row = 0; row < 9; row++) {
    std::vector<Variable*> r;
    for (int col = 0; col < 9; col++) {
      r.push_back(cells[row][col]);
    }
    problem.addConstraint<AllDiff>(r);
  }
  for (int col = 0; col < 9; col++) {
    std::vector<Variable*> c;
    for (int row = 0; row < 9; row++) {
      c.push_back(cells[row][col]);
    }
    problem.addConstraint<AllDiff>(c);
  }
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      std::vector<Variable*> box = getBox(row, col);
      problem.addConstraint<AllDiff>(box);
    }
  }
}


void loadStringClue(const std::string& puzzle) {
  if (puzzle.size() != 81) {
    std::cout << "Invalid string size\n";
  }
  std::vector<int> domain = {1,2,3,4,5,6,7,8,9};
  for (int i = 0; i < 81; i++) {
    int row = i / 9;
    int col = i % 9;
    char c = puzzle[i];
    std::string name = "Cell" + std::to_string(row*9+col);
    int val = c - '0';
    if (val <= 9 && val >= 1) {
      cells[row][col] = problem.addVariable(name, {val});
      cells[row][col]->isAssigned = true;
      cells[row][col]->value = val;
    } else {
      cells[row][col] = problem.addVariable(name, domain);
    }
  }
}

int main(int argc, char* argv[]) {
  // setVariables();
  loadStringClue(argv[1]);
  std::cout << "Loaded\n";
  printBoard();
  setConstraints();
  if (solve(problem)) {
  	printBoard();
  } else {
	std::cout << "Solution does not exist\n";
  }
}