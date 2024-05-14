#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>

using namespace std;

/*******************
  DIRECTED GRAPH
*******************/
bool hamiltionian(int vertex, vector<vector<int>>& graphMatrix, vector<int>& cycle, vector<bool>& visited, int n, int& vis, int start, int& k)
{
    visited[vertex - 1] = true;  //dodajemy nowy wierzchołek
    vis++;
    cycle.push_back(vertex);

    for (int i = 0; i < n; i++)
    {
        if (graphMatrix[vertex - 1][i] >= 0 && graphMatrix[vertex - 1][i] <= n)
        {
            if (i + 1 == start && vis == n)
            {
                cycle.push_back(i+1);
                return true;
            }
            if (!visited[i])
            {
                if (hamiltionian(i + 1, graphMatrix, cycle, visited, n, vis, start, k))
                {
                    return true;
                }
            }
        }
    }
    visited[vertex-1] = false;
    vis--;
    cycle.pop_back();
    return false;
}


bool hcycle(vector<vector<int>>& graphMatrix, vector<int>& cycle, vector<bool>& visited, int n)
{
    int k = 1;
    int vis = 0;
    int start = 1;
    return hamiltionian(start, graphMatrix, cycle, visited, n, vis, start, k);
}


unordered_map<int, vector<int>> generateSuccessorsList(const vector<pair<int, int>>& edges) {
    unordered_map<int, vector<int>> successors;
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        successors[u].push_back(v);
    }
    for (auto& pair : successors) {
        sort(pair.second.begin(), pair.second.end());
    }
    for (auto& pair : successors) {
        sort(pair.second.begin(), pair.second.end());
    }

    // Сортировка по ключам
    vector<int> sorted_keys;
    for (const auto& pair : successors) {
        sorted_keys.push_back(pair.first);
    }
    sort(sorted_keys.begin(), sorted_keys.end());

    // Создание отсортированного списка предшественников
    unordered_map<int, vector<int>> sorted_successors;
    for (int key : sorted_keys) {
        sorted_successors[key] = successors[key];
    }

    return sorted_successors;
}

unordered_map<int, vector<int>> generatePredecessorsList(const vector<pair<int, int>>& edges) {
    unordered_map<int, vector<int>> predecessors;
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        predecessors[v].push_back(u);
    }
    for (auto& pair : predecessors) {
        sort(pair.second.begin(), pair.second.end());
    }

    // Сортировка по ключам
    vector<int> sorted_keys;
    for (const auto& pair : predecessors) {
        sorted_keys.push_back(pair.first);
    }
    sort(sorted_keys.begin(), sorted_keys.end());

    // Создание отсортированного списка предшественников
    unordered_map<int, vector<int>> sorted_predecessors;
    for (int key : sorted_keys) {
        sorted_predecessors[key] = predecessors[key];
    }

    return sorted_predecessors;
}

unordered_map<int, vector<int>> generateIncidenceList(int vertices, const unordered_map<int, vector<int>>& predecessors, const unordered_map<int, vector<int>>& successors) {
    unordered_map<int, vector<int>> incidence;
    unordered_map<int, vector<int>> none_incidence;

    vector<int> referenceVector;
    for (int i = 1; i <= vertices; ++i) {
        referenceVector.push_back(i);
    }

    // Обходим каждую вершину
    for (const auto& pair : predecessors) {
        int vertex = pair.first;
        const vector<int>& pred = pair.second;
        vector<int> ttt;

        // Добавляем предшественников
        incidence[vertex] = pred;

        // Добавляем последователей
        for (int succ : successors.at(vertex)) {
            incidence[vertex].push_back(succ);
        }

        // Сортируем список инцидентности для данной вершины
        sort(incidence[vertex].begin(), incidence[vertex].end());


        for (const auto& pair : incidence) {
            vector<int> missingElements;
            set_difference(
                    referenceVector.begin(), referenceVector.end(),
                    pair.second.begin(), pair.second.end(),
                    back_inserter(missingElements)
            );

            if (!missingElements.empty()) {
                none_incidence[pair.first] = missingElements;
            }
        }
    }

    for (auto& pair : none_incidence) {
        sort(pair.second.begin(), pair.second.end());
    }

    // Сортировка по ключам
    vector<int> sorted_keys;
    for (const auto& pair : none_incidence) {
        sorted_keys.push_back(pair.first);
    }
    sort(sorted_keys.begin(), sorted_keys.end());

    // Создание отсортированного списка предшественников
    unordered_map<int, vector<int>> sorted_none_incidence;
    for (int key : sorted_keys) {
        sorted_none_incidence[key] = none_incidence[key];
    }

    return sorted_none_incidence;
}


vector<vector<int>> fillMatrix(unordered_map<int, vector<int>>& successors,
                               unordered_map<int, vector<int>>& predecessors,
                               unordered_map<int, vector<int>>& incidences,
                               int vertices) {

    // Initialize matrix
    vector<vector<int>> matrix(vertices, vector<int>(vertices + 3, 0));

    // Fill the rightmost columns based on Successors list
    for (auto& pair : successors) {
        int vertex = pair.first;
        if (!pair.second.empty()) {
            matrix[vertex - 1][vertices] = pair.second[0];
        }
    }

    // Fill the rightmost columns based on Predecessors list
    for (auto& pair : predecessors) {
        int vertex = pair.first;
        if (!pair.second.empty()) {
            matrix[vertex - 1][vertices + 1] = pair.second[0];
        }
    }

    // Fill the rightmost columns based on None Incidences list
    for (auto& pair : incidences) {
        int vertex = pair.first;
        if (!pair.second.empty()) {
            matrix[vertex - 1][vertices + 2] = pair.second[0];
        }
    }

    // Fill the matrix based on Successors list
    for (auto& pair : successors) {
        int vertex = pair.first;
        int row = vertex - 1;
        int counter = 0;
        for (int i = 0; i < pair.second.size(); ++i) {
            int col = pair.second[i] - 1;
            if (i + 1 >= pair.second.size()) matrix[row][col] = pair.second.back();  //если индекс элемента >= кол-ву элементов, то вставляем посдедний элемент списка
            else matrix[row][col] = pair.second[counter + 1];    //вставляем 2 элемент второй части списка, т к первый находится в 3 колонке с конца
            counter += 1;
        }
    }

    // Fill the matrix based on Predecessors list
    for (auto& pair : predecessors) {
        int vertex = pair.first;
        int row = vertex - 1;
        int counter = 0;
        for (int i = 0; i < pair.second.size(); ++i) {
            int col = pair.second[i] - 1;
            if (i + 1 >= pair.second.size()) matrix[row][col] = pair.second.back() + vertices;
            else matrix[row][col] = pair.second[counter + 1] + vertices;
            counter += 1;
        }
    }

    // Fill the matrix based on None Incidences list
    for (auto& pair : incidences) {
        int vertex = pair.first;
        int row = vertex - 1;
        int counter = 0;
        for (int i = 0; i < vertices; ++i) {
            if (matrix[row][i] == 0) {
                if (i + 1 >= pair.second.size()) {
                    matrix[row][i] = -(pair.second.back());
                }
                else matrix[row][i] = -(pair.second[counter + 1]); // Dodajemy ostatni z minusem z None Incidences list
                counter += 1;
            }
        }
    }


    // Output the matrix

    return matrix;
}



int main() {
    int vertices, edges;
    vector<pair<int, int>> edgesList;

    int choice;
    cout << "Choose data input method:" << endl;
    cout << "1. Keyboard input" << endl;
    cout << "2. File input" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        cout << "Enter the number of vertices and edges: ";
        cin >> vertices >> edges;
        cout << "Enter pairs of vertices connected by an edge:" << endl;
        for (int i = 0; i < edges; ++i) {
            int u, v;
            cin >> u >> v;
            edgesList.push_back({ u, v });
        }
    }
    else if (choice == 2) {
        string filename = "D:/PROGRAMMING/C/Algorithms_with_return/input.txt";
        ifstream inputFile(filename);
        if (!inputFile) {
            cerr << "Error opening file." << endl;
            return 1;
        }
        inputFile >> vertices >> edges;
        for (int i = 0; i < edges; ++i) {
            int u, v;
            inputFile >> u >> v;
            edgesList.push_back({ u, v });
        }
    }
    else {
        cout << "Input error";
        return 0;
    }


    unordered_map<int, vector<int>> successors = generateSuccessorsList(edgesList);
    cout << "Successors list:" << endl;
    for (int i = 1; i <= vertices; i++) {
        cout << i << ": ";
        for (int successor : successors[i]) {
            cout << successor << " ";
        }
        cout << endl;
    }

    unordered_map<int, vector<int>> predecessors = generatePredecessorsList(edgesList);
    cout << endl << "Predecessors list:" << endl;
    for (int i = 1; i <= vertices; i++) {
        cout << i << ": ";
        for (int predecessor : predecessors[i]) {
            cout << predecessor << " ";
        }
        cout << endl;
    }

    unordered_map<int, vector<int>> incidences = generateIncidenceList(vertices, predecessors, successors);
    cout << endl << "None Incidences list:" << endl;
    for (int i = 1; i <= vertices; i++) {
        cout << i << ": ";
        for (int incidence : incidences[i]) {
            cout << incidence << " ";
        }
        cout << endl;
    }

    cout << endl;
    vector<vector<int>> graphMatrix;
    graphMatrix = fillMatrix(successors, predecessors, incidences, vertices);
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices + 3; ++j) {
            cout << graphMatrix[i][j] << " ";
        }
        cout << endl;
    }

    vector<int> cycle;
    vector<bool> visited(vertices, false); // Вектор для отслеживания посещенных вершин

    if (hcycle(graphMatrix, cycle, visited, vertices)) {
        cout << "Hamiltonian cycle found: ";
        for (int vertex : cycle) {
            cout << vertex << " ";
        }
        cout << endl;
    }
    else {
        cout << "No Hamiltonian cycle found in the graph." << endl;
    }

    return 0;
}