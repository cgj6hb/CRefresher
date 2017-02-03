#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/error_handling.h"

int create_blank_records(Record_t **records, const size_t num_records) {
    /* 1000 is an arbitarily-picked number to check if the value of num_records
     * is huge. This is to detect if a negative number was assigned to
     * num_records on the way in. For example, if num_records was initialized to
     * -1, it gets converted to INT_MAX since size_t is an unsigned type.
     */
    if (num_records > 1000) {
        return -2;
    }

    // Check params
    if (*records || !records || num_records == 0) {
        return -1;
    }

    // Allocate memory for as many records as needed
    *records = (Record_t*) malloc(sizeof(Record_t) * num_records);

    // Check that allocation was successful; return -2 if not
    if (!records) {
        return -2;
    }

    // Zero initialize each record, as per the function specifications
    memset(*records,0,sizeof(Record_t) * num_records);
    return 0;
}

int read_records(const char *input_filename, Record_t *records, const size_t num_records) {
    // Check params
    if (!input_filename || !records || num_records == 0) {
        return -1;
    }

    // System call to open file
    int fd = open(input_filename, O_RDONLY);

    // Error opening file
    if (fd < 0) {
        return -2;
    }

    ssize_t data_read = 0;

    // Loop through records and read in the data
    for (size_t i = 0; i < num_records; ++i) {
        data_read = read(fd,&records[i], sizeof(Record_t));

        // Read error
        if (sizeof(Record_t) != data_read) {
            return -3;
        }
    }

    return 0;
}

int create_record(Record_t **new_record, const char* name, int age) {
    // Check params
    if (*new_record || !name || age < 1 || age > 200) {
        return -1;
    }

    // Check for length of string
    if (strlen(name) >= 50) {
        return -1;
    }

    // Check for newlines
    if (strcmp(name, "\n") == 0) {
        return -1;
    }

    // Allocate space for a new record
    *new_record = (Record_t*) malloc(sizeof(Record_t));

    if (!(*new_record)) {
        return -2;
    }

    // Initialize
    memcpy((*new_record)->name,name,sizeof(char) * strlen(name));
    (*new_record)->name[MAX_NAME_LEN - 1] = 0;
    (*new_record)->age = age;
    return 0;
}
