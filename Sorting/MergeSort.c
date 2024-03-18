#include<stdio.h>
//#include <stdlib.h>    //uncomment for generator
//#include <time.h>

int merge_count = 0; // Inicjalizacja zmiennej globalnej do zliczania liczby scalania

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int part1 = m - l + 1;              // Obliczenie rozmiaru pierwszej części
    int part2 = r - m;                  // Obliczenie rozmiaru drugiej części
    merge_count++;                      // Zwiększenie liczby scalania
    int L[part1], R[part2];             // Inicjalizacja tablic pomocniczych dla dwóch części


    // Przekopiowanie danych do tablic pomocniczych L i R
    for (i = 0; i < part1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < part2; j++)
        R[j] = arr[m + 1 + j];


    // Scalanie tablic pomocniczych L i R z powrotem do tablicy arr
    i = 0;
    j = 0;
    k = l;
    while (i < part1 && j < part2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;

    }

    // Skopiowanie pozostałych elementów z L[], jeśli jakieś zostały
    while (i < part1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Skopiowanie pozostałych elementów z R[], jeśli jakieś zostały
    while (j < part2) {
        arr[k] = R[j];
        j++;
        k++;
    }

}

void MergeSort(int arr[], int l, int r) {   // l - element początkowy, r - pivot (końcowy)
    if (l < r) {
        int m = l + (r - l) / 2;            // element środkowy

        // Wywołanie rekurencyjne dla pierwszej i drugiej części tablicy
        MergeSort(arr, l, m);
        MergeSort(arr, m + 1, r);

        // Scalanie posortowanych części
        merge(arr, l, m, r);
    }
}

int main() {
    int n, num;
    scanf("%d", &n);
    int digits[n];
    
    /* generator losowych liczb
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
    	num = rand() % 100;
        printf("%d ", num); 
        digits[i] = num;
    }
    printf("\n");
    */

    // user input:
    for (int j = 0; j < n; j++) {
        scanf("%d", &num);
        digits[j] = num;
    }

    // Wywołanie sortowania przez scalanie
    MergeSort(digits, 0, n - 1);
    //printf("Liczba scalen: %d\n", merge_count);

    //output:
    for (int k = n-1; k >= 0; k--) {
        printf("%d ", digits[k]);
    }
    return 0;
}
