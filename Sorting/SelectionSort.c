#include<stdio.h>

void SelectionSort(int n, int *digits) {
    int t, min;
    for (int j = 0; j < n - 1; j++) {
        min = j;
        for (int i = j + 1; i < n; i++) {
            if (digits[i] < digits[min])
                min = i;
        }
        t = digits[min];
        digits[min] = digits[j];
        digits[j] = t;
    }
}

int main() {
	int n, num;
	scanf("%d", &n);
	int digits[n];
	for (int i=0; i<n; i++) {
		scanf("%d", &num);
		digits[i] = num;
	}

    SelectionSort(n, digits);

    for (int i=0; i<n; i++) {
        printf("%d ", digits[i]);
    }
	return 0;
}
