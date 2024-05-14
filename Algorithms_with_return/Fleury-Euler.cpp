#include <iostream>
#include <cstdlib>
#include <list>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;


//dfs po nieskierowanym
void dfsUD(int vertex, int** matrix, vector<bool>& visited, int n)
{
    visited[vertex] = true;
    for (int i = 0; i < n; i++)
    {
        if (matrix[vertex][i] == 1 && visited[i] == false) dfsUD(i, matrix, visited, n);
    }
}


//dfs po skierowanym
void dfsD(int vertex, int** graphMatrix, vector<bool>& visited, int n, vector<vector<int>>& cycles)
{
    visited[vertex - 1] = true;
    cout << "dfs: " << vertex << endl;
    //pierwszy nastepnik, w sumie bez sensu troche
    if (graphMatrix[vertex - 1][n] != 0 && visited[graphMatrix[vertex - 1][n]-1] == false)
    {
        dfsD(graphMatrix[vertex-1][n], graphMatrix, visited, n, cycles);
    }
    //petle
    for (int i = 1; i < cycles[vertex - 1].size(); i++)
    {
        if (visited[cycles[vertex - 1][i]-1] == false)
        {
            dfsD(cycles[vertex - 1][i], graphMatrix, visited, n, cycles);
        }
    }
    /*if (graphMatrix[vertex - 1][n + 3] != 0 && visited[graphMatrix[vertex - 1][n + 3] - 1] == false)
    {
        dfsD(graphMatrix[vertex - 1][n + 3], graphMatrix, visited, n, cycles);
    }*/
    //nastepniki z pol macierzy
    for (int i = 0; i < n; i++)
    {
        if (graphMatrix[vertex - 1][i] > 0 && graphMatrix[vertex - 1][i] <= n && visited[graphMatrix[vertex - 1][i] - 1] == false)
        {
            dfsD(graphMatrix[vertex - 1][i], graphMatrix, visited, n, cycles);
        }
    }
}


//resetowanie tablicy odwiedzonych, przydaje sie do robienia dfsa wiele razy
void resetVisited(vector<bool>& visited, int n)
{
    for (int i = 0; i < n; i++) visited[i] = 0;
}


//tworzenie listy nastepnikow z macierzy sasiedztwa
void getSuccessors(int** matrix, vector<vector<int>>& successors, int n)
{
    for (int i = 0; i < n; i++)
    {
        vector<int> vec;
        vec.push_back(i + 1);
        successors.push_back(vec);
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 1)
            {
                successors[i].push_back(j + 1);
            }
        }
    }
}


//tworzenie listy poprzednikow z macierzy sasiedztwa
void getPredecessors(int** matrix, vector<vector<int>>& predecessors, int n)
{
    for (int i = 0; i < n; i++)
    {
        vector<int> vec;
        vec.push_back(i + 1);
        predecessors.push_back(vec);
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == -1)
            {
                predecessors[i].push_back(j + 1);
            }
        }
    }
}


//tworzenie listy nieincydentnych z macierzy sasiedztwa
void getNonIncidents(int** matrix, vector<vector<int>>& nonIncidents, int n)
{
    for (int i = 0; i < n; i++)
    {
        vector<int> vec;
        vec.push_back(i + 1);
        nonIncidents.push_back(vec);
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 0)
            {
                nonIncidents[i].push_back(j + 1);
            }
        }
    }
}


//tworzenie listy petli z macierzy sasiedztwa
void getCycles(int** matrix, vector<vector<int>>& cycles, int n)
{
    for (int i = 0; i < n; i++)
    {
        vector<int> vec;
        vec.push_back(i+1);
        cycles.push_back(vec);
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 2 || matrix[j][i] == 2) cycles[i].push_back(j+1);
        }
    }
}


//dfs na nieskierowanym ze zliczaniem odwiedzonych, do sprawdzania mostow
int dfsUDCount(int vertex, int** matrix, vector<bool>& visited, int n, int excluded1, int excluded2, int& count)
{
    visited[vertex] = true;
    for (int i = 0; i < n; i++)
    {
        if (matrix[vertex][i] == 1 && visited[i] == false)
        {
            if (vertex != excluded1 || i != excluded2)
            {
                count++;
                dfsUDCount(i, matrix, visited, n, excluded1, excluded2, count);
            }
        }
    }
    return count;
}


//dfs na skierowanym ze zliczaniem, ostatecznie nieuzyty (zastapiony dfsFind, bo bylo pewniejsze)
int dfsDCount(int vertex, int** graphMatrix, vector<bool>& visited, int n, int excluded1, int excluded2, int& count, int** deletedMatrix)
{
    visited[vertex - 1] = true;
    cout << "dfs: " << vertex << endl;
    cout << "count: " << count << endl;
    if (graphMatrix[vertex - 1][n] != 0 && visited[graphMatrix[vertex - 1][n] - 1] == false)
    {
        if (vertex != excluded1 || graphMatrix[vertex - 1][n] != excluded2)
        {
            count++;
            if(deletedMatrix[vertex-1][graphMatrix[vertex - 1][n]-1] != 1) dfsDCount(graphMatrix[vertex - 1][n], graphMatrix, visited, n, excluded1, excluded2, count, deletedMatrix);
        }
    }
    if (graphMatrix[vertex - 1][n + 3] != 0 && visited[graphMatrix[vertex - 1][n + 3] - 1] == false)
    {
        if (vertex != excluded1 || graphMatrix[vertex - 1][n + 3] != excluded2)
        {
            count++;
            if (deletedMatrix[vertex - 1][graphMatrix[vertex - 1][n + 3] - 1] != 1) dfsDCount(graphMatrix[vertex - 1][n + 3], graphMatrix, visited, n, excluded1, excluded2, count, deletedMatrix);
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (graphMatrix[vertex - 1][i] > 0 && graphMatrix[vertex - 1][i] <= n && visited[graphMatrix[vertex - 1][i] - 1] == false)
        {
            if (vertex != excluded1 || graphMatrix[vertex - 1][i] != excluded2)
            {
                count++;
                if (deletedMatrix[vertex - 1][graphMatrix[vertex - 1][i] - 1] != 1) dfsDCount(graphMatrix[vertex - 1][i], graphMatrix, visited, n, excluded1, excluded2, count, deletedMatrix);
            }
        }
    }
    return count;
}


//dfs na skierowanym szukajacy danego wierzcholka, do sprawdzania mostow na skierowanym
void dfsDFind(int vertex, int** graphMatrix, vector<bool>& visited, int n, int target, int** deletedMatrix, bool& found, vector<vector<int>>& cycles)
{
    visited[vertex - 1] = true;
    if (vertex == target) found = true;
    cout << "dfs: " << vertex << endl;
    if (graphMatrix[vertex - 1][n] != 0 && visited[graphMatrix[vertex - 1][n] - 1] == false)
    {
        if (deletedMatrix[vertex - 1][graphMatrix[vertex - 1][n] - 1] != 1)
        {
            cout << "dfs next: " << graphMatrix[vertex - 1][n] << endl;
            //if (graphMatrix[vertex - 1][n] == target) return true;
            dfsDFind(graphMatrix[vertex - 1][n], graphMatrix, visited, n, target, deletedMatrix, found, cycles);
        }
    }
    /*if (graphMatrix[vertex - 1][n + 3] != 0 && visited[graphMatrix[vertex - 1][n + 3] - 1] == false)
    {
        if (deletedMatrix[vertex - 1][graphMatrix[vertex - 1][n + 3] - 1] != 1)
        {
            cout << "dfs next: " << graphMatrix[vertex - 1][n+3] << endl;
            //if (graphMatrix[vertex - 1][n + 3] == target) return true;
            dfsDFind(graphMatrix[vertex - 1][n + 3], graphMatrix, visited, n, target, deletedMatrix, found, cycles);
        }
    }*/
    for (int i = 1; i < cycles[vertex - 1].size(); i++)
    {
        if (visited[cycles[vertex - 1][i] - 1] == false && deletedMatrix[vertex - 1][cycles[vertex - 1][i] - 1] != 1)
        {
            dfsDFind(cycles[vertex - 1][i], graphMatrix, visited, n, target, deletedMatrix, found, cycles);
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (graphMatrix[vertex - 1][i] > 0 && graphMatrix[vertex - 1][i] <= n && visited[graphMatrix[vertex - 1][i] - 1] == false)
        {
            if (deletedMatrix[vertex - 1][graphMatrix[vertex - 1][i] - 1] != 1)
            {
                cout << "dfs next: " << graphMatrix[vertex - 1][i] << endl;
                //if (graphMatrix[vertex - 1][i] == target) return true;
                dfsDFind(graphMatrix[vertex - 1][i], graphMatrix, visited, n, target, deletedMatrix, found, cycles);
            }
        }
    }
}


//sprawdzanie, czy wierzcholek jest izolowany w nieskierowanym
bool isIsolatedUD(int vertex, int** matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (matrix[vertex][i] == 1) return false;
    }
    return true;
}


//sprawdzanie, czy jest izolowany na skierowanym
bool isIsolatedD(int vertex, int** graphMatrix, int n)
{
    if (graphMatrix[vertex-1][n] == 0 && graphMatrix[vertex - 1][n+1] == 0 && graphMatrix[vertex - 1][n + 3] == 0) return true;
    return false;
}


//sprawdzanie, czy graf nieskierowany jest spojny (dfs z pierwszego nieizolowanego i sprawdzenie czy odwiedza wszystkie nieizolowane)
bool isConnectedUD(int** matrix, int n, vector<bool>& isolated, vector<bool>& visited)
{
    resetVisited(visited, n);
    for (int i = 0; i < n; i++)
    {
        if (isolated[i] == false)
        {
            dfsUD(i, matrix, visited, n);
            break;
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (visited[i] == 0 && isolated[i] == 0)
        {
            return false;
        }
    }

    return true;
}


//sprawdzenie, czy graf skierowany jest spojny (dfs, dfs na odwroconych lukach, sprawdzenie czy gdzies jest nieodwiedzony w obu i nieizolowany, algorytm z geeks for geeks)
bool isConnectedD(int** graphMatrix, int** graphMatrixInv, int n, vector<bool>& isolated, vector<bool>& visited, vector<bool>& visitedInv, vector<vector<int>>& cycles)
{
    resetVisited(visited, n);
    for (int i = 0; i < n; i++)
    {
        if (isolated[i] == false)
        {
            dfsD(i+1, graphMatrix, visited, n, cycles);
            dfsD(i+1, graphMatrixInv, visitedInv, n, cycles);
            break;
        }
    }

    cout << "Vis1" << endl;
    for (int i = 0; i < visited.size(); ++i) {
        cout << visited[i] << " ";
    }
    cout << endl;

    cout << "Vis2" << endl;
    for (int i = 0; i < visitedInv.size(); ++i) {
        cout << visitedInv[i] << " ";
    }
    cout << endl;

    cout << "Iso" << endl;
    for (int i = 0; i < isolated.size(); ++i) {
        cout << isolated[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < n; i++)
    {
        if (visited[i] == 0 && visitedInv[i] == 0 && isolated[i] == 0)
        {
            return false;
        }
    }

    return true;
}


//wypelnianie macierzy sasiedztwa dla nieskierowanego
void fillMatrixUD(int** matrix, int n, int m, char inp)
{
    if (m > n * (n - 1)) m = n * (n - 1);
    if (inp == 'p') {
        string filename = "input.txt";
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Nie udalo sie otworzyc pliku." << filename << endl;
            exit(0);
        }
        int a, b;
        file >> a >> b;
        cout << a << " " << b;
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            int vert1, vert2;
            if (iss >> vert1 >> vert2) {
                matrix[vert1 - 1][vert2 - 1] = 1;
                matrix[vert2 - 1][vert1 - 1] = 1; //-1
            }
        }
        file.close();
    }
    else {
        cout << "Podaj krawedzie:" << endl;
        for (int i = 0; i < m; i++)
        {
            int vert1, vert2;
            cin >> vert1 >> vert2;
            //if (vert1 == vert2) vert2 = n - 1;
            matrix[vert1 - 1][vert2 - 1] = 1;
            matrix[vert2 - 1][vert1 - 1] = 1; //-1
        }
    }
}


//wypelnianie macierzy sasiedztwa dla skierowanego, 2 oznacza petle
void fillMatrixD(int** matrix, int n, int m, char inp)
{
    if (inp == 'p') {
        string filename = "input.txt";
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Nie udalo sie otworzyc pliku." << filename << endl;
            exit(0);
        }
        int a, b;
        file >> a >> b;
        cout << a << " " << b << endl;
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            int vert1, vert2;
            if (iss >> vert1 >> vert2) {
                if (vert1 == vert2)
                {
                    matrix[vert1 - 1][vert2 - 1] = 2;
                }
                else if (matrix[vert2 - 1][vert1 - 1] == 1)
                {
                    matrix[vert1 - 1][vert2 - 1] = 2;
                    matrix[vert2 - 1][vert1 - 1] = 2;
                }
                else
                {
                    matrix[vert1 - 1][vert2 - 1] = 1;
                    matrix[vert2 - 1][vert1 - 1] = -1; //-1
                }
            }
        }
        file.close();
    }
    else {
        cout << "Podaj krawedzie:" << endl;
        for (int i = 0; i < m; i++)
        {
            int vert1, vert2;
            cin >> vert1 >> vert2;
            if (vert1 == vert2)
            {
                matrix[vert1 - 1][vert2 - 1] = 2;
            }
            else if (matrix[vert2 - 1][vert1 - 1] == 1)
            {
                matrix[vert1 - 1][vert2 - 1] = 2;
                matrix[vert2 - 1][vert1 - 1] = 2;
            }
            else
            {
                matrix[vert1 - 1][vert2 - 1] = 1;
                matrix[vert2 - 1][vert1 - 1] = -1; //-1
            }
        }
    }
}


//sprawdzanie, czy w grafie skierowanym dla kazdego wierzcholka |in| == |out|
bool checkInOutD(int** graphMatrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        int countS = 0;
        int countP = 0;
        for (int j = 0; j < n; j++)
        {
            if (graphMatrix[i][j] >= 0 && graphMatrix[i][j] <= n) countS++;
        }
        for (int j = 0; j < n; j++)
        {
            if (graphMatrix[i][j] >= n+1 && graphMatrix[i][j] <= 2*n) countP++;
        }
        if (countS != countP) return false;
    }
    return true;
}


//sprawdzanie, czy w grafie nieskierowanym kazdy wierzcholek ma parzysty stopien
bool checkEvenUD(int** matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        int count = 0;
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 1) count++;
        }
        if (count % 2 != 0) return false;
    }
    return true;
}


//sprawdzenie mostu w nieskierowanym (dwa dfsy, jeden z wylaczona krawedzia dla ktorej jest sprawdzane, jesli ilosc odwiedzonych w jednym i drugim jest taka sama, to nie jest mostem)
bool checkBridgeUD(int vertex, int** matrix, vector<bool>& visited, int n, int edge)
{
    resetVisited(visited, n);
    int count0 = 0;
    dfsUDCount(vertex, matrix, visited, n, vertex, -1, count0);
    resetVisited(visited, n);
    int count1 = 0;
    dfsUDCount(vertex, matrix, visited, n, vertex, edge, count1);
    //cout << vertex+1 << " " << edge+1 << " " << count0 << " " << count1 << endl;
    if (count0 != count1) return true;
    return false;
}


//sprawdzanie mostu w grafie skierowanym (puszczanie dfsa z konca luku, jesli dfs znajdzie poczatek luku, to nie jest mostem)
bool checkBridgeD(int vertex, int** graphMatrix, vector<bool>& visited, int n, int edge, int** deletedMatrix, vector<vector<int>>& cycles)
{
    /*resetVisited(visited, n);
    int count0 = 0;
    dfsDCount(vertex, graphMatrix, visited, n, vertex, -1, count0, deletedMatrix);

    resetVisited(visited, n);
    int count1 = 0;
    dfsDCount(edge, graphMatrix, visited, n, vertex, -1, count1, deletedMatrix);
    cout << vertex << " " << edge << " counts: " << count0 << " " << count1 << endl;
    if (count0 != count1) return true;
    return false;*/

    resetVisited(visited, n);
    bool found = false;
    dfsDFind(edge, graphMatrix, visited, n, vertex, deletedMatrix, found, cycles);
    cout << "check bridge: " << vertex << " " << edge << endl;
    if (found)
    {
        cout << "not a bridge" << endl;
        return false;
    }
    return true;
}


//sprawdzanie, czy mozna przejsc krawedzia w poszukiwaniu cyklu (mozna, jesli nie jest mostem, lub jesli jest to jedyna pozostala krawedz z wierzcholka)
bool checkValidUD(int vertex, int** matrix, vector<bool>& visited, int n, int edge)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (matrix[vertex][i] == 1) count++;
    }
    if (count == 1)
    {
        return true;
    }

    if (!checkBridgeUD(vertex, matrix, visited, n, edge))
    {
        return true;
    }

    return false;
}


//sprawdzanie, czy mozna przejsc lukiem w poszukiwaniu cyklu (mozna, jesli nie jest mostem, lub jesli jest to jedyny pozostaly luk wychodzacy z wierzcholka)
bool checkValidD(int vertex, int** graphMatrix, vector<bool>& visited, int n, int edge, int** deletedMatrix, vector<vector<int>>& cycles)
{
    cout << "edge validity check: " << vertex << " " << edge << endl;
    int count = 0;
    /*if (graphMatrix[vertex - 1][n + 3] > 0 && deletedMatrix[vertex - 1][graphMatrix[vertex - 1][n + 3] - 1] != 1)
    {
        cout << "Cycle ++" << endl;
        count++;
    }*/
    //(liczenie wychodzacych petli)
    for (int i = 1; i < cycles[vertex - 1].size(); i++)
    {
        if (deletedMatrix[vertex - 1][cycles[vertex - 1][i] - 1] != 1)
        {
            cout << "Cycle ++" << endl;
            count++;
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (graphMatrix[vertex - 1][i] >= 0 && graphMatrix[vertex - 1][i] <= n && deletedMatrix[vertex - 1][i] != 1)
        {
            cout << "Regular ++" << endl;
            count++;
        }
    }
    cout << "successors count: " << count << endl;
    if (count == 1)
    {
        return true;
    }

    if (!checkBridgeD(vertex, graphMatrix, visited, n, edge, deletedMatrix, cycles))
    {
        return true;
    }

    return false;
}


//poszukiwanie cyklu w nieskierowanym, algorym Fleury'ego
void findCycleUD(int vertex, int** matrix, vector<bool>& visited, int n, vector<int>& cycle)
{
    cycle.push_back(vertex);
    for (int i = 0; i < n; i++)
    {
        if (matrix[vertex][i] == 1)
        {
            if (checkValidUD(vertex, matrix, visited, n, i))
            {
                matrix[i][vertex] = 0;
                matrix[vertex][i] = 0;
                //cout << "edge: " << endl;
                //cout << vertex+1 << " " << i+1 << endl;
                findCycleUD(i, matrix, visited, n, cycle);
            }
        }
    }
}


//poszukiwanie cyklu w skierowanym, algorym Fleury'ego,
void findCycleD(int vertex, int** graphMatrix, vector<bool>& visited, int n, vector<int>& cycle, int** deletedMatrix, vector<vector<int>>& cycles)
{
    cycle.push_back(vertex);
    cout << "vertex: " << vertex << endl << endl;
    cout << "Deleted: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << deletedMatrix[i][j] << " ";
        }
        cout << endl;
    }
    //pierwszy nastepnik, w sumie bez sensu
    if (graphMatrix[vertex-1][n] != 0 && deletedMatrix[vertex-1][graphMatrix[vertex-1][n]-1] != 1)
    {
        cout << "List" << endl;
        if (checkValidD(vertex, graphMatrix, visited, n, graphMatrix[vertex-1][n], deletedMatrix, cycles))
        {
            deletedMatrix[vertex-1][graphMatrix[vertex-1][n]-1] = 1;
            cout << "edge (1): " << endl;
            cout << vertex << " " << graphMatrix[vertex-1][n] << endl;
            findCycleD(graphMatrix[vertex - 1][n], graphMatrix, visited, n, cycle, deletedMatrix, cycles);
        }
    }

    /*if (graphMatrix[vertex - 1][n + 3] != 0 && deletedMatrix[vertex - 1][graphMatrix[vertex - 1][n + 3] - 1] != 1)
    {
        cout << "Cycles" << endl;
        if (checkValidD(vertex, graphMatrix, visited, n, graphMatrix[vertex - 1][n + 3], deletedMatrix, cycles))
        {
            deletedMatrix[vertex - 1][graphMatrix[vertex - 1][n + 3] - 1] = 1;
            cout << "edge (1): " << endl;
            cout << vertex << " " << graphMatrix[vertex - 1][n + 3] << endl;
            findCycleD(graphMatrix[vertex - 1][n + 3], graphMatrix, visited, n, cycle, deletedMatrix, cycles);
        }
    }*/
    //petle
    for (int i = 1; i < cycles[vertex - 1].size(); i++)
    {
        cout << "Cycles" << endl;
        if (deletedMatrix[vertex - 1][cycles[vertex - 1][i] - 1] != 1)
        {
            if (checkValidD(vertex, graphMatrix, visited, n, cycles[vertex-1][i], deletedMatrix, cycles))
            {
                deletedMatrix[vertex - 1][cycles[vertex - 1][i] - 1] = 1;
                cout << "edge (cycle): " << endl;
                cout << vertex << " " << cycles[vertex - 1][i] << endl;
                findCycleD(cycles[vertex - 1][i], graphMatrix, visited, n, cycle, deletedMatrix, cycles);
            }
        }
    }
    //kolejne nastepniki
    for (int i = 0; i < n; i++)
    {
        if (graphMatrix[vertex-1][i] >= 0 && graphMatrix[vertex-1][i] <= n && deletedMatrix[vertex-1][graphMatrix[vertex-1][i]-1] != 1)
        {
            cout << "Matrix" << endl;
            if (checkValidD(vertex, graphMatrix, visited, n, graphMatrix[vertex-1][i], deletedMatrix, cycles))
            {
                deletedMatrix[vertex-1][graphMatrix[vertex-1][i]-1] = 1;
                cout << "edge (n): " << endl;
                cout << vertex << " " << graphMatrix[vertex-1][i] << endl;
                findCycleD(graphMatrix[vertex - 1][i], graphMatrix, visited, n, cycle, deletedMatrix, cycles);
            }
        }
    }
}

//odwracanie macierzy, zeby moc obrocic luki w grafie skierowanym
void inverseMatrix(int** matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 1)
            {
                matrix[i][j] = -1;
            }
            if (matrix[i][j] == -1)
            {
                matrix[i][j] = 1;
            }
        }
    }
}


//wypelnianie macierzy grafu, algorytm z ekursow
void fillGraphMatrix(int** graphMatrix, int** matrix, int n, vector<vector<int>>& successors, vector<vector<int>>& predecessors, vector<vector<int>>& nonIncident, vector<vector<int>>& cycles)
{
    //Krok 1 :

    for (int i = 0; i < n; i++)
    {
        if (successors[i].size() > 1)
        {
            graphMatrix[i][n] = successors[i][1];
        }
        else graphMatrix[i][n] = 0;
    }

    int count;
    for (int i = 0; i < n; i++)
    {
        count = 2;
        for (int j = 0; j < n; j++)
        {
            if (successors[i].size() == 1)
            {
                graphMatrix[i][j] = 0;
            }
            else
            {
                if (matrix[i][j] == 1)
                {
                    if (successors[i].size() > count)
                    {
                        graphMatrix[i][j] = successors[i][count];
                        count++;
                    }
                    else
                    {
                        graphMatrix[i][j] = successors[i][successors[i].size() - 1];
                    }
                }
            }
        }
    }

    //Krok 2:

    for (int i = 0; i < n; i++)
    {
        if (predecessors[i].size() > 1)
        {
            graphMatrix[i][n+1] = predecessors[i][1];
        }
        else graphMatrix[i][n+1] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        count = 2;
        for (int j = 0; j < n; j++)
        {
            if (matrix[j][i] == 1)
            {
                if (predecessors[i].size() == 1)
                {
                    graphMatrix[i][j] = 0;
                }
                else
                {
                    if (predecessors[i].size() > count)
                    {
                        graphMatrix[i][j] = predecessors[i][count] + n;
                        count++;
                    }
                    else
                    {
                        graphMatrix[i][j] = predecessors[i][predecessors[i].size() - 1] + n;
                    }
                }
            }
        }
    }

    //Krok 3:

    for (int i = 0; i < n; i++)
    {
        if (nonIncident[i].size() > 1)
        {
            graphMatrix[i][n + 2] = nonIncident[i][1];
        }
        else graphMatrix[i][n + 2] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        count = 2;
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 0)
            {
                if (nonIncident[i].size() == 1)
                {
                    graphMatrix[i][j] = 0;
                }
                else
                {
                    if (nonIncident[i].size() > count)
                    {
                        graphMatrix[i][j] = -nonIncident[i][count];
                        count++;
                    }
                    else
                    {
                        graphMatrix[i][j] = -nonIncident[i][nonIncident[i].size() - 1];
                    }
                }
            }
        }
    }

    //Krok 4:

    for (int i = 0; i < n; i++)
    {
        if (cycles[i].size() > 1)
        {
            graphMatrix[i][n + 3] = cycles[i][1];
        }
        else graphMatrix[i][n + 3] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        count = 2;
        for (int j = 0; j < n; j++)
        {
            if (cycles[i].size() != 1)
            {
                if (matrix[i][j] == 2)
                {
                    if (cycles[i].size() > count)
                    {
                        graphMatrix[i][j] = cycles[i][count] + 2 * n;
                        count++;
                    }
                    else
                    {
                        graphMatrix[i][j] = cycles[i][cycles[i].size() - 1] + 2 * n;
                    }
                }
            }
        }
    }
}


int main()
{
    int n, m;
    char inp;
    char type;
    vector<int> cycle;
    bool isConnected;
    bool allEven;

    cout << "Skierwoany czy nieskierowany? (s/n):";
    cin >> type;

    cout << "Wczytac dane z pliku czy z klawiatury (p/k):";
    cin >> inp;
    if (inp == 'p') {
        string filename = "D:/PROGRAMMING/C/Algorithms_with_return/input.txt";
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Unable to open file " << filename << endl;
            return 1;
        }
        file >> n >> m;
        file.close();
    }
    else if (inp == 'k') {
        cout << "Podaj n i m:";
        cin >> n >> m;
    }
    else {
        cout << "ERROR";
        return 0;
    }
    int** matrix = new int* [n];

    for (int i = 0; i < n; i++)
    {
        matrix[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }

    vector<bool> visited(n, false);
    vector<bool> visitedInv(n, false);
    vector<bool> isolated(n, false);

    vector<vector<int>> successors;
    vector<vector<int>> predecessors;
    vector<vector<int>> nonIncident;
    vector<vector<int>> cycles;

    int** graphMatrix = new int* [n];
    for (int i = 0; i < n; i++)
    {
        graphMatrix[i] = new int[n + 4];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n+4; j++) {
            graphMatrix[i][j] = 0;
        }
    }

    int** graphMatrixInv = new int* [n];
    for (int i = 0; i < n; i++)
    {
        graphMatrixInv[i] = new int[n + 4];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 4; j++) {
            graphMatrixInv[i][j] = 0;
        }
    }

    if (type == 'n') fillMatrixUD(matrix, n, m, inp);
    else fillMatrixD(matrix, n, m, inp);

    int** matrixCopyJustInCase = new int* [n];

    for (int i = 0; i < n; i++)
    {
        matrixCopyJustInCase[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrixCopyJustInCase[i][j] = matrix[i][j];
        }
    }

    int** deletedMatrix = new int* [n];

    for (int i = 0; i < n; i++)
    {
        deletedMatrix[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            deletedMatrix[i][j] = 0;
        }
    }

    cout << "Macierz:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    if (type == 'n')
    {
        for (int i = 0; i < n; i++)
        {
            if (isIsolatedUD(i, matrix, n)) isolated[i] = true;
        }

        isConnected = isConnectedUD(matrix, n, isolated, visited);
        allEven = checkEvenUD(matrix, n);
        cout << isConnected << " " << allEven << endl;

        if (isConnected && allEven)
        {
            int sV;
            for (int i = 0; i < n; i++)
            {
                if (isolated[i] == 0)
                {
                    sV = i;
                    break;
                }
            }
            findCycleUD(sV, matrix, visited, n, cycle);
        }
        else
        {
            cout << "Nie ma cyklu." << endl;
            return 0;
        }

        cout << "Cykl: ";
        for (int i = 0; i < cycle.size(); ++i) {
            cout << cycle[i] << " ";
        }
        cout << endl;
    }
    else
    {
        cout << "Nastepniki:" << endl;
        getSuccessors(matrix, successors, n);
        for (const auto& innerVec : successors) {
            for (int num : innerVec) {
                cout << num << " ";
            }
            cout << endl;
        }
        cout << endl;
        cout << "Poprzedniki:" << endl;
        getPredecessors(matrix, predecessors, n);
        for (const auto& innerVec : predecessors) {
            for (int num : innerVec) {
                cout << num << " ";
            }
            cout << endl;
        }
        cout << endl;
        cout << "Nieincydentne:" << endl;
        getNonIncidents(matrix, nonIncident, n);
        for (const auto& innerVec : nonIncident) {
            for (int num : innerVec) {
                cout << num << " ";
            }
            cout << endl;
        }
        cout << endl;
        cout << "Cykle:" << endl;
        getCycles(matrix, cycles, n);
        for (const auto& innerVec : cycles) {
            for (int num : innerVec) {
                cout << num << " ";
            }
            cout << endl;
        }
        cout << endl;

        fillGraphMatrix(graphMatrix, matrix, n, successors, predecessors, nonIncident, cycles);
        inverseMatrix(matrix, n);
        fillGraphMatrix(graphMatrixInv, matrix, n, successors, predecessors, nonIncident, cycles);
        inverseMatrix(matrix, n);

        cout << "Macierz grafu:" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n+4; j++) {
                cout << graphMatrix[i][j] << "    ";
            }
            cout << endl;
        }
        cout << endl;

        resetVisited(visited, n);
        dfsD(1, graphMatrix, visited, n, cycles);
        for (int i = 0; i < visited.size(); ++i) {
            cout << visited[i] << " ";
        }
        cout << endl;

        for (int i = 0; i < n; i++)
        {
            if (isIsolatedD(i+1, graphMatrix, n)) isolated[i] = true;
        }

        isConnected = isConnectedD(graphMatrix, graphMatrixInv, n, isolated, visited, visitedInv, cycles);
        bool isEqualInOut = checkInOutD(graphMatrix, n);

        cout << isConnected << endl;
        cout << isEqualInOut << endl;

        if (isConnected && isEqualInOut)
        {
            int sV = 0;
            for (int i = 0; i < n; i++)
            {
                if (isolated[i] == 0)
                {
                    sV = i+1;
                    break;
                }
            }
            findCycleD(sV, graphMatrix, visited, n, cycle, deletedMatrix, cycles);

            cout << "Cykl: ";
            for (int i = 0; i < cycle.size(); ++i) {
                cout << cycle[i] << " ";
            }
            cout << endl;
        }
        else
        {
            cout << "Nie ma cyklu." << endl;
        }
    }

    for (int i = 0; i < n; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    for (int i = 0; i < n; ++i) {
        delete[] graphMatrix[i];
    }
    delete[] graphMatrix;

    for (int i = 0; i < n; ++i) {
        delete[] graphMatrixInv[i];
    }
    delete[] graphMatrixInv;

    for (int i = 0; i < n; ++i) {
        delete[] matrixCopyJustInCase[i];
    }
    delete[] matrixCopyJustInCase;

    for (int i = 0; i < n; ++i) {
        delete[] deletedMatrix[i];
    }
    delete[] deletedMatrix;

    return 0;
}