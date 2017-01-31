#include "../include/debug.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// protected function, that only this .c can use
int comparator_func(const void *a, const void *b) {
    return *(uint8_t *)a - *(uint8_t *)b;
}

bool terrible_sort(uint16_t *data_array, const size_t value_count) {
    //check params
    if (!data_array || value_count < 1) {
        return false;
    }

    uint16_t* sorting_array = malloc(value_count * sizeof(uint16_t));

    for (unsigned int i = 0; i < value_count; ++i) {
        sorting_array[i] = data_array[i];
    }

    qsort(sorting_array, value_count, sizeof(uint16_t), comparator_func);

    bool sorted = true;
    for (unsigned int i = 1; i < value_count - 1; ++i) {
        sorted &= sorting_array[i - 1] <= sorting_array[i];
    }

    if (sorted) {
        memcpy(data_array, sorting_array, value_count * sizeof(uint16_t));
    }

    free(sorting_array);
    return sorted;
}
