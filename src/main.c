#include <stdio.h>
#include <stdlib.h>
#include "../include/quadtree.h"
#include "../include/image.h"

int main(int argc, char *argv[])
{
    const char *input_file;
    const char *output_file;
    int threshold;
    int width, height, max_value;
    unsigned char **image;
    struct square boundary;
    struct quadtree *tree;

    /* Verifica os argumentos da linha de comando */
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <input_pgm> <output_pgm> <threshold>\n", argv[0]);
        return EXIT_FAILURE;
    }

    input_file = argv[1];
    output_file = argv[2];
    threshold = atoi(argv[3]);

    if (threshold <= 0) {
        fprintf(stderr, "Erro: O limiar deve ser um número inteiro positivo.\n");
        return EXIT_FAILURE;
    }

    /* Lê a imagem PGM de entrada */
    image = read_pgm(input_file, &width, &height, &max_value);
    if (image == NULL) {
        fprintf(stderr, "Erro: Falha ao ler a imagem de entrada %s.\n", input_file);
        return EXIT_FAILURE;
    }

    /* Cria a quadtree para a imagem */
    boundary = create_square(0, 0, width);
    tree = create_quadtree(boundary, image, threshold);
    if (tree == NULL) {
        fprintf(stderr, "Erro: Falha ao criar a quadtree.\n");
        for (int i = 0; i < height; i++) {
            free(image[i]);
        }
        free(image);
        return EXIT_FAILURE;
    }

    /* Salva a imagem comprimida */
    save_pgm_from_quadtree(output_file, tree, width);

    /* Libera a memória alocada */
    free_quadtree(tree);
    for (int i = 0; i < height; i++) {
        free(image[i]);
    }
    free(image);

    return EXIT_SUCCESS;
}
