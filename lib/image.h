/*
 * **************************************************
 * Aluno: Enderson Aguiar Rodrigues Linhares
 * Matrícula: 20232045050074
 * Aluno: Matheus Giovanni Sagioro
 * Matrícula: 20232045050201
 * Aluno: Erivando de Castro Batista
 * Matrícula: 20232045050120
 * Aluno: Ian Felipo Nascimento Oliveira
 * Matrícula: 20232045050082
 * Avaliação 04: Trabalho Final
 * 04.505.23-2024.1 - Prof. Daniel Ferreira
 * Compilador: gcc-12
 * **************************************************
 */
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
void fill_image(struct quadtree *qt, int **image);
void save_pgm_p5(const char *filename, int **image, int width, int height);

#endif // IMAGE_COMPRESSION_H
