#ifndef QUADTREE_H
#define QUADTREE_H


/* Estrutura que representa uma região quadrada na imagem */
struct square {
    int x;      /* Coordenada x do canto superior esquerdo */
    int y;      /* Coordenada y do canto superior esquerdo */
    int size;   /* Tamanho do quadrado (largura e altura) */
};

/* Estrutura do nó da quadtree */
struct quadtree {
    struct square boundary;     /* A área quadrada representada por este nó */
    unsigned char pixel_value;  /* Valor médio do pixel (para nós-folha) */
    unsigned char is_leaf;               /* Indica se o nó é uma folha */

    /* Ponteiros para os quadrantes filhos */
    struct quadtree *northwest;
    struct quadtree *northeast;
    struct quadtree *southwest;
    struct quadtree *southeast;
};

/* Protótipos de funções */
struct square create_square(int x, int y, int size);
void free_quadtree(struct quadtree *tree);
void save_quadtree_binary(struct quadtree *qt, FILE *file);
void save_quadtree(const char *filename, struct quadtree *qt, int image_dimension);
void write_9bits_to_file(FILE *file, uint8_t is_leaf, uint8_t pixel_value);

#endif /* QUADTREE_H */
