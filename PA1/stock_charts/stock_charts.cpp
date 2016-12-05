#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> // std::min
#include <memory>
#include <string>

using std::vector;
using std::cin;
using std::cout;
using std::string;
using std::endl;

class FlowGraph {

private:
    vector<vector<int>> C, T;
    int nodes;

    void initializeMatrix() {
        C.resize(nodes);
        T.resize(nodes);
        for (int i = 0; i < nodes; ++i) {
            C[i].resize(nodes, 0);
            T[i].resize(nodes, 0);
        }
    }

    int BFS(const vector<vector<int>> &C,
            const vector<vector<int>> &T,
            int s,
            int t,
            const vector<vector<int>> &F,
            vector<int> &P) {

            vector<int> M(nodes);
            P[s] = -2;
            M[s] = 1000000;

            vector<int> Q;
            Q.push_back(s);
            while (Q.size() > 0) {
                int u = Q.front();
                Q.erase(Q.begin());
                for (int v = 0; v < nodes; ++v) {
                    if (v != u) {
                        if (T[u][v] > 0 && P[v] == -1) {
                            P[v] = u;
                            M[v] = std::min(M[u], C[u][v] - F[u][v]);
                            if (v != t) {
                                Q.push_back(v);
                            } else {
                                return M[t];
                            }
                        }
                    }
                }
            }
        return 0;
    }

    void printPath(const vector<int> &P, int from, int to) const {
        int v = to;
        cout << "Print path." << endl;
        while (v != from) {
            cout << v << " <- ";
            v = P[v];
        }
        cout << v << endl;
    }

    void printMatrix(const vector<vector<int>> &C) const {
        for (int i = 0; i < nodes; ++i) {
            for (int j = 0; j < nodes; ++j) {
                cout << C[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

public:
    void read_data(const vector<vector<int>> &_C) {
        C = _C;
        T = _C;
        nodes = C.size();

    }

    void read_data(const string &filename) {
        std::ifstream fs(filename);
        fs >> nodes;
        initializeMatrix();

        int edges; fs >> edges;
        for (int i = 0; i < edges; ++i) {
            int from, to, capacity;
            fs >> from >> to >> capacity;
            C[from - 1][to - 1] = capacity;
            T[from - 1][to - 1] = capacity;
        }
    }

    int size() const {
        return nodes;
    }

    vector<vector<int>> max_flow(int from, int to) {
        int flow = 0;
        vector<vector<int>> F(nodes, vector<int>(nodes));

        while (true) {
            vector<int> P(nodes, -1);
            int m = BFS(C, T, from, to, F, P);

            if (m == 0) {
                break;
            } else {
                flow += m;
                int v = to;
                while (v != from) {
                    int u = P[v];
                    F[u][v] += m;
                    F[v][u] -= m;
                    T[u][v] -= m;
                    T[v][u] += m;
                    v = u;
                }
            }
        }

        return F;
    }
};

class StockCharts {
 public:
  void Solve(const string & filename) {
    vector<vector<int>> stock_data = ReadData(filename);
    int result = MinCharts(stock_data);
    WriteResponse(result);
  }

 private:
  vector<vector<int>> ReadData(const string & filename) {
      std::ifstream fs(filename);
      int num_stocks, num_points;
      fs >> num_stocks >> num_points;
      vector<vector<int>> stock_data(num_stocks, vector<int>(num_points));
      for (int i = 0; i < num_stocks; ++i)
        for (int j = 0; j < num_points; ++j) {
        fs >> stock_data[i][j];
      }
    return stock_data;
  }

  void WriteResponse(int result) {
    cout << result << "\n";
  }

  int MinCharts(const vector<vector<int>>& stock_data) {
    // Replace this incorrect greedy algorithm with an
    // algorithm that correctly finds the minimum number
    // of charts on which we can put all the stock data
    // without intersections of graphs on one chart.

    int num_stocks = stock_data.size();

    // Vector of charts; each chart is a vector of indices of individual stocks.
    vector<vector<bool>> charts(num_stocks, vector<bool>(num_stocks, false));
    for (int i = 0; i < num_stocks; ++i) {
        for (int j = 0; j < num_stocks; ++j) {
            if (i != j && compare(stock_data[i], stock_data[j])) {
                charts[i][j] = true;
            }
        }
    }

    vector<vector<int>> C = convertMatrix(charts);
    FlowGraph graph;
    graph.read_data(C);
    vector<vector<int>> F = graph.max_flow(0, graph.size() - 1);
    int n = 0;
    for (int i = 0; i < F.size(); ++i) {
        if (F[0][i] != 0) {
            n++;
        }
    }

    return num_stocks - n;
  }

  bool compare(const vector<int>& stock1, const vector<int>& stock2) {
    for (int i = 0; i < stock1.size(); ++i)
      if (stock1[i] >= stock2[i])
        return false;
    return true;
  }

  vector<vector<int>> convertMatrix(const vector<vector<bool>> &charts) {
      int num_stocks = charts.size();
      int nodes = num_stocks * 2 + 2;

      // node 0 is source, node nodes is sink.
      vector<vector<int>> C(nodes, vector<int>(nodes));

      // Connect source to all nodes on the left.
      for (int i = 1; i < num_stocks + 1; ++i) {
          C[0][i] = 1;
      }

      // Connect all nodes on the right to sink.
      for (int i = num_stocks + 1; i < nodes - 1; ++i) {
          C[i][nodes - 1] = 1;
      }

      for (int i = 0; i < num_stocks; ++i) {
          for (int j = 0; j < num_stocks; ++j) {
              if (charts[i][j]) {
                  C[j + 1][num_stocks + i + 1] = 1;
              }
          }
      }

      return C;
  }
};

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Please specify a file name for test." << endl;
    } else {
        std::ios_base::sync_with_stdio(false);
        string filename(argv[1]);
        StockCharts stock_charts;
        stock_charts.Solve(filename);
        return 0;
    }
}
