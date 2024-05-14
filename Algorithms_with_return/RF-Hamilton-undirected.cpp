#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int MAX_VERTICES = 10;

/*******************
  UNDIRECTED GRAPH
*******************/
class HamiltonianCycleFinder {
private:
    int vertices;
    int graph[MAX_VERTICES][MAX_VERTICES];
    vector<int> path;

public:
    HamiltonianCycleFinder(int v) {
        vertices = v;
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                graph[i][j] = 0;
            }
        }
    }

    void addEdge(int u, int v) {
        graph[u][v] = 1;
        graph[v][u] = 1;
    }

    void printGraph() {
        cout << "Adjacency matrix:" << endl;
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                cout << graph[i][j] << " ";
            }
            cout << endl;
        }
    }

    bool isSafe(int v, int pos) {
        if (graph[path[pos - 1]][v] == 0)
            return false;
        for (int i = 0; i < pos; ++i)
            if (path[i] == v)
                return false;
        return true;
    }

    bool hamiltonianCycleUtil(int pos) {
        if (pos == vertices) {
            if (graph[path[pos - 1]][path[0]] == 1)
                return true;
            else
                return false;
        }
        for (int v = 1; v < vertices; ++v) {
            if (isSafe(v, pos)) {
                path[pos] = v;
                if (hamiltonianCycleUtil(pos + 1))
                    return true;
                path[pos] = -1;
            }
        }
        return false;
    }

    bool hamiltonianCycle() {
        path.resize(vertices, -1);
        path[0] = 0;
        if (!hamiltonianCycleUtil(1)) {
            cout << "There is no Hamiltonian cycle in this graph." << endl;
            return false;
        }
        cout << "Found Hamiltonian cycle: ";
        for (int i = 0; i < vertices; ++i)
            cout << path[i] + 1 << " ";
        cout << path[0] + 1 << endl;
        return true;
    }
};

int main() {
    int choice;
    cout << "Choose data input method:" << endl;
    cout << "1. Keyboard input" << endl;
    cout << "2. File input" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        int vertices, edges;
        cout << "Enter the number of vertices and edges: ";
        cin >> vertices >> edges;
        HamiltonianCycleFinder hcFinder(vertices);
        cout << "Enter pairs of vertices connected by an edge:" << endl;
        for (int i = 0; i < edges; ++i) {
            int u, v;
            cin >> u >> v;
            hcFinder.addEdge(u - 1 , v - 1);
        }
        hcFinder.printGraph();
        hcFinder.hamiltonianCycle();
    } else if (choice == 2) {
        string filename = "D:/PROGRAMMING/C/Algorithms_with_return/input.txt";
        ifstream inputFile(filename);
        if (!inputFile) {
            cerr << "Error opening file." << endl;
            return 1;
        }
        int vertices, edges;
        inputFile >> vertices >> edges;
        HamiltonianCycleFinder hcFinder(vertices);
        for (int i = 0; i < edges; ++i) {
            int u, v;
            inputFile >> u >> v;
            hcFinder.addEdge(u - 1 , v - 1);
        }
        inputFile.close();
        hcFinder.printGraph();
        hcFinder.hamiltonianCycle();
    } else {
        cout << "Invalid choice. Please enter 1 or 2." << endl;
        return 1;
    }

    return 0;
}
