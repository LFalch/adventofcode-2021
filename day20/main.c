#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#define _GNU_SOURCE 1

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define pixel(c) ((c) == '#' ? 1 : 0)
#define inbounds(uint, uint2, len) (uint < len && uint2 < len)

#define getpixel(img, x, y, dim, oob) (inbounds(x, y, dim) ? pixel(img[(x) + (y) * dim]) : oob)

// Returns next oob
int enhance(const char *input_image, char *output_image, const char enhancement[], size_t dim, int oob) {
    for (size_t y = 0; y < dim + 2; y++) {
        for (size_t x = 0; x < dim + 2; x++) {
            x -= 1;
            y -= 1;
            size_t in = 0;
            in |= getpixel(input_image, x-1, y-1, dim, oob) << 8;
            in |= getpixel(input_image, x  , y-1, dim, oob) << 7;
            in |= getpixel(input_image, x+1, y-1, dim, oob) << 6;
            in |= getpixel(input_image, x-1,  y , dim, oob) << 5;
            in |= getpixel(input_image, x  ,  y , dim, oob) << 4;
            in |= getpixel(input_image, x+1,  y , dim, oob) << 3;
            in |= getpixel(input_image, x-1, y+1, dim, oob) << 2;
            in |= getpixel(input_image, x  , y+1, dim, oob) << 1;
            in |= getpixel(input_image, x+1, y+1, dim, oob) << 0;
            x += 1;
            y += 1;

            output_image[y * (dim + 2) + x] = enhancement[in];
        }
    }
    return pixel(enhancement[oob * ((1<<9)-1)]);
}

int main() {
    FILE *f = fopen("input.txt", "r");
    char enhancement[512];

    assert(fread(enhancement, 512, 1, f) == 1);
    assert(getc(f) == '\n');
    assert(getc(f) == '\n');

    char input_image[150*150];
    size_t dim = 0;
    size_t ldim = -1;

    char *line = NULL;
    size_t n = 0;
    ssize_t size;

    while ((size = getline(&line, &n, f)) > 1) {
        if (ldim != -1) assert(size - 1 == ldim);
        ldim = size - 1;

        memcpy(&input_image[dim * ldim], line, ldim);
        ++dim;
    }
    assert(ldim == dim);

    // for (size_t y = 0; y < dim; y++) {
    //     for (size_t x = 0; x < dim; x++) {
    //         printf("%c", input_image[y * dim + x]);
    //     }
    //     printf("\n");
    // }

    char *middle_img = malloc((dim+2)*(dim+2));
    int oob = enhance(input_image, middle_img, enhancement, dim, 0);
    dim += 2;


    // for (size_t y = 0; y < dim; y++) {
    //     for (size_t x = 0; x < dim; x++) {
    //         printf("%c", middle_img[y * dim + x]);
    //     }
    //     printf("\n");
    // }

    char *last_img = malloc((dim + 2) * (dim + 2));
    oob = enhance(middle_img, last_img, enhancement, dim, oob);
    dim += 2;

    // for (size_t y= 0; y < dim; y++) {
    //     for (size_t x = 0; x < dim; x++) {
    //         printf("%c", last_img[y * dim + x]);
    //     }
    //     printf("\n");
    // } 

    unsigned long count = 0;

    for (size_t y = 0; y < dim; y++) {
        for (size_t x = 0; x < dim; x++) {
            if (pixel(last_img[(y)*dim + x])) count++;
        }
    }

    printf("%ld\n", count);

    free(middle_img);
    for (int n = 2; n < 50; n++) {
        char *new_last_img = malloc((dim + 2) * (dim + 2));
        oob = enhance(last_img, new_last_img, enhancement, dim, oob);
        dim += 2;
        free(last_img);
        last_img = new_last_img;
    }

    count = 0;

    for (size_t y = 0; y < dim; y++) {
        for (size_t x = 0; x < dim; x++) {
            if (pixel(last_img[(y)*dim + x])) count++;
        }
    }

    printf("%ld\n", count);
}
