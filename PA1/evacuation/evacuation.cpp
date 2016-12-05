#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm> // std::min

using std::vector;
using std::string;
using std::cout;
using std::endl;

/* This class implements a bit unusual scheme for storing edges of the graph,
 * in order to retrieve the backward edge for a given edge quickly. */
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
                        // cout << "T[" << u + 1 << "][" << v + 1 << "] = " << T[u][v] << endl;
                        // cout << "P[" << v + 1 << "] = " << P[v] << endl;
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
            cout << v + 1 << " <- ";
            v = P[v];
        }
        cout << v + 1 << endl;
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

        // printMatrix(C);
    }

    int size() const {
        return nodes;
    }

    int max_flow(int from, int to) {
        int flow = 0;
        vector<vector<int>> F(nodes);
        for (int i = 0; i < nodes; ++i) {
            F[i].resize(nodes, 0);
        }

        while (true) {
            vector<int> P(nodes, -1);
            int m = BFS(C, T, from, to, F, P);

            if (m == 0) {
                break;
            } else {
                // printPath(P, from, to);

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

                // cout << "Print matrix F." << endl;
                // printMatrix(F);
                // cout << "Print matrix T." << endl;
                // printMatrix(T);
            }


        }

        return flow;
    }

};


int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Please specify a file name for test." << endl;
    } else {
        std::ios_base::sync_with_stdio(false);
        string filename(argv[1]);
        FlowGraph graph;
        graph.read_data(filename);

        std::cout << graph.max_flow(0, graph.size() - 1) << "\n";
    }

    return 0;
}
