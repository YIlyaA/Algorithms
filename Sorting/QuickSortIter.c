#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];                      // Wybieramy ostatni element jako pivot
    int i = low - 1;                            // Indeks mniejszego elementu

    // Iterujemy przez tablicę od low do high-1
    for (int j = low; j <= high - 1; j++) {
        // Jeśli aktualny element jest mniejszy lub równy pivotowi
        if (arr[j] < pivot) {
            i++;  // Inkrementujemy indeks mniejszego elementu
            swap(&arr[i], &arr[j]); // Zamieniamy arr[i] z arr[j]
        }
    }
    swap(&arr[i + 1], &arr[high]);  // Zamieniamy arr[i+1] z pivotem
    return i + 1;  // Zwracamy indeks, gdzie pivot jest na swoim miejscu
}

void QuickSort(int arr[], int low, int high) {
    int stack[high - low + 1]; // Tworzymy stos do przechowywania granic podtablic
    int top = -1; // Wskaźnik wierzchołka stosu

    // Inicjalizujemy stos granicami początkowymi
    stack[++top] = low;
    stack[++top] = high;

    // Dopóki stos nie jest pusty
    while (top >= 0) {
        high = stack[top--];  // Pobieramy górną granicę
        low = stack[top--];   // Pobieramy dolną granicę

        int pi = partition(arr, low, high);

        // Jeśli istnieją elementy na lewo od pivotu, to dodajemy granice tych elementów do stosu
        if (pi - 1 > low) {
            stack[++top] = low;
            stack[++top] = pi - 1;
        }
        // Jeśli istnieją elementy na prawo od pivotu, to dodajemy granice tych elementów do stosu
        if (pi + 1 < high) {
            stack[++top] = pi + 1;
            stack[++top] = high;
        }
    }
}

int main() {
    int n, num;
    scanf("%d", &n);

    int arr[n];

    // generator losowych liczb
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        num = rand() % 100;
        printf("%d ", num);
        arr[i] = num;
    }
    printf("\n");


//    for (int i = 0; i < n; i++) {
//        scanf("%d", &arr[i]);
//    }

    QuickSort(arr, 0, n - 1);

    for (int i = n-1; i >= 0; i--) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
