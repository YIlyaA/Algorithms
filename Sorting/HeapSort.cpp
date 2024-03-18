#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <random>
using namespace std;

void heapify(vector<int> &data, int n, int i)
{
    int largest = i;
    //znajdowanie dzieci, +1 bo indeksowane od 0
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    //zamień indeksy jeśli trzeba
    if (left < n && data[largest] > data[left])
    {
        largest = left;
    }

    if (right < n && data[largest] > data[right])
    {
        largest = right;
    }

    //zamień wartości jeśli zamieniono wartości
    if (largest != i)
    {
        swap(data[i], data[largest]);
        //zmieniono wartość, zatem trzeba się upewnić, że rekurencyjnie kolejne poddrzewo nadal ma postać kopca
        heapify(data, n, largest);
    }
}

void sort(vector<int> &data, int n)
{
    //układanie danych w kopiec począwszy od ostatniego rodzica
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        //tablica, długość tablicy, indeks kolejnego rodzica od końca
        heapify(data, n, i);
    }

    //teraz przekształcić heap w malejącą tablicę

    //dla każdego elementu
    for (int i = n - 1; i > 0; i--)
    {
        //przesuń pierwszy element na koniec, potem skopcuj resztę tablicy
        swap(data[0], data[i]);
        //tablica, granica części nieposortowanej, kopcowanie od zerowego indeksu
        heapify(data, i, 0);
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
    sort(data, n);

    cout << "Dane po sortowaniu:";
    for (int i = 0; i < n; i++) {
        cout << data[i] << " ";
    }
    cout << endl;

    return 0;
}