#ifndef IMAGE_H
#define IMAGE_H

#include "quadtree.h"

/* Preenche a imagem com valores da quadtree */
void fill_image(struct quad_tree *qt, int **image);

/* Salva a imagem no formato PGM P5 */
void save_pgm_p5(const char *filename, int **image, int width, int height);

#endif /* IMAGE_H */
