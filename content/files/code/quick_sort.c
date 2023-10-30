#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////

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
// void _quick_sort(int *a, int lo, int hi) {
//   if (lo < hi) {
//     int pivot = partition(a, lo, hi);
//     _quick_sort(a, lo, pivot - 1);
//     _quick_sort(a, pivot + 1, hi);
//   }
// }
// void quick_sort(int *a, int n) {
//   _quick_sort(a, 0, n - 1);
// }

typedef struct Sect { int lo, hi; } Sect;
void quick_sort(int *a, int n) {
  int M = 32, front = 0, rear = 0;
  Sect queue[M], p;
  queue[rear].lo = 0; queue[rear].hi = n - 1;
  rear = (rear + 1) % M;
  while (front != rear) {
    p = queue[front];
    front = (front + 1) % M;
    int pivot = partition(a, p.lo, p.hi);
    queue[rear].lo = p.lo; queue[rear].hi = pivot - 1; rear = (rear + 1) % M;
    queue[rear].lo = pivot + 1; queue[rear].hi = p.hi; rear = (rear + 1) % M;
  }
}

///////////////////////////////////////////////////////////////////////////////

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
