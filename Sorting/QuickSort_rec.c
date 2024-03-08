#include <stdio.h>
#include <stdlib.h>

int Partition(int *digits, int p, int r);
void Qsort(int *digits, int p, int r);

int main() {
    int n, num;
    printf("Enter the number of elements:");
    scanf("%d", &n);
    int digits[n];

    printf("Enter %d elements:", n);
    for (int j = 0; j < n; j++) {
        scanf("%d", &num);
        digits[j] = num;
    }

    Qsort(digits, 0, n - 1);

    printf("\nSorted array: ");
    for (int k = 0; k < n; k++) {
        printf("%d ", digits[k]);
    }

    return 0;
}

int Partition(int *digits, int p, int r) {
    int x = digits[r];
    int i = p, j = r;
    int t;
    while (1) {
        while (digits[i] < x) i++;
        while (digits[j] > x) j--;
        if (i <= j) {
            t = digits[i];
            digits[i] = digits[j];
            digits[j] = t;
            i++;
            j--;
        } else
            return j;
    }
}

void Qsort(int *digits, int p, int r) {
    if (p < r) {
        int q = Partition(digits, p, r);
        Qsort(digits, p, q);
        Qsort(digits, q + 1, r);
    }
}
