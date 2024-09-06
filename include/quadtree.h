#ifndef QUADTREE_H
#define QUADTREE_H

#include <stdbool.h>

#define THRESHOLD 90

typedef struct square {
    int x, y;
    int size;
} square;

typedef struct quadtree {
    square boundary;
    unsigned char pixel_value;
    bool is_leaf;
    struct quadtree *northwest;
    struct quadtree *northeast;
    struct quadtree *southwest;
    struct quadtree *southeast;
} quadtree;

square create_square(int, int, int);
quadtree *create_quadtree(square, unsigned char **);
void free_quadtree(quadtree *);

#endif
