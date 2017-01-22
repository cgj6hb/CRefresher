#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/arrays.h"

// LOOK INTO MEMCPY, MEMCMP, FREAD, and FWRITE

/* this function receives two arrays and copies the source array into the
 * destination array. 
 */
bool array_copy(const void *src, void *dst, const size_t elem_size, const size_t elem_count) {
	//check that src and dst aren't null
	if (!src || !dst) {
		return false;
	}

	//check that elem_size and elem_count are positive
	if (elem_size <= 0 || elem_count <= 0) {
		return false;
	}

	memcpy(dst, src, elem_size * elem_count);

	//make sure dst was set properly
	if (dst) {
		return true;
	} else {
		return false;
	}
}

/* this function receives two arrays and checks to see if the contents are 
 * equal.
 */
bool array_is_equal(const void *data_one, void *data_two, const size_t elem_size, const size_t elem_count) {
	//check that data_one and data_two aren't null
	if (!data_one || !data_two) {
		return false;
	}

	//check that elem_size and elem_count are positive
	if (elem_size <= 0 || elem_count <= 0) {
		return false;
	}

	int result = memcmp(data_one, data_two, elem_size * elem_count);

	//make sure memcmp worked successfully
	if (result == 0) {
		return true;
	} else {
		return false;
	}
}

/* this function receives an array and a target and checks to see if the target
 * exists in the array.
 */
ssize_t array_locate(const void *data, const void *target, const size_t elem_size, const size_t elem_count) {
	//check that data and target aren't null
	if (!data || !target) {
		return -1;
	}

	//check that elem_size and elem_count are positive
	if (elem_size <= 0 || elem_count <= 0) {
		return -1;
	}

	//loop through array by number of bytes
	for (int i = 0; i < (elem_count * elem_size); i += elem_size) {
		//compare current index with the target, and increment index if target not found
		if (memcmp(data, target, elem_size) == 0) {
			return (i / elem_size);
		} else {
			data += elem_size;
		}
	}
	
	return -1;
}

/* this function receives an array and writes its contents to a binary file. */
bool array_serialize(const void *src_data, const char *dst_file, const size_t elem_size, const size_t elem_count) {
	//check that src_data and dst_file aren't null
	if (!src_data || !dst_file) {
		return false;
	}

	//check bounds
	if (elem_size <= 0 || elem_count <= 0) {
		return false;
	}

	//check for newlines
	if (strcmp(dst_file, "\n") == 0) {
		return false;
	}

	FILE* fp = fopen(dst_file, "wb");
	
	//check that file was opened successfully
	if (!fp) {
		return false;
	}
	
	int result = fwrite(src_data, elem_size, elem_count, fp);
	fclose(fp);

	//make sure fwrite was successful
	if (result != elem_count) {
		return false;
	} else {
		return true;
	}
}

/* this function receives a binary file and reads its content into an array. */
bool array_deserialize(const char *src_file, void *dst_data, const size_t elem_size, const size_t elem_count) {
	//check that src_file and dst_data aren't null
	if (!src_file || !dst_data) {
		return false;
	}

	//check bounds
	if (elem_size <= 0 || elem_count <= 0) {
		return false;
	}

	//check for tricky newlines
	if (strcmp(src_file, "\n") == 0) {
		return false;
	}

	FILE* fp = fopen(src_file, "rb");

	//check that file was opened successfully
	if (!fp) {
		return false;
	}

	int result = fread(dst_data, elem_size, elem_count, fp);
	fclose(fp);

	//check that fread was successful
	if (result != elem_count) {
		return false;
	} else {
		return true;
	}
}
