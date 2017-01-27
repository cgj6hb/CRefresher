#include "../include/bitmap.h"
#include <math.h>

// data is an array of uint8_t and needs to be allocated in bitmap_create
// and used in the remaining bitmap functions. You will use data for any bit operations and bit logic
// bit_count the number of requested bits, set in bitmap_create from n_bits
// byte_count the total number of bytes the data contains, set in bitmap_create

bitmap_t *bitmap_create(size_t n_bits) {
    //check for a valid value of bits
    if (n_bits < 1) {
        return NULL;
    }

    //calculate number of bytes based on number of bits
    //take the ceiling in case number of bits doesn't divide evenly by 8
    size_t n_bytes = ceil(n_bits / 8.0);
    bitmap_t* bitmap = malloc(sizeof(bitmap_t));

    //initialize and allocate members
    bitmap->bit_count = n_bits;
    bitmap->byte_count = n_bytes;
    bitmap->data = calloc(n_bytes, sizeof(*bitmap->data));

    return bitmap;
}

bool bitmap_set(bitmap_t *const bitmap, const size_t bit) {
    //check if bitmap is NULL
    if (!bitmap) {
        return false;
    }

    //check if bit is valid number
    if (bit < 0 || bit >= bitmap->bit_count) {
        return false;
    }

    /* The next line first locates the correct byte to manipulate. 
     * bit & 7 tells us how many bits to shift. That finally gets ORed with
     * bitmap->data[bit/8] to determine 0 or 1.
     */
    bitmap->data[bit/8] |= (uint8_t)1 << (bit & (7));
    return true;
}

bool bitmap_reset(bitmap_t *const bitmap, const size_t bit) {
    //check bounds of parameters
    if (!bitmap || bit >= bitmap->bit_count) {
        return false;
    } else {
        /* Similar to bitmap_set... the next line locates the correct byte to manipulate.
         * bit & 7 tells us how many bits to shift. That result gets ORed with 
         * bitmap->data[bit/8] to determine 0 or 1.
         */
        bitmap->data[bit/8] &= ~((uint8_t)1 << (bit & (7)));
        return true;
    }
}

bool bitmap_test(const bitmap_t *const bitmap, const size_t bit) {
    //check that bitmap isn't null
    if (!bitmap) {
        return false;
    }

    //check that bit is valid
    if (bit >= bitmap->bit_count) {
        return false;
    }

    /* This determines how many bits over to shift the bit in question. After
     * the shift, every bit will be 0 with the possible exception of the LSB, 
     * which could be 0 or 1. Whatever its value is determines the function's
     * final return value (true or false).
     */
    if ((bitmap->data[bit/8] >> (bit & 7)) == 1) {
        return true;
    } else {
        return false;
    }
}

size_t bitmap_ffs(const bitmap_t *const bitmap) {
    //check that bitmap isn't null
    if (!bitmap) {
        return SIZE_MAX;
    }

    int i;
    bool result;

    //loop through each bit until you find the first one that's set (using bitmap_test above)
    for (i = 0; i < bitmap->bit_count; i++) {
        result = bitmap_test(bitmap, i);

        //success
        if (result) {
            return i;
        }
    }

    //failure
    return SIZE_MAX;
}

size_t bitmap_ffz(const bitmap_t *const bitmap) {
    //check that bitmap isn't null
    if (!bitmap) {
        return SIZE_MAX;
    }

    /* The for loop loops until it finds a byte that isn't equal to 255 (1111 1111).
     * If it finds such a byte, we know there's at least one 0 bit in that byte.
     * The second if handles the byte being equal to 0. If the byte is greater than
     * 0 but less than 255, the else handles it. First, it inverts the number.
     * Then, it takes the 2's complement. Then it ANDS those two results, and then 
     * takes the binary logarithm. 
     */
    int i;
    for (i = 0; i < bitmap->byte_count; i++) {
        if (bitmap->data[i] !=  255) {
            if (bitmap_test(bitmap, i * 8)) {
                continue;
            } else {
                int data_inverted = ~bitmap->data[i];
                int data_2c = ~data_inverted + 1;
                int and = data_inverted & data_2c;
                return (i * 8) + log(and)/log(2);
            }
        }
    }

    //failure
    return SIZE_MAX;
}

bool bitmap_destroy(bitmap_t *bitmap) {
    //check that bitmap isn't NULL
    if (!bitmap) {
        return false;
    } else {
        //free the member and then free the struct
        free(bitmap->data);
        free(bitmap);
        return true;
    }
}
