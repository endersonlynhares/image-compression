#ifndef BITS_H
#define BITS_H

#include <stdio.h>
#include <stdint.h>

/* Lê num_bits a partir do arquivo em buffer, começando na posição bit_pos */
int read_bits(FILE *file, uint8_t *buffer, int *bit_pos, int num_bits);

#endif /* BITS_H */
