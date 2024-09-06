#include "../include/quadtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

square create_square(int x, int y, int size) {
    square rect;
    rect.x = x;
    rect.y = y;
    rect.size = size;

    return rect;
}

quadtree *create_quadtree(square boundary, unsigned char **image) {
    quadtree *tree = (quadtree *)malloc(sizeof(quadtree));
    tree->boundary = boundary;
    tree->northwest = tree->northeast = tree->southwest = tree->southeast = NULL;

    int sum = 0;
    int size = boundary.size;

    // calcular o valor médio dos pixel da região
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            sum += image[boundary.y + i][boundary.x + j];
        }
    }

    int average =  sum / (size * size);

    tree->pixel_value = (unsigned char) average;

    // Calcula a variancia dos pixels
    int variance = 0;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            variance += pow(image[boundary.y + i][boundary.x + j] - average, 2);
        }
    }

    variance /= (size * size);

    // Condição de uma nova folha (uma nova criação de quadrantes): Se a variância estiver abaixo do limiar ou o tamanho for 1
    if (variance < THRESHOLD || size == 1) {
        tree->is_leaf = true;
    } else {
        tree->is_leaf = false;
        int half_size = size / 2;

        // Cria as subárvores (novos quadrantes)
        tree->northwest = create_quadtree(create_square(boundary.x, boundary.y, half_size), image);
        tree->northeast = create_quadtree(create_square(boundary.x + half_size, boundary.y, half_size), image);
        tree->southwest = create_quadtree(create_square(boundary.x, boundary.y + half_size, half_size), image);
        tree->southeast = create_quadtree(create_square(boundary.x + half_size, boundary.y + half_size, half_size), image);
    }

    return tree;
}

void free_quadtree(quadtree *tree) {
    if(!tree->is_leaf) {
        free_quadtree(tree -> northwest);
        free_quadtree(tree -> northeast);
        free_quadtree(tree -> southwest);
        free_quadtree(tree -> southeast);
    }

    free(tree);
}
