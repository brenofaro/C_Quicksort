#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define THREAD_THRESHOLD 100000 // Tamanho mínimo do subarray para criar novas threads

typedef struct {
    int low;
    int high;
    int* arr;
} QuickSortArgs;

void swap(int* a, int* b) {
   int t = *a;
   *a = *b;
   *b = t;
}

int partition (int arr[], int low, int high) {
   int pivot = arr[high];
   int i = (low - 1);
   for (int j = low; j <= high - 1; j++) {
      if (arr[j] <= pivot) {
         i++;
         swap(&arr[i], &arr[j]);
      }
   }
   swap(&arr[i + 1], &arr[high]);
   return (i + 1);
}

void* quickSort(void* args) {
    QuickSortArgs* qsArgs = (QuickSortArgs*) args;
    int low = qsArgs->low;
    int high = qsArgs->high;
    int* arr = qsArgs->arr;

    if (low < high) {
        int pi = partition(arr, low, high);

        if (high - low > THREAD_THRESHOLD) {
            pthread_t tid1, tid2;
            QuickSortArgs args1 = {low, pi - 1, arr};
            QuickSortArgs args2 = {pi + 1, high, arr};

            pthread_create(&tid1, NULL, quickSort, &args1);
            pthread_create(&tid2, NULL, quickSort, &args2);

            pthread_join(tid1, NULL);
            pthread_join(tid2, NULL);
        } else {
            QuickSortArgs args1 = {low, pi - 1, arr};
            QuickSortArgs args2 = {pi + 1, high, arr};

            quickSort(&args1);
            quickSort(&args2);
        }
    }

    return NULL;
}

void printArray(int arr[], int size) {
   for (int i = 0; i < size; i++)
      printf("%d ", arr[i]);
   printf("\n");
}

int main() {
   long int n = 100000000;
   int *arr = (int*) malloc(n * sizeof(int));
   srand(0);
   for (long int i = 0; i < n; i++) {
      arr[i] = rand() % n;
   }

   clock_t start, end;
   double cpu_time_used;

   start = clock();

   QuickSortArgs args = {0, n - 1, arr};
   quickSort(&args);

   end = clock();

   cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

   printf("CLOCKS_PER_SEC: %ld \n", CLOCKS_PER_SEC);
   printf("CPU time used is: %f \n", cpu_time_used);

   free(arr);

   return 0;
}
