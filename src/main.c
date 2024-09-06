#include <stdio.h>
#include <stdlib.h>
#include "../include/quadtree.h"
#include "../include/image_compression.h"

int main(void) {
    int width, height, max_value;
    unsigned char **image = read_pgm("images/input/input.pgm", &width, &height, &max_value);

    if(image == NULL) {
        return 1;
    }

    square boundary = create_square(0, 0, width);
    quadtree *tree = create_quadtree(boundary, image);
    
    save_pgm_from_quadtree("images/output/output.pgm", tree, width);

    free_quadtree(tree);

    for(int i = 0; i < height; i++) {
        free(image[i]);
    }

    free(image);

    return 0;
}
