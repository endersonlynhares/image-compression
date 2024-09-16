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

    /* Verifica se a estrutura da imagem é válida. */
    if (!img) {
        return;
    }

    /* Abre o arquivo de imagem no modo de leitura. */
    file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir a imagem");
        return;
    }

    /* Verifica o cabeçalho do formato da imagem. */
    ch = getc(file);
    if (ch != 'P') {
        puts("A imagem fornecida não está no formato PGM");
        fclose(file);
        return;
    }

    /* Lê o tipo de imagem (P2 ou P5). */
    img->type = getc(file) - '0';

    /* Se a imagem for P5 (binária), reabre o arquivo em modo binário. */
    if (img->type == 5) {
        fclose(file);
        file = fopen(filename, "rb");
        if (!file) {
            perror("Erro ao abrir a imagem");
            return;
        }
        fseek(file, 2, SEEK_CUR); /* Pular o cabeçalho "P5" */
    }

    /* Pular o caractere de nova linha após o tipo. */
    fseek(file, 1, SEEK_CUR);

    /* Ignora comentários no arquivo (linhas que começam com '#'). */
    while ((ch = getc(file)) == '#') {
        while ((ch = getc(file)) != '\n')
            ;
    }

    /* Coloca o caractere lido de volta no fluxo de entrada. */
    ungetc(ch, file);

    /* Lê a largura e altura da imagem. */
    if (fscanf(file, "%d %d", &img->width, &img->height) != 2) {
        perror("Erro ao ler a largura e altura");
        fclose(file);
        return;
    }

    /* Lê o valor máximo de intensidade do pixel. */
    if (fscanf(file, "%hhd", &img->maxval) != 1) {
        perror("Erro ao ler o valor máximo");
        fclose(file);
        return;
    }

    /* Pular o caractere de nova linha após o valor máximo. */
    fseek(file, 1, SEEK_CUR);

    /* Verifica se a imagem é quadrada. */
    if (img->width != img->height) {
        fprintf(stderr, "Erro: A imagem deve ser quadrada. Largura: %d, Altura: %d\n",
                img->width, img->height);
        free(img);
        fclose(file);
        return;
    }

    /* Aloca memória para a matriz de pixels. */
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

    /* Lê os dados dos pixels dependendo do tipo de imagem. */
    switch (img->type) {
    case 2: /* P2 (texto). */
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
    case 5: /* P5 (binário). */
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

    /* Verifica se a estrutura da imagem é válida. */
    if (!img)
        return;

    /* Libera a memória de cada linha de pixels. */
    for (i = 0; i < img->height; i++)
        free(img->pixels[i]);

    /* Libera a memória da matriz de pixels e da estrutura da imagem. */
    free(img->pixels);
    free(img);
}
