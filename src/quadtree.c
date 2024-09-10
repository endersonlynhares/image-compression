#include "../include/quadtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Create and return a square given its top-left coordinates and size */
struct square create_square(int x, int y, int size)
{
    struct square sq;
    sq.x = x;
    sq.y = y;
    sq.size = size;

    return sq;
}

/* Cria uma quadtree para um dado limite e dados de imagem usando um limite de variação dinâmico */
struct quadtree *create_quadtree(struct square boundary, unsigned char **image, int threshold)
{
    struct quadtree *tree;
    int sum = 0, i, j;
    int variance = 0, size = boundary.size;
    int avg;

    /* Aloca memória para o nó da quadtree */
    tree = malloc(sizeof(*tree));
    if (!tree) {
        perror("Erro ao alocar memória para o nó da quadtree");
        return NULL;
    }

    tree->boundary = boundary;
    tree->northwest = tree->northeast = tree->southwest = tree->southeast = NULL;

    /* Calcula o valor médio dos pixels dentro do quadrado */
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            sum += image[boundary.y + i][boundary.x + j];
        }
    }

    avg = sum / (size * size);
    tree->pixel_value = (unsigned char)avg;

    /* Calcula a variação dos pixels */
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            variance += pow(image[boundary.y + i][boundary.x + j] - avg, 2);
        }
    }

    variance /= (size * size);

    /* Verifica se o nó deve ser uma folha com base na variação e no tamanho */
    if (variance < threshold || size == 1) {
        tree->is_leaf = true;
    } else {
        int half_size = size / 2;

        tree->is_leaf = false;

        /* Cria recursivamente os quadrantes filhos */
        tree->northwest = create_quadtree(create_square(boundary.x, boundary.y, half_size), image, threshold);
        tree->northeast = create_quadtree(create_square(boundary.x + half_size, boundary.y, half_size), image, threshold);
        tree->southwest = create_quadtree(create_square(boundary.x, boundary.y + half_size, half_size), image, threshold);
        tree->southeast = create_quadtree(create_square(boundary.x + half_size, boundary.y + half_size, half_size), image, threshold);
    }

    return tree;
}

/* Libera a memória alocada para a quadtree */
void free_quadtree(struct quadtree *tree)
{
    if (!tree)
        return;

    if (!tree->is_leaf) {
        free_quadtree(tree->northwest);
        free_quadtree(tree->northeast);
        free_quadtree(tree->southwest);
        free_quadtree(tree->southeast);
    }

    free(tree);
}
