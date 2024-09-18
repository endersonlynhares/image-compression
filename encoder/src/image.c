#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../../lib/image.h"

void read_pgm(struct image *img, const char *filename)
{
    FILE *file;
    char ch;
    int i, j, pixel_value;

    if (!img) {
        fclose(file);
        return;
    }

    if (!(file = fopen(filename, "r"))) {
        perror("Erro ao abrir a imagem");
        return;
    }

    if ( (ch = getc(file))!='P'){
        puts("A imagem fornecida não está no formato pgm");
        return;
    }

    img->type = getc(file)-48;

    if(img->type == 5){
        fclose(file);
        if (!(file = fopen(filename, "rb"))) {
            perror("Erro ao abrir a imagem");
            return;
        }
        fseek(file, 2, SEEK_CUR);
    }

	fseek(file, 1, SEEK_CUR);

	while((ch = getc(file))=='#'){
		while((ch = getc(file)) != '\n');
	}

    ungetc(ch, file);

    fscanf(file, "%d %d", &img->width, &img->height);
	if (ferror(file)){ 
		perror(NULL);
		exit(3);
	}

    fscanf(file, "%d",&img->maxval);

	fseek(file,1, SEEK_CUR);

    if (img->width != img->height) {
        fprintf(stderr, "Err: A imagem deve ser quadrada. Largura: %d, Altura: %d", img->width, img->height);
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

    switch(img->type){
        case 2:
            for (int i = 0; i < img->height; i++) {
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
                    img->pixels[i][j] = (unsigned char) pixel_value;
                }
            }
        break;
        case 5:
            for (int i = 0; i < img->height; i++) {
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
                    img->pixels[i][j] = (unsigned char) pixel_value;
                }
            }
        break;
        default:
            puts("Não está implementado esse formato.");
    }

    fclose(file);
}

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
