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

#ifndef IMAGE_H
#define IMAGE_H

#include "quadtree.h"

/* Estrutura que representa uma imagem. */
struct image {
    unsigned char type;        /* Tipo da imagem (P2 ou P5). */
    unsigned char maxval;      /* Valor máximo dos pixels. */
    int width;                 /* Largura da imagem. */
    int height;                /* Altura da imagem. */
    unsigned char **pixels;    /* Matriz de pixels da imagem. */
};

/* Função para ler uma imagem no formato PGM. */
void read_pgm(struct image *img, const char *filename);

/* Função para criar uma quadtree a partir da imagem e um valor de limiar. */
struct quadtree *create_quadtree(struct square boundary, struct image *img, int threshold);

#endif /* IMAGE_H */
