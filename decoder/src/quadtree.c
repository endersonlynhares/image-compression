#include "../include/quadtree.h"

/* Carrega a quadtree a partir de um arquivo binário */
struct quad_tree *load_quadtree_bin(FILE *file, int x, int y, int size)
{
    struct quad_tree *qt;
    static uint8_t buffer = 0; /* Buffer para armazenar bytes lidos do arquivo */
    static int bit_pos = 0;    /* Posição do bit atual no buffer */

    qt = (struct quad_tree *)malloc(sizeof(struct quad_tree));
    if (qt == NULL) {
        perror("Erro ao alocar memória para o quadtree");
        return NULL;
    }

    int is_leaf = read_bits(file, &buffer, &bit_pos, 1);   /* Lê o bit de controle */
    int pixel_value = read_bits(file, &buffer, &bit_pos, 8); /* Lê o valor do pixel */

    if (is_leaf < 0 || pixel_value < 0) {
        free(qt);
        return NULL; /* Erro na leitura */
    }

    qt->boundary.x = x;
    qt->boundary.y = y;
    qt->boundary.size = size;
    qt->pixel_value = (int)pixel_value;
    qt->is_leaf = (int)is_leaf;

    qt->northwest = qt->northeast = qt->southwest = qt->southeast = NULL;

    if (!qt->is_leaf) {
        int half_size = size / 2;

        qt->northwest = load_quadtree_bin(file, x, y, half_size);
        qt->northeast = load_quadtree_bin(file, x + half_size, y, half_size);
        qt->southwest = load_quadtree_bin(file, x, y + half_size, half_size);
        qt->southeast = load_quadtree_bin(file, x + half_size, y + half_size, half_size);
    }

    return qt;
}

/* Carrega a quadtree a partir de um arquivo de nome filename */
struct quad_tree *load_quadtree(const char *filename, int *image_dimension)
{
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erro ao abrir arquivo para carregar a quadtree");
        return NULL;
    }

    /* Ler a dimensão da imagem */
    fread(image_dimension, sizeof(unsigned int), 1, file);

    /* Carregar a quadtree a partir do arquivo binário */
    struct quad_tree *qt = load_quadtree_bin(file, 0, 0, *image_dimension);

    fclose(file);
    return qt;
}

/* Libera a memória alocada para a quadtree */
void free_quadtree(struct quad_tree *qt)
{
    if (qt == NULL) {
        return; /* Se o ponteiro for nulo, não faz nada */
    }

    /* Se não for uma folha, libera recursivamente os filhos */
    if (!qt->is_leaf) {
        free_quadtree(qt->northwest);
        free_quadtree(qt->northeast);
        free_quadtree(qt->southwest);
        free_quadtree(qt->southeast);
    }

    /* Finalmente, libera o próprio nó */
    free(qt);
}
