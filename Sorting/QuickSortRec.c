#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int Partition(int *digits, int p, int r);

void Qsort(int *digits, int p, int r);

int main() {
    int n, num;
    scanf("%d", &n);
    int digits[n];

    // generator losowych liczb
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        num = rand() % 100;
        printf("%d ", num);
        digits[i] = num;
    }
    printf("\n");


//    for (int j = 0; j < n; j++) {
//        scanf("%d", &num);
//        digits[j] = num;
//    }

    Qsort(digits, 0, n - 1);

    for (int k = n - 1; k >= 0; k--) {
        printf("%d ", digits[k]);
    }

    return 0;
}

// Funkcja do podziału tablicy na dwie części wokół elementu pivotu
int Partition(int *digits, int p, int r) {
    int x = digits[r]; // Wybieramy ostatni element jako pivot
    int i = p, j = r;
    int t;
    while (1) {
        // Znajdujemy elementy mniejsze i większe od pivotu
        while (digits[i] < x) i++;
        while (digits[j] > x) j--;
        if (i <= j) {
            // Zamieniamy miejscami elementy mniejsze i większe od pivotu
            t = digits[i];
            digits[i] = digits[j];
            digits[j] = t;
            i++;
            j--;
        } else
            return j; // Zwracamy indeks podziału
    }
}

// Funkcja sortująca tablicę za pomocą sortowania szybkiego
void Qsort(int *digits, int p, int r) {
    if (p < r) {
        int q = Partition(digits, p, r);  // Znajdujemy indeks podziału
        Qsort(digits, p, q);    // Sortujemy lewą część tablicy
        Qsort(digits, q + 1, r); // Sortujemy prawą część tablicy
    }
}
