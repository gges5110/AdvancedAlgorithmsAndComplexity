#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm> // std::min

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

class MaxMatching {
public:
    void Solve(const string &filename) {
        vector<vector<bool>> adj_matrix = ReadData(filename);
        vector<int> matching = FindMatching(adj_matrix);
        WriteResponse(matching);
    }

private:
     vector<vector<bool>> ReadData(const string &filename) {
         std::ifstream fs(filename);

        int num_flights, num_crews;
        fs >> num_flights >> num_crews;
        vector<vector<bool>> adj_matrix(num_flights, vector<bool>(num_crews));
        for (int i = 0; i < num_flights; ++i) {
            for (int j = 0; j < num_crews; ++j) {
                int bit;
                fs >> bit;
                adj_matrix[i][j] = (bit == 1);
            }
        }

        return adj_matrix;
    }

    vector<int> FindMatching(const vector<vector<bool>>& adj_matrix) {
        int num_flights = adj_matrix.size();
        int num_crews = adj_matrix[0].size();

        vector<vector<int>> C = convertMatrix(adj_matrix);
        FlowGraph graph;
        graph.read_data(C);
        vector<vector<int>> F = graph.max_flow(0, graph.size() - 1);

        return parseMatrix(F, num_flights, num_crews);
    }

    void WriteResponse(const vector<int>& matching) {
        for (int i = 0; i < matching.size(); ++i) {
            if (i > 0) {
                cout << " ";
            }

            cout << (matching[i]);            
        }
        cout << "\n";
    }

    vector<vector<int>> convertMatrix(const vector<vector<bool>> &adj_matrix) {
        int num_flights = adj_matrix.size();
        int num_crews = adj_matrix[0].size();
        int nodes = num_flights + num_crews + 2;

        // node 0 is source, node nodes is sink.
        vector<vector<int>> C(nodes, vector<int>(nodes));

        // Connect source to all nodes on the left.
        for (int i = 1; i < num_crews + 1; ++i) {
            C[0][i] = 1;
        }

        // Connect all nodes on the right to sink.
        for (int i = num_crews + 1; i < nodes - 1; ++i) {
            C[i][nodes - 1] = 1;
        }

        for (int i = 0; i < num_flights; ++i) {
            for (int j = 0; j < num_crews; ++j) {
                if (adj_matrix[i][j]) {
                    C[j + 1][num_crews + i + 1] = 1;
                }
            }
        }

        return C;
    }

    vector<int> parseMatrix(vector<vector<int>> F, int num_flights, int num_crews) {
        vector<int> matching(num_flights, -1);
        for (int i = 1; i < num_crews + 1; ++i) {
            if (F[0][i] == 1) {
                int pos = std::find(F[i].begin(), F[i].end(), 1) - F[i].begin();
                if (pos < F[i].size()) {
                    matching[pos - num_crews - 1] = i;
                }
            }
        }

        return matching;
    }
};

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Please specify a file name for test." << endl;
    } else {
        std::ios_base::sync_with_stdio(false);
        string filename(argv[1]);
        MaxMatching max_matching;
        max_matching.Solve(filename);
        return 0;
    }
}
