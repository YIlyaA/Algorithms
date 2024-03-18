#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <random>
using namespace std;

void ShellSort(vector<int> &data, int n)
{
    // kolejne przerwy zgodnie z ciągiem Hibbarda, 2^iteracja + 1

    for (int gap = pow(2, floor(log2(n/2))) - 1; gap > 0; gap = (gap - 1) / 2)
    {
        cout << "Gap: ";
        cout << gap;
        cout << endl;
        // dla kolejnych przerw, wykonaj insertion sort
        // insertion sort biorący gap jako krok zamieniania wartości, jeśli są mniejsze od klucza (sortowanie malejące)
        // kolejne liczby, od których rozpoczyna się sortowania i idzie w lewo
        for (int i = gap; i < n; i++)
        {
            int key = data[i];
            int j = i;
            //sam mechanizm zamiany
            while (j >= gap && data[j - gap] < key)
            {
                data[j] = data[j - gap];
                j -= gap;
            }
            data[j] = key;
            /*for (int i = 0; i < n; i++) {
                cout << data[i] << " ";
            }
            cout << endl;*/
        }
    }
}

int main()
{
    vector<int> data;
    string input;
    string toggle;

    cout << "Dane z klawiatury? T/N:";
    cin >> toggle;
    cin.ignore();
    if (toggle == "T")
    {
        cout << "Podaj liczby (spacje), puste zaby zakonczyc:";
        cout << endl;
        while (getline(cin, input))
        {
            if (input == "") break;
            int number;
            stringstream ss(input);
            while (ss >> number) {
                if (data.size() < 10) data.push_back(number);
            }
            cout << endl;
        }
    }
    else
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> distribution(1, 1000);
        const int numberOfRandomNumbers = 20;
        for (int i = 0; i < numberOfRandomNumbers; ++i) {
            data.push_back(distribution(gen));
        }
    }

    int n = data.size();
    cout << "Dane przed sortowaniem:";
    for (int i = 0; i < n; i++) {
        cout << data[i] << " ";
    }
    cout << endl;
    ShellSort(data, n);

    cout << "Dane po sortowaniu:";
    for (int i = 0; i < n; i++) {
        cout << data[i] << " ";
    }
    cout << endl;

    return 0;
}