#include<stdio.h>

void BubbleSort(int n, int *digits) {
    for (int j = 1; j <= n - 1; j++) {
        for (int i = 0; i < n - j; i++){
            if (digits[i] > digits[i+1]) {
                int t = digits[i];
                digits[i] = digits[i+1];
                digits[i+1] = t;
            }
        }
    }
}

int main() {
    int n, num;
    scanf("%d", &n);
    int digits[n];
    for (int j=0; j<n; j++) {
        scanf("%d", &num);
        digits[j] = num;
    }

    BubbleSort(n, digits);


    for (int k=0; k<n; k++) {
        printf("%d ", digits[k]);
    }
    return 0;
}
