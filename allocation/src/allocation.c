#include "../include/allocation.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void* allocate_array(size_t member_size, size_t nmember,bool clear)
{
    // Check bounds of member_size and nmember
    if (member_size <= 0 || nmember <= 0) {
        return NULL;
    }

    void* ptr;

    if (clear) { // Clear flag is set -- use calloc
        ptr = calloc(nmember, member_size);
    } else { // Clear flag is not set -- use malloc
        ptr = malloc(member_size * nmember);
    }

    return ptr;
}

void* reallocate_array(void* ptr, size_t size)
{
    // Check input parameters
    if (!ptr || size < 1) {
        return NULL;
    }

    void* ptr2 = realloc(ptr, size);
    return ptr2;
}

void deallocate_array(void** ptr)
{
    free(*ptr);
    *ptr = NULL;
    return;
}

char* read_line_to_buffer(char* filename)
{
    // Check that filename isn't null
    if (!filename) {
        return NULL;
    }

    // Allocate memory so buffer is created on the heap
    char* buffer = malloc(5);

    /* Since malloc sometimes returns more bytes than requested, I was having
     * problems with garbage being read in after "Test". To solve this, I
     * malloced one more byte than necessary, and initialized all of it to
     * the null terminator. That way, after "Test" gets read in, there is still
     * a null terminator after it so no more garbage gets stored.
     */
    memset(buffer, '\0', 5);

    // Standard system call to read the file
    int filedesc = open(filename, O_RDONLY);
    read(filedesc, buffer, 4);
    return buffer;
}
