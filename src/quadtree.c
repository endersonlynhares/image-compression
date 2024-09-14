#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/quadtree.h"
#include "../include/image.h"

struct square create_square(int x, int y, int size)
{
    struct square sq;
    sq.x = x;
    sq.y = y;
    sq.size = size;

    return sq;
}

struct quadtree *create_quadtree(struct square boundary, struct image *img, int threshold)
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
            sum += img->pixels[boundary.y + i][boundary.x + j];
        }
    }

    avg = sum / (size * size);
    tree->pixel_value = (unsigned char)avg;

    /* Calcula a variação dos pixels */
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            variance += pow(img->pixels[boundary.y + i][boundary.x + j] - avg, 2);
        }
    }

    variance /= (size * size);

    /* Verifica se o nó deve ser uma folha com base na variação e no tamanho */
    if (variance < threshold || size == 1) {
        tree->is_leaf = 1;
    } else {
        int half_size = size / 2;

        tree->is_leaf = 0;

        /* Cria recursivamente os quadrantes filhos */
        tree->northwest = create_quadtree(create_square(boundary.x, boundary.y, half_size), img, threshold);
        tree->northeast = create_quadtree(create_square(boundary.x + half_size, boundary.y, half_size), img, threshold);
        tree->southwest = create_quadtree(create_square(boundary.x, boundary.y + half_size, half_size), img, threshold);
        tree->southeast = create_quadtree(create_square(boundary.x + half_size, boundary.y + half_size, half_size), img, threshold);
    }

    return tree;
}

// void save_quadtree_binary(struct quadtree *qt, FILE *file)
// {
//     if (qt == NULL) {
//         return;
//     }

//     unsigned char pixel_value = (unsigned char) qt->pixel_value;
//     unsigned char is_leaf = (unsigned char) qt->is_leaf;

//     fwrite(&is_leaf, sizeof(unsigned char), 1, file);
//     fwrite(&qt->boundary.x, sizeof(int), 1, file);
//     fwrite(&qt->boundary.y, sizeof(int), 1, file);
//     fwrite(&qt->boundary.size, sizeof(int), 1, file);
//     fwrite(&pixel_value, sizeof(unsigned char), 1, file);

//     if (!qt->is_leaf)
//     {
//         save_quadtree_binary(qt->northwest, file);
//         save_quadtree_binary(qt->northeast, file);
//         save_quadtree_binary(qt->southwest, file);
//         save_quadtree_binary(qt->southeast, file);
//     }
// }

void save_quadtree_binary(struct quadtree *qt, FILE *file)
{
    if (qt == NULL) {
        return;
    }

    unsigned char is_leaf = (unsigned char) qt->is_leaf;
    unsigned char pixel_value = (unsigned char) qt->pixel_value;

    fwrite(&is_leaf, sizeof(unsigned char), 1, file);
    fwrite(&pixel_value, sizeof(unsigned char), 1, file);

    if (!qt->is_leaf)
    {
        save_quadtree_binary(qt->northwest, file);
        save_quadtree_binary(qt->northeast, file);
        save_quadtree_binary(qt->southwest, file);
        save_quadtree_binary(qt->southeast, file);
    }
}


void save_quadtree(const char *filename, struct quadtree *qt)
{
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erro ao abrir arquivo para salvar a quadtree");
        return;
    }

    save_quadtree_binary(qt, file);

    fclose(file);
}

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
