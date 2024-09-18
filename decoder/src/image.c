#include "../../lib/image.h"

/* Preenche a imagem com valores da quadtree */
void fill_image(struct quadtree *qt, int **image)
{
    if (qt->is_leaf) {
        for (int i = 0; i < qt->boundary.size; i++) {
            for (int j = 0; j < qt->boundary.size; j++) {
                image[qt->boundary.y + i][qt->boundary.x + j] = qt->pixel_value;
            }
        }
    } else {
        fill_image(qt->northwest, image);
        fill_image(qt->northeast, image);
        fill_image(qt->southwest, image);
        fill_image(qt->southeast, image);
    }
}

/* Salva a imagem no formato PGM P5 */
void save_pgm_p5(const char *filename, int **image, int width, int height)
{
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return;
    }

    /* Cabeçalho PGM para formato P5 */
    fprintf(file, "P5\n");
    fprintf(file, "%d %d\n", width, height);
    fprintf(file, "255\n"); /* Valor máximo de pixel (supondo 8 bits por pixel) */

    /* Gravar os dados da imagem em formato binário */
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char pixel = (unsigned char)image[i][j];
            fwrite(&pixel, sizeof(unsigned char), 1, file);
        }
    }

    fclose(file);
}
