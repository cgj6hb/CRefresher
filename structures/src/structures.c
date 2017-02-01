
#include "../include/structures.h"
#include <stdio.h>

int compare_structs(sample_t* a, sample_t* b)
{
	// Check params
	if (!a || !b) {
		return 0;
	}

	// Check member a
	if (a->a != b->a) {
		return 0;
	}

	// Check member b
	if (a->b != b->b) {
		return 0;
	}

	// Check member c
	if (a->c != b->c) {
		return 0;
	}

	return 1;
}

void print_alignments()
{
	printf("Alignment of int is %zu bytes\n", __alignof__(int));
	printf("Alignment of double is %zu bytes\n",__alignof__(double));
	printf("Alignment of float is %zu bytes\n",__alignof__(float));
	printf("Alignment of char is %zu bytes\n",__alignof__(char));
	printf("Alignment of long long is %zu bytes\n",__alignof__(long long));
	printf("Alignment of short is %zu bytes\n",__alignof__(short));
	printf("Alignment of structs are %zu bytes\n",__alignof__(fruit_t));
}

int sort_fruit(const fruit_t* a,int* apples,int* oranges, const size_t size)
{
	// Check params
	if (!a || !apples || !oranges || size <= 0) {
		return -1;
	}

	// Loop through fruits array and count apples and oranges
	for (unsigned int i = 0; i < size; i++) {
		if (a[i].type == 1) { // Apple
			*(apples) += 1;
		} else { // Orange
			*(oranges) += 1;
		}
	}

	return size;
}

int initialize_array(fruit_t* a, int apples, int oranges)
{
	// Check params
	if (!a || apples < 0 || oranges < 0) {
		return -1;
	}

	// Initialize all apple arrays
	for (int i = 0; i < apples; i++) {
		initialize_apple((apple_t*)a + i);
	}

	// Initialize all orange arrays
	for (int i = apples; i < (apples + oranges); i++) {
		initialize_orange((orange_t*)a + i);
	}

	return 0;
}

int initialize_orange(orange_t* a)
{
	// Check params
	if (!a) {
		return -1;
	}

	a->type = 0;
	a->weight = 0;
	a->peeled = 0;
	return 0;
}

int initialize_apple(apple_t* a)
{
	// Check params
	if (!a) {
		return -1;
	}

	a->type = 1;
	a->weight = 0;
	a->worms = 0;
	return 0;
}
