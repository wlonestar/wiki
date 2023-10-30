#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>

int partition(int *a, int lo, int hi) {
  int pivot = a[lo];
  while (lo < hi) {
    while (lo < hi && a[hi] >= pivot) hi--;
    a[lo] = a[hi];
    while (lo < hi && a[lo] <= pivot) lo++;
    a[hi] = a[lo];
  }
  a[lo] = pivot;
  return lo;
}
void _quick_sort(int *a, int lo, int hi) {
  if (lo < hi) {
    int pivot = partition(a, lo, hi);
    _quick_sort(a, lo, pivot - 1);
    _quick_sort(a, pivot + 1, hi);
  }
}
void quick_sort(int *a, int n) {
  _quick_sort(a, 0, n - 1);
}

int main(int argc, char *argv[]) {
  assert(argc == 2);
  srand(time(NULL));
  int n = atoi(argv[1]);
  int *a = (int *) malloc(sizeof(int) * n);
  for (int i = 0; i < n; i++) a[i] = rand() % (n * n);
  quick_sort(a, n);
  for (int i = 0; i < n; i++) printf("%d ", a[i]);
  return 0;
}
