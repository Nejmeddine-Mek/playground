#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void merge(int *arr1, int *arr2, size_t s1, size_t s2, int *out){
    
    size_t j = 0, k = 0;
    for(size_t i = 0; i < s1 + s2; ++i){
        if(j < s1 && k < s2){
            out[i] = arr1[j] <= arr2[k] ? arr1[j++]:arr2[k++];
        } else if(j < s1){
            out[i] = arr1[j++];
        }else{
            out[i] = arr2[k++];
        }
    }

}

void MergeSortHelper(int *data, size_t size, int *out){

    if (size <= 1) return;

    size_t mid = size / 2;
    size_t right_size = size - mid;

    MergeSortHelper(data, mid, out);
    MergeSortHelper(data + mid, right_size, out);

    merge(data, data + mid, mid, right_size, out);

    memcpy(data, out, size * sizeof(int));

}

void MergeSort(int *data, size_t size){
    if (size <= 1) return;
    /**
     * We created this wrapper function to create a scratch buffer "temp" for the merge function in order to minimize
     * the dynamic memory allocations in the process
     * while keeping alloc in MergeSortHelper or merge is still okay, this reduces the number of malloc and free calls
     */
    int *temp = (int *)malloc(size * sizeof(int));
    if (!temp) return; // Allocation guard

    MergeSortHelper(data, size, temp);

    free(temp);
}

int main(int argc,[[maybe_unused]] char **argv){

    size_t size = 10;
    int *data = (int *)malloc(10* sizeof(int));
    int init[] = {42, -7, 19, 42, 0, 88, 3, -15, 27, 11};
    memcpy(data, init, size * sizeof(int));
    MergeSort(data, size);
    printf("Data: ");
    for(size_t i = 0; i < size; ++i){
        printf("%d ", data[i]);
    }
    printf("\n");
    free(data);
    return 0;
}