#include "../include/image.h"
#include "../include/quadtree.h"
#include <stdio.h>
#include <stdlib.h>

/* Função principal do decodificador */
int main(int argc, char *argv[])
{
    int image_dimension;
    struct quad_tree *qt;
    int **image;
    int size;
    const char *input_file, *output_file;

    if (argc != 3){
        fprintf(stderr, "Uso: %s <bitstream_dat> <output_pgm> \n", argv[0]);
        return EXIT_FAILURE;
    }

    input_file = argv[1];
    output_file = argv[2];

    /* Abrir o arquivo binário contendo a quadtree */
    qt = load_quadtree(input_file, &image_dimension);
    if (!qt) {
        fprintf(stderr, "Erro ao carregar a quadtree\n");
        return 1;
    }

    /* Criar e inicializar a imagem */
    size = image_dimension;
    image = (int **)malloc(size * sizeof(int *));
    if (!image) {
        perror("Erro ao alocar memória para a imagem");
        free_quadtree(qt);
        return 1;
    }

    for (int i = 0; i < size; i++) {
        image[i] = (int *)malloc(size * sizeof(int));
        if (!image[i]) {
            perror("Erro ao alocar memória para a linha da imagem");
            while (--i >= 0) {
                free(image[i]);
            }
            free(image);
            free_quadtree(qt);
            return 1;
        }
    }

    /* Preencher a imagem com base na quadtree */
    fill_image(qt, image);

    /* Salvar a imagem em formato PGM */
    save_pgm_p5(output_file, image, size, size);

    /* Liberar memória */
    for (int i = 0; i < size; i++) {
        free(image[i]);
    }
    free(image);
    free_quadtree(qt);

    return 0;
}
