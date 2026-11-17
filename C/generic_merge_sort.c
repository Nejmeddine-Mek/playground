#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef int (*compar)(const void *, const void*);
void merge(void *arr1, void *arr2, size_t NumOfElements1, size_t NumOfElements2, size_t SizeOfElements, void *scratch, compar compar_fn){
    char *a1 = (char *)arr1;
    char *a2 = (char *)arr2;
    char *s = (char *)scratch;
    size_t k = 0, j = 0;
    for(size_t i = 0; i < (NumOfElements1 + NumOfElements2); ++i){
        if(k < NumOfElements1 && j < NumOfElements2){
            int compar_res = compar_fn(a1 + k * SizeOfElements, a2 + j * SizeOfElements);
            if(compar_res > 0){
                memcpy(s + i * SizeOfElements, a2 + j * SizeOfElements, SizeOfElements);
                j++;
            } else{
                memcpy(s + i * SizeOfElements, a1 + k * SizeOfElements, SizeOfElements);
                k++;
            }
        } else if(k < NumOfElements1){
            memcpy(s + i * SizeOfElements, a1 + k * SizeOfElements, SizeOfElements);
            k++;
        } else{
            memcpy(s + i * SizeOfElements, a2 + j * SizeOfElements, SizeOfElements);
            j++;
        }
    }
}
void MergeSortHelper(void *base, size_t NumOfElements,size_t SizeOfElements, void *scratch, compar compar_fn){
    if (NumOfElements <= 1) return;

    size_t mid = NumOfElements / 2;
    size_t right_size = NumOfElements - mid;
    MergeSortHelper(base , mid, SizeOfElements, scratch, compar_fn);
    MergeSortHelper(base + mid*SizeOfElements, right_size, SizeOfElements, scratch, compar_fn);
    merge(base, base + mid * SizeOfElements, mid, right_size, SizeOfElements, scratch, compar_fn);

    memcpy(base, scratch, NumOfElements * SizeOfElements);
    
}
/**
 * size is the size of the type 1, 4, 8 ...
 * while the length is the length of the array, so the actual size of the array in bytes is size * length
 */
void msort(void *base, size_t NumOfElements, size_t SizeOfElements, compar compar_fn){
    if(NumOfElements <= 1) return;
    /**
     * we keep it simple allocation wise, we only allocate once here
     */
    void *temp = malloc(NumOfElements * SizeOfElements);
    if (!temp) return; // Allocation guard

    MergeSortHelper(base, NumOfElements, SizeOfElements,temp, compar_fn);

    free(temp);
}

// --- COMPARATORS & TEST DATA ---

// 1. Integer Comparator
int compare_ints(const void *a, const void *b) {
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    return (int_a > int_b) - (int_a < int_b);
}

// 2. Custom Struct Definition & Comparator
typedef struct {
    int id;
    float gpa;
} Student;

int compare_students(const void *a, const void *b) {
    const Student *s1 = (const Student *)a;
    const Student *s2 = (const Student *)b;
    if (s1->gpa < s2->gpa) return -1;
    if (s1->gpa > s2->gpa) return 1;
    return 0;
}

int main(int argc, [[maybe_unused]] char **argv) {
    // --- Test 1: Sorting Integers ---
    int numbers[] = {42, 7, 19, 3, 85, 12, 19};
    size_t num_count = sizeof(numbers) / sizeof(numbers[0]);

    printf("Unsorted integers: ");
    for (size_t i = 0; i < num_count; i++) printf("%d ", numbers[i]);
    printf("\n");

    msort(numbers, num_count, sizeof(int), compare_ints);

    printf("Sorted integers:   ");
    for (size_t i = 0; i < num_count; i++) printf("%d ", numbers[i]);
    printf("\n\n");

    // --- Test 2: Sorting Custom Structs (by GPA) ---
    Student students[] = {
        {101, 3.2f},
        {102, 3.9f},
        {103, 2.7f},
        {104, 3.5f}
    };
    size_t student_count = sizeof(students) / sizeof(students[0]);

    printf("Unsorted students (ID, GPA):\n");
    for (size_t i = 0; i < student_count; i++) {
        printf("  ID: %d, GPA: %.1f\n", students[i].id, students[i].gpa);
    }

    msort(students, student_count, sizeof(Student), compare_students);

    printf("Sorted students by GPA:\n");
    for (size_t i = 0; i < student_count; i++) {
        printf("  ID: %d, GPA: %.1f\n", students[i].id, students[i].gpa);
    }

    return 0;
}
