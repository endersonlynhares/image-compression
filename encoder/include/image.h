#ifndef IMAGE_COMPRESSION_H
#define IMAGE_COMPRESSION_H
#include "quadtree.h"

struct image {
    unsigned char type;
    unsigned char maxval;
    int width;
    int height;
    unsigned char **pixels;
};

void read_pgm(struct image *img, const char *filename);
struct quadtree *create_quadtree(struct square boundary, struct image *img, int threshold);

#endif // IMAGE_COMPRESSION_H
