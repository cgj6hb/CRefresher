#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/sys_prog.h"

// LOOK INTO OPEN, READ, WRITE, CLOSE, FSTAT/STAT, LSEEK
// GOOGLE FOR ENDIANESS HELP

bool bulk_read(const char *input_filename, void *dst, const size_t offset, const size_t dst_size) {
    //check that input_filename and dst aren't null
    if (!input_filename || !dst) {
        return false;
    }

    //check that offset is valid and dst_size is positive
    if (offset > dst_size || dst_size <= 0) {
        return false;
    }

    //attempt to open file
    //filedesc will be negative if this fails
    int filedesc = open(input_filename, O_RDONLY);

    //check if file was opened successfully
    if (filedesc < 0) {
        return false;
    } else {
        //attempt to read file
        if (read(filedesc, dst, dst_size) < 0) {
            return false;
        } else {
            //attempt to close file
            if (close(filedesc) < 0) {
                return false;
            } else {
                //success
                return true;
            }
        }
    }
}

bool bulk_write(const void *src, const char *output_filename, const size_t offset, const size_t src_size) {
    //check that src and output_filename aren't null
    if (!src || !output_filename) {
        return false;
    }

    //check that offset is valid and src_size is positive
    if (offset < 0 || src_size <= 0) {
        return false;
    }

    //attempt to open file
    int filedesc = open(output_filename, O_WRONLY);

    //check if file was opened successfully
    if (filedesc < 0) {
        return false;
    } else {
        //attempt to write to file
        if (write(filedesc, src, src_size) < 0) {
            return false;
        } else {
            //attempt to close file
            if (close(filedesc) < 0) {
                return false;
            } else {
                //success
                return true;
            }
        }
    }
}


bool file_stat(const char *query_filename, struct stat *metadata) {
    //check that query_filename and metadata aren't null
    if (!query_filename || !metadata) {
        return false;
    }

    //attempt to open file
    int filedesc = open(query_filename, O_RDONLY);

    //check if file was opened successfully
    if (filedesc < 0) {
        return false;
    } else {
        //obtain metadata from file if possible
        if (fstat(filedesc, metadata) < 0) {
            close(filedesc);
            return false;
        } else {
            close(filedesc);
            return true;
        }
    }
}

bool endianess_converter(uint32_t *src_data, uint32_t *dst_data, const size_t src_count) {
    //check that src_data and dst_data aren't null
    if (!src_data || !dst_data) {
        return false;
    }

    //check bounds on src_count
    if (src_count <= 0) {
        return false;
    }

    uint32_t input;//this temp variable will hold the src_data from a particular index to flip
    uint32_t byte1, byte2, byte3, byte4; //split input into bytes and flip endianness
    int i;
    for (i = 0; i < src_count; i++) {
        input = src_data[i];//assign to indexed src_data

        /* The next four lines of code will rearrange the four bytes from one
         * endianess to the other. The first and last bytes have to shift 24 bits
         * to swap with each other, while the two middle bytes only have to shift
         * 8 bits each, since they're already next to each other. The result of the 
         * shift is ANDed with 0xff at the relevant byte to keep the variable as
         * a 32 bit entity. This will be important for the final step.
         */
        byte1 = ((input>>24)&0x000000ff);//flip 3rd byte with 0th
        byte2 = ((input<<8)&0x00ff0000);//flip middle two bytes
        byte3 = ((input>>8)&0x0000ff00);//flip middle two bytes
        byte4 = ((input<<24)&0xff000000);//flip 0th byte with 3rd

        /* This last line "compresses" the four variables into the final answer.
         * Before this, each one has 32 bits, but 24 of those bits are 0s. The 
         * remaining 8 bits are the relevant portion from each variable. By 
         * ORing them together, you end up with the relevant 8 bits from each 
         * variable into one final "concatenated" result.
         */
        dst_data[i] = byte1 | byte2 | byte3 | byte4;
    }

    return true;
}
