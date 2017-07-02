#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>

const double EPS = 1e-6;
const int PRECISION = 6;

typedef std::vector<double> Column;
typedef std::vector<double> Row;
typedef std::vector<Row> Matrix;
using std::ifstream;

struct Equation {
    Equation(const Matrix &a, const Column &b):
        a(a),
        b(b)
    {}

    Matrix a;
    Column b;
};

struct Position {
    Position(int column, int row):
        column(column),
        row(row)
    {}

    int column;
    int row;
};

Equation ReadEquation(ifstream &fs) {
  int size;
  fs >> size;
  Matrix a(size, std::vector <double> (size, 0.0));
  Column b(size, 0.0);
  for (int row = 0; row < size; ++row) {
    for (int column = 0; column < size; ++column)
      fs >> a[row][column];
    fs >> b[row];
  }
  return Equation(a, b);
}

Position SelectPivotElement(
  const Matrix &a,
  std::vector <bool> &used_rows,
  std::vector <bool> &used_columns) {
  // This algorithm selects the first free element.
  // You'll need to improve it to pass the problem.
  Position pivot_element(0, 0);
  while (used_rows[pivot_element.row])
      ++pivot_element.row;
  while (used_columns[pivot_element.column] || a[pivot_element.row][pivot_element.column] == 0)
      ++pivot_element.column;
  return pivot_element;
}

void SwapLines(Matrix &a, Column &b, std::vector <bool> &used_rows, Position &pivot_element) {
  std::swap(a[pivot_element.column], a[pivot_element.row]);
  std::swap(b[pivot_element.column], b[pivot_element.row]);
  std::swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
  pivot_element.row = pivot_element.column;
}

void SubtractLines(Matrix &a, Column &b, const Position &pivot_element, int row) {
  if (a[row][pivot_element.column] != 0) {
    double factor = a[row][pivot_element.column] / a[pivot_element.row][pivot_element.column];
    b[row] -= b[pivot_element.row] * factor;
    for (int i = 0; i < a[row].size(); ++i) {
      a[row][i] -= a[pivot_element.row][i] * factor;
    }
  }
}

void ProcessPivotElement(Matrix &a, Column &b, const Position &pivot_element) {    
    // Rescale to make pivot 1
    double scale = a[pivot_element.row][pivot_element.column];
    for (int column = 0; column < a[pivot_element.row].size(); ++column) {
      a[pivot_element.row][column] /= scale;
    }
    b[pivot_element.row] /= scale;
    // Subtract row from others to make other entries in the same column 0.
    for (int row = 0; row < a.size(); ++row) {
      if (row != pivot_element.row) {
        SubtractLines(a, b, pivot_element, row);
      }
    }
}

void MarkPivotElementUsed(const Position &pivot_element, std::vector <bool> &used_rows, std::vector <bool> &used_columns) {
  used_rows[pivot_element.row] = true;
  used_columns[pivot_element.column] = true;
}

void printDebug(const Matrix &a, const Column &b) {
  for (int i = 0; i < a.size(); ++i) {
    for (double element: a[i]) {
      std::cout << element << " ";
    }
    std::cout << "| " << b[i] << std::endl;
  }
}

Column SolveEquation(Equation equation) {
  Matrix &a = equation.a;
  Column &b = equation.b;
  int size = a.size();

  std::vector <bool> used_columns(size, false);
  std::vector <bool> used_rows(size, false);
  for (int step = 0; step < size; ++step) {
    Position pivot_element = SelectPivotElement(a, used_rows, used_columns);
    SwapLines(a, b, used_rows, pivot_element);
    ProcessPivotElement(a, b, pivot_element);
    MarkPivotElementUsed(pivot_element, used_rows, used_columns);
    // std::cout << "PrintDebug" << std::endl;
    // printDebug(a, b);
  }

  return b;
}

void PrintColumn(const Column &column) {
  int size = column.size();
  std::cout.precision(PRECISION);
  std::cout << std::fixed;
  for (int row = 0; row < size; ++row) {
    std::cout << column[row] << " ";
  }
  std::cout << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Please provide a valid filename for testing." << std::endl;
  } else {
    ifstream fs;
    fs.exceptions(ifstream::failbit | ifstream::badbit);
    try {
      fs.open(argv[1]);

      // Original main()
      Equation equation = ReadEquation(fs);
      Column solution = SolveEquation(equation);
      PrintColumn(solution);
      // End of Original main()

      fs.close();
    } catch (ifstream::failure& e) {
      std::cerr << "Exception opening/reading file" << std::endl;
    }
  }
  return 0;
}
