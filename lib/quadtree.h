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
 
#ifndef QUADTREE_H
#define QUADTREE_H

#include<stdlib.h>
#include<stdint.h>
#include "bits.h"

/* Estrutura que representa uma região quadrada na imagem */
struct square {
    int x;      /* Coordenada x do canto superior esquerdo */
    int y;      /* Coordenada y do canto superior esquerdo */
    int size;   /* Tamanho do quadrado (largura e altura) */
};

/* Estrutura do nó da quadtree */
struct quadtree {
    struct square boundary;     /* A área quadrada representada por este nó */
    unsigned char pixel_value;  /* Valor médio do pixel (para nós-folha) */
    unsigned char is_leaf;               /* Indica se o nó é uma folha */

    /* Ponteiros para os quadrantes filhos */
    struct quadtree *northwest;
    struct quadtree *northeast;
    struct quadtree *southwest;
    struct quadtree *southeast;
};

/* Protótipos de funções */
struct square create_square(int x, int y, int size);
void free_quadtree(struct quadtree *tree);
void save_quadtree_binary(struct quadtree *qt, FILE *file);
void save_quadtree(const char *filename, struct quadtree *qt, int image_dimension);
void write_9bits_to_file(FILE *file, uint8_t is_leaf, uint8_t pixel_value);
struct quadtree *load_quadtree_bin(FILE *file, int x, int y, int size);
struct quadtree *load_quadtree(const char *filename, int *image_dimension);

#endif /* QUADTREE_H */
