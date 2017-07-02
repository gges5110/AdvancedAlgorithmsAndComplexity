/*
brute-force algorithm for Linear Programming
1. read the inequalities.
2. add m inequalities to guarantee that the amounts cannot be negative.
3. add 1 inequality to manage the infinity case
4. solve all the possible combinations of m inequalities, and take the one that give the max pleasure while keeping all the restrictions
5. write the values with 18 decimals
*/
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

using std::vector;
using std::pair;

typedef vector<vector<double>> matrix;

pair<int, vector<double>> solve_diet_problem(
    int n,
    int m,
    matrix A,
    vector<double> b,
    vector<double> c) {

  // Write your code here

  return {0, vector<double>(m, 0)};
}


int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Please provide a valid filename for testing." << std::endl;
  } else {
    std::ifstream fs;
    fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      fs.open(argv[1]);

      // Original main()
      int n, m;
      fs >> n >> m;
      matrix A(n, vector<double>(m));
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
          fs >> A[i][j];
        }
      }
      vector<double> b(n);
      for (int i = 0; i < n; i++) {
        fs >> b[i];
      }
      vector<double> c(m);
      for (int i = 0; i < m; i++) {
        fs >> c[i];
      }

      pair<int, vector<double>> ans = solve_diet_problem(n, m, A, b, c);

      switch (ans.first) {
        case -1:
          std::cout << "No solution\n";
          break;
        case 0:
          std::cout << "Bounded solution\n";
          for (int i = 0; i < m; i++) {
            std::cout << ans.second[i] << "\n";
            // std::cout << "%.18f%c", ans.second[i], " \n"[i + 1 == m];
          }
          break;
        case 1:
          std::cout << "Infinity\n";
          break;
      }
      // End of Original main()

      fs.close();
    } catch (std::ifstream::failure& e) {
      std::cerr << "Exception opening/reading file" << std::endl;
    }
  }
  return 0;
}
