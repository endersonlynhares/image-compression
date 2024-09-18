#ifndef QUADTREE_H
#define QUADTREE_H

#include <stdlib.h>
#include "square.h"
#include "bits.h"

struct quad_tree {
    struct rectangle boundary;
    int pixel_value;   /* Valor médio dos pixels na região */
    int is_leaf;       /* Flag para indicar se é folha */
    struct quad_tree *northwest;
    struct quad_tree *northeast;
    struct quad_tree *southwest;
    struct quad_tree *southeast;
};

/* Carrega a quadtree a partir de um arquivo binário */
struct quad_tree *load_quadtree_bin(FILE *file, int x, int y, int size);

/* Carrega a quadtree a partir de um arquivo de nome filename */
struct quad_tree *load_quadtree(const char *filename, int *image_dimension);

/* Libera a memória alocada para a quadtree */
void free_quadtree(struct quad_tree *qt);

#endif /* QUADTREE_H */
