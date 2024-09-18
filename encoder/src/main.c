#include <stdio.h>
#include <stdlib.h>
#include "../../lib/quadtree.h"
#include "../../lib/image.h"

int main(int argc, char *argv[])
{
    const char *input_file;
    const char *output_file;
    int threshold;

    struct image img;
    struct square boundary;
    struct quadtree *tree;

    /* Verifica os argumentos da linha de comando */
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <input_pgm> <bitstream_dat> <threshold>\n", argv[0]);
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
    read_pgm(&img, input_file);

    /* Cria a quadtree para a imagem */
    boundary = create_square(0, 0, img.width);
    tree = create_quadtree(boundary, &img, threshold);
    if (tree == NULL) {
        fprintf(stderr, "Erro: Falha ao criar a quadtree.\n");
        return EXIT_FAILURE;
    }

    /* Salva a imagem comprimidaem versão uma bitstream */
    save_quadtree(output_file, tree, img.width);

    /* Libera a memória alocada */
    free_quadtree(tree);
    return EXIT_SUCCESS;
}
