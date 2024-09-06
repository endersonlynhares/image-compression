#include <stdio.h>
#include <stdlib.h>
#include "../include/image.h"

unsigned char **read_pgm(const char *filename, int *width, int *height, int *max_value) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir a imagem");
        return NULL;
    }

    char type[3];
    fscanf(file, "%s", type);

    if(type[0] != 'P' || type[1] != '2'){
        perror("Arquivo PGM inválido");
        fclose(file);
        return NULL;
    }

    fscanf(file, "%d %d", width, height);
    fscanf(file, "%d", max_value);

    if (*width != *height){
        fprintf(stderr, "Err: A imagem deve ser quadrada. Largura: %d, Altura: %d", *width, *height);
        fclose(file);
        return NULL;
    }


    unsigned char **image = (unsigned char **)malloc((*height) * sizeof(unsigned char *));

    for(int i = 0; i < *height; i++){
        image[i] = (unsigned char *) malloc((*width) * sizeof(unsigned char));
        for(int j = 0; j < *width; j++){
            int pixel_value;
            fscanf(file, "%d", &pixel_value);
            image[i][j] = (unsigned char) pixel_value;
        }
    }

    fclose(file);

    return image;
}

void fill_image(quadtree *tree, unsigned char **image){
    if(tree->is_leaf) {
        for(int i = 0; i < tree->boundary.size; i++) {
            for(int j = 0; j < tree->boundary.size; j++) {
                image[tree->boundary.y + i][tree->boundary.x + j] = tree->pixel_value;
            }
        }
    }else {
        fill_image(tree->northwest, image);
        fill_image(tree->northeast, image);
        fill_image(tree->southwest, image);
        fill_image(tree->southeast, image);
    }
}

void save_pgm_from_quadtree(const char *filename, quadtree *tree, int size){
    FILE *file = fopen(filename, "w");

    if(!file){
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(file, "P2\n");
    fprintf(file, "%d %d\n", size, size);
    fprintf(file, "255\n");

    unsigned char **image = (unsigned char **)malloc(size * sizeof(unsigned char *));

    if (image == NULL) {
        perror("Erro ao alocar memória para a imagem");
        fclose(file);
        return;
    }
    
    for (int i = 0; i < size; i++) {
        image[i] = (unsigned char *)malloc(size * sizeof(unsigned char));
        if (image[i] == NULL) {
            perror("Erro ao alocar memória para a linha da imagem");
            for (int j = 0; j < i; j++) {
                free(image[j]);
            }
            free(image);
            fclose(file);
            return;
        }
    }

    fill_image(tree, image);

    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            fprintf(file, "%d ", (int)image[i][j]);
        }
        fprintf(file, "\n");
    }

    for(int i = 0; i < size; i++) {
        free(image[i]);
    }

    free(image);
    fclose(file);
}


