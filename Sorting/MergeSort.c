#include<stdio.h>

int merge_count = 0;

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int part1 = m - l + 1;
    int part2 = r - m;
    merge_count++;
    int L[part1], R[part2];

    for (i = 0; i < part1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < part2; j++)
        R[j] = arr[m + 1 + j];

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

    while (i < part1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < part2) {
        arr[k] = R[j];
        j++;
        k++;
    }

}

void MergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        MergeSort(arr, l, m);
        MergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

int main() {
    int n, num, count=0;
    scanf("%d", &n);
    int digits[n];
    for (int j = 0; j < n; j++) {
        scanf("%d", &num);
        digits[j] = num;
    }

    MergeSort(digits, 0, n - 1);
    printf("Liczba scalen: %d\n", merge_count);

    for (int k = 0; k < n; k++) {
        printf("%d ", digits[k]);
    }
    return 0;
}
