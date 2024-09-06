#ifndef IMAGE_COMPRESSION_H
#define IMAGE_COMPRESSION_H

#include "quadtree.h"

unsigned char **read_pgm(const char *, int *, int *, int *);
void save_pgm_from_quadtree(const char *, quadtree *, int);
void fill_image(quadtree *, unsigned char **);

#endif
