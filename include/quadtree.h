#ifndef QUADTREE_H
#define QUADTREE_H

#include <stdbool.h>

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
    bool is_leaf;               /* Indica se o nó é uma folha */

    /* Ponteiros para os quadrantes filhos */
    struct quadtree *northwest;
    struct quadtree *northeast;
    struct quadtree *southwest;
    struct quadtree *southeast;
};

/* Protótipos de funções */
struct square create_square(int x, int y, int size);
struct quadtree *create_quadtree(struct square boundary, unsigned char **image, int threshold);
void free_quadtree(struct quadtree *tree);

#endif /* QUADTREE_H */
