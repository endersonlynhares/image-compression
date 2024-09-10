#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* Lê uma imagem PGM de um arquivo e retorna uma estrutura de imagem alocada */
struct image *read_pgm(const char *filename)
{
    FILE *file;
    char type[3];
    struct image *img;
    int i, j, pixel_value, max_value;

    file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir a imagem");
        return NULL;
    }

    if (fscanf(file, "%2s", type) != 1 || type[0] != 'P' || type[1] != '2') {
        perror("Arquivo PGM inválido");
        fclose(file);
        return NULL;
    }

    img = malloc(sizeof(struct image));
    if (!img) {
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "%d %d", &img->width, &img->height) != 2 || fscanf(file, "%d", &max_value) != 1) {
        perror("Erro ao ler dimensões da imagem");
        free(img);
        fclose(file);
        return NULL;
    }

    if (img->width != img->height) {
        fprintf(stderr, "Err: A imagem deve ser quadrada. Largura: %d, Altura: %d", img->width, img->height);
        free(img);
        fclose(file);
        return NULL;
    }

    img->pixels = malloc(img->height * sizeof(unsigned char *));
    if (!img->pixels) {
        perror("Erro ao alocar memória para pixels");
        free(img);
        fclose(file);
        return NULL;
    }

    for (i = 0; i < img->height; i++) {
        img->pixels[i] = malloc(img->width * sizeof(unsigned char));
        if (!img->pixels[i]) {
            perror("Erro ao alocar memória para linha da imagem");
            while (--i >= 0)
                free(img->pixels[i]);
            free(img->pixels);
            free(img);
            fclose(file);
            return NULL;
        }

        for (j = 0; j < img->width; j++) {
            if (fscanf(file, "%d", &pixel_value) != 1) {
                perror("Erro ao ler pixel");
                while (--i >= 0)
                    free(img->pixels[i]);
                free(img->pixels);
                free(img);
                fclose(file);
                return NULL;
            }
            img->pixels[i][j] = (unsigned char) pixel_value;
        }
    }

    fclose(file);
    return img;
}

/* Libera a memória alocada para uma imagem */
void free_image(struct image *img)
{
    int i;

    if (!img)
        return;

    for (i = 0; i < img->height; i++)
        free(img->pixels[i]);

    free(img->pixels);
    free(img);
}

/* Preenche uma imagem usando os dados da quadtree */
void fill_image(const struct quadtree *tree, struct image *img)
{
    int i, j;

    if (tree->is_leaf) {
        for (i = 0; i < tree->boundary.size; i++) {
            for (j = 0; j < tree->boundary.size; j++) {
                img->pixels[tree->boundary.y + i][tree->boundary.x + j] = tree->pixel_value;
            }
        }
    } else {
        fill_image(tree->northwest, img);
        fill_image(tree->northeast, img);
        fill_image(tree->southwest, img);
        fill_image(tree->southeast, img);
    }
}

/* Salva uma imagem PGM a partir dos dados da quadtree */
void save_pgm_from_quadtree(const char *filename, const struct quadtree *tree, int size)
{
    FILE *file;
    struct image *img;
    int i, j;

    file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    img = malloc(sizeof(struct image));
    if (!img) {
        fclose(file);
        return;
    }

    img->width = size;
    img->height = size;
    img->pixels = malloc(size * sizeof(unsigned char *));
    if (!img->pixels) {
        perror("Erro ao alocar memória para a imagem");
        free(img);
        fclose(file);
        return;
    }

    for (i = 0; i < size; i++) {
        img->pixels[i] = malloc(size * sizeof(unsigned char));
        if (!img->pixels[i]) {
            perror("Erro ao alocar memória para a linha da imagem");
            while (--i >= 0)
                free(img->pixels[i]);
            free(img->pixels);
            free(img);
            fclose(file);
            return;
        }
    }

    fill_image(tree, img);

    fprintf(file, "P2\n%d %d\n255\n", size, size);
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            fprintf(file, "%d ", img->pixels[i][j]);
        }
        fprintf(file, "\n");
    }

    for (i = 0; i < size; i++)
        free(img->pixels[i]);

    free(img->pixels);
    free(img);
    fclose(file);
}
