
#include "sudoku.h"
#include "lib/csp-solver/solver.h"
#include "lib/csp-solver/constraints.h"
#include <fstream>
#include <sstream>

void Sudoku::printBoard() {
  for (int row = 0; row < 9; row++) {
    if (row % 3 == 0) {
      printf("+-------+-------+-------+\n");
    }
    for (int col = 0; col < 9; col++) {
      if (col % 3 == 0) {
        printf("| ");
      }
      if (cells[row][col]->isAssigned == false) {
        printf("x ");
      } else {
        printf("\033[1;34m%d \033[0m", cells[row][col]->value);
      }
    }
    printf("|\n");
  }
  printf("+-------+-------+-------+\n");
}

void Sudoku::setVariables() {
  std::vector<int> domain = {1,2,3,4,5,6,7,8,9};
  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      std::string name = "Cell" + std::to_string(row*9+col);
      cells[row][col] = problem.addVariable(name, domain);
    }
  }
}

std::vector<Variable *> Sudoku::getBox(int boxRow, int boxCol) {
  std::vector<Variable *> box;
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      box.push_back(cells[boxRow*3 + row][boxCol*3 + col]);
    }
  }
  return box;
}

void Sudoku::setConstraints() {
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


void Sudoku::loadStringClue(const std::string& puzzle) {
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

void Sudoku::loadCSV(const std::string& fileName) {
  std::ifstream file(fileName);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open " << fileName << "\n";
    return;
  }
  std::vector<int> domain = {1,2,3,4,5,6,7,8,9};
  std::string line;
  int row = 0;
  while (getline(file, line) &&  row < 9) {
    std::stringstream ss(line);
    int col = 0;
    std::string cell;
    std::string name = "Cell" + std::to_string(row*9 + col);
    while (std::getline(ss, cell, ',') && col < 9) {
      int val = std::stoi(cell);
      if (val <= 9 && val >= 1) {
        cells[row][col] = problem.addVariable(name, {val});
        cells[row][col]->isAssigned = true;
        cells[row][col]->value = val;
      } else {
        cells[row][col] = problem.addVariable(name, domain);
      }
      col++;
    }
    row++;
  }

}

int main(int argc, char* argv[]) {
  // setVariables();
  Sudoku sudoku;
  std::string mode = argv[1];
  if (mode == "--string") {
    sudoku.loadStringClue(argv[2]);
  } else if (mode == "--csv") {
    sudoku.loadCSV(argv[2]);
  } else {
    std::cout << "Invalid mode\n";
    return 1;
  }
  sudoku.printBoard();
  sudoku.setConstraints();
  solve(sudoku.problem);
  sudoku.printBoard();
}