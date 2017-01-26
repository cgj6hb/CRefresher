#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../include/sstring.h"

bool string_valid(const char *str, const size_t length) {
    //check that str isn't null
    if (!str) {
        return false;
    }

    //check that length is valid number
    if (length < 1) {
        return false;
    }

    //grab last character of string, to compare against null terminator later
    const char last_char = str[length - 1];

    //compare and return accordingly
    if (strcmp(&last_char, "\0") == 0) {
        return true;
    } else {
        return false;
    }
}

char *string_duplicate(const char *str, const size_t length) {
    //check that str isn't null
    if (!str) {
        return NULL;
    }

    //check that length is valid number
    if (length < 1) {
        return NULL;
    }

    //allocate space for second string and copy contents of str into it
    char* str2 = (char*) malloc(sizeof(char) * length);
    memcpy(str2, str, (sizeof(char) * length));
    return str2;
}

bool string_equal(const char *str_a, const char *str_b, const size_t length) {
    //check that str_a and str_b aren't null
    if (!str_a || !str_b) {
        return false;
    }

    //check that length is valid number
    if (length < 1) {
        return false;
    }

    //compare strings using strcmp and return result
    if (strcmp(str_a, str_b) == 0) {
        return true;
    } else {
        return false;
    }
}

int string_length(const char *str, const size_t length) {
    //check that str isn't null
    if (!str) {
        return -1;
    }

    //veryify length
    if (length < 1) {
        return -1;
    }

    int str_length = strlen(str);

    //compare length of string with max possible string length
    if (str_length > length) {
        return -1;
    } else {
        return str_length;
    }
}

int string_tokenize(const char *str, const char *delims, const size_t str_length,char **tokens, const size_t max_token_length, const size_t requested_tokens) {
    //check that str, delims, and tokens aren't null
    if (!str || !delims || !tokens) {
        return 0;
    }

    //check bounds for other parameters
    if (str_length < 1 || max_token_length < 1 || requested_tokens < 1) {
        return 0;
    }

    //make a coy of str and ensure that it cannot be longer than max_token_length
    //will use this to obtain the first token

    //due to the behavior of strtok, the first token must be found outside of the loop, 
    //because afterwards strtok calls receive a NULL pointer, not the original string
    char str2[max_token_length];
    strcpy(str2, str);
    char* token_ptr = strtok(str2, delims);
    int i;

    for (i = 0; i < requested_tokens; i++) {
        //quick double check
        if (!token_ptr) {
            break;
        }

        //if there is an open slot in the tokens array, copy the string in
        if (tokens[i]) {
            if (token_ptr) {
                strcpy(tokens[i], token_ptr);
            } else {
                break;
            }
        } else {
            return -1;//misallocation
        }

        token_ptr = strtok(NULL, delims);
    }

    //return number of tokens
    return i;
}

bool string_to_int(const char *str, int *converted_value) {
    //check that pointers aren't null
    if (!str || !converted_value) {
        return false;
    }

    /* originally tried to use atoi(), but that function does not have a way to
     * indicate if the result is out of bounds for an int type. Instead, it just
     * reacts with undefined behavior. strtol() on the other hand has more informative
     * return types, or I can just compare the result with INT_MAX, which is what I 
     * decided to do.
     */
    long int converted_value_long = strtol(str, NULL, 10);

    //here is the check mentioned above
    if (converted_value_long < INT_MAX) {
        *converted_value = converted_value_long;
        return true;
    } else {
        return false;
    }
}
