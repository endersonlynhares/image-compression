#ifndef IMAGE_COMPRESSION_H
#define IMAGE_COMPRESSION_H

#include "quadtree.h"

/* Estrutura de imagem com dimensões dinâmicas */
struct image {
    int width;
    int height;
    unsigned char **pixels;
};

/* Protótipos de funções */
struct image *read_pgm(const char *filename);
void save_pgm_from_quadtree(const char *filename, const struct quadtree *tree, int size);
void free_image(struct image *img);
void fill_image(const struct quadtree *tree, struct image *img);

#endif /* IMAGE_COMPRESSION_H */
