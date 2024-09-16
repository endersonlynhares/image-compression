#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../include/image.h"

/* Lê uma imagem PGM do arquivo e armazena os dados na estrutura 'img'. */
void read_pgm(struct image *img, const char *filename)
{
    FILE *file;
    char ch;
    int i, j, pixel_value;

    if (!img) {
        return;
    }

    file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir a imagem");
        return;
    }

    ch = getc(file);
    if (ch != 'P') {
        puts("A imagem fornecida não está no formato PGM");
        fclose(file);
        return;
    }

    img->type = getc(file) - '0';

    if (img->type == 5) {
        fclose(file);
        file = fopen(filename, "rb");
        if (!file) {
            perror("Erro ao abrir a imagem");
            return;
        }
        fseek(file, 2, SEEK_CUR);
    }

    fseek(file, 1, SEEK_CUR);

    while ((ch = getc(file)) == '#') {
        while ((ch = getc(file)) != '\n')
            ;
    }

    ungetc(ch, file);

    if (fscanf(file, "%d %d", &img->width, &img->height) != 2) {
        perror("Erro ao ler a largura e altura");
        fclose(file);
        return;
    }

    if (fscanf(file, "%hhd", &img->maxval) != 1) {
        perror("Erro ao ler o valor máximo");
        fclose(file);
        return;
    }

    fseek(file, 1, SEEK_CUR);

    if (img->width != img->height) {
        fprintf(stderr, "Erro: A imagem deve ser quadrada. Largura: %d, Altura: %d\n",
                img->width, img->height);
        free(img);
        fclose(file);
        return;
    }

    img->pixels = malloc(img->height * sizeof(unsigned char *));
    if (!img->pixels) {
        perror("Erro ao alocar memória para pixels");
        free(img);
        fclose(file);
        return;
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
            return;
        }
    }

    switch (img->type) {
    case 2:
        for (i = 0; i < img->height; i++) {
            for (j = 0; j < img->width; j++) {
                if (fscanf(file, "%d", &pixel_value) != 1) {
                    perror("Erro ao ler pixel");
                    while (--i >= 0)
                        free(img->pixels[i]);
                    free(img->pixels);
                    free(img);
                    fclose(file);
                    return;
                }
                img->pixels[i][j] = (unsigned char)pixel_value;
            }
        }
        break;
    case 5:
        for (i = 0; i < img->height; i++) {
            for (j = 0; j < img->width; j++) {
                if (fread(&pixel_value, sizeof(unsigned char), 1, file) != 1) {
                    perror("Erro ao ler pixel - P5");
                    while (--i >= 0)
                        free(img->pixels[i]);
                    free(img->pixels);
                    free(img);
                    fclose(file);
                    return;
                }
                img->pixels[i][j] = (unsigned char)pixel_value;
            }
        }
        break;
    default:
        puts("Formato não implementado.");
        break;
    }

    fclose(file);
}

/* Libera a memória alocada para a estrutura da imagem. */
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
