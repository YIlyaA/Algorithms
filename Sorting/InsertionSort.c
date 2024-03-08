#include<stdio.h>

void Insertion(int n, int *digits) {
    int i, key;
    for (int j = 1; j < n; j++) {
        key = digits[j];
        i = j - 1;
        while (i >= 0 && digits[i] > key) {
            digits[i + 1] = digits[i];
            i -= 1;
        }
        digits[i + 1] = key;
    }
}

int main() {
    int n, num;
    scanf("%d", &n);
    int digits[n];
    for (int j = 0; j < n; j++) {
        scanf("%d", &num);
        digits[j] = num;
    }

    Insertion(n, digits);

    for (int k = 0; k < n; k++) {
        printf("%d ", digits[k]);
    }
    return 0;
}
