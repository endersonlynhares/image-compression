#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../include/quadtree.h"
#include "../include/image.h"

/* Cria uma estrutura que representa um quadrado com coordenadas e tamanho especificados. */
struct square create_square(int x, int y, int size)
{
    struct square sq;
    sq.x = x;
    sq.y = y;
    sq.size = size;

    return sq;
}

/* Cria uma quadtree para a região especificada da imagem com base no valor do limiar. */
struct quadtree *create_quadtree(struct square boundary, struct image *img, int threshold)
{
    struct quadtree *tree;
    int sum = 0, i, j;
    int variance = 0, size = boundary.size;
    int avg;

    /* Aloca memória para o nó da quadtree. */
    tree = malloc(sizeof(*tree));
    if (!tree) {
        perror("Erro ao alocar memória para o nó da quadtree");
        return NULL;
    }

    tree->boundary = boundary;
    tree->northwest = tree->northeast = tree->southwest = tree->southeast = NULL;

    /* Calcula o valor médio dos pixels dentro do quadrado. */
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            sum += img->pixels[boundary.y + i][boundary.x + j];
        }
    }

    avg = sum / (size * size);
    tree->pixel_value = (unsigned char)avg;

    /* Calcula a variação dos pixels. */
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            variance += pow(img->pixels[boundary.y + i][boundary.x + j] - avg, 2);
        }
    }

    variance /= (size * size);

    /* Verifica se o nó deve ser uma folha com base na variação e no tamanho. */
    if (variance < threshold || size == 1) {
        tree->is_leaf = 1;
    } else {
        int half_size = size / 2;

        tree->is_leaf = 0;

        /* Cria recursivamente os quadrantes filhos. */
        tree->northwest = create_quadtree(create_square(boundary.x, boundary.y, half_size), img, threshold);
        tree->northeast = create_quadtree(create_square(boundary.x + half_size, boundary.y, half_size), img, threshold);
        tree->southwest = create_quadtree(create_square(boundary.x, boundary.y + half_size, half_size), img, threshold);
        tree->southeast = create_quadtree(create_square(boundary.x + half_size, boundary.y + half_size, half_size), img, threshold);
    }

    return tree;
}

uint8_t bit_buffer = 0;  /* Armazena os bits temporariamente. */
int bit_count = 0;       /* Contagem de bits no buffer. */

/* Função para escrever 9 bits no arquivo. */
void write_9bits_to_file(FILE *file, uint8_t is_leaf, uint8_t pixel_value)
{
    /* Compacta os 9 bits (1 bit de is_leaf + 8 bits de pixel_value). */
    uint16_t data = ((is_leaf & 1) << 8) | (pixel_value & 0xFF);

    /* Insere bits no buffer de bits. */
    for (int i = 8; i >= 0; i--) {
        /* Extrai o bit mais significativo de data. */
        bit_buffer = (bit_buffer << 1) | ((data >> i) & 1);
        bit_count++;

        /* Se o buffer tiver 8 bits, escreve no arquivo. */
        if (bit_count == 8) {
            fwrite(&bit_buffer, sizeof(uint8_t), 1, file);
            bit_count = 0;
            bit_buffer = 0;  /* Resetar buffer. */
        }
    }
}

/* Função recursiva para salvar a quadtree de forma compactada. */
void save_quadtree_binary(struct quadtree *qt, FILE *file)
{
    if (qt == NULL) {
        return;
    }

    /* Salva 9 bits (1 bit para is_leaf, 8 bits para pixel_value). */
    write_9bits_to_file(file, qt->is_leaf, qt->pixel_value);

    /* Se não for folha, salva as subárvores. */
    if (!qt->is_leaf) {
        save_quadtree_binary(qt->northwest, file);
        save_quadtree_binary(qt->northeast, file);
        save_quadtree_binary(qt->southwest, file);
        save_quadtree_binary(qt->southeast, file);
    }
}

/* Função para salvar a quadtree em um arquivo binário. */
void save_quadtree(const char *filename, struct quadtree *qt, int image_dimension)
{
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erro ao abrir arquivo para salvar a quadtree");
        return;
    }

    /* Escreve a dimensão da imagem no início do arquivo. */
    fwrite(&image_dimension, sizeof(unsigned int), 1, file);

    /* Salva a quadtree de forma compactada. */
    save_quadtree_binary(qt, file);

    /* Se houver bits sobrando no buffer, escreva-os no arquivo. */
    if (bit_count > 0) {
        bit_buffer <<= (8 - bit_count);  /* Preencher o restante com zeros. */
        fwrite(&bit_buffer, sizeof(uint8_t), 1, file);
    }

    fclose(file);
}

/* Libera a memória alocada para a quadtree. */
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
