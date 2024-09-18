#include "../include/bits.h"

/* Lê num_bits a partir do arquivo em buffer, começando na posição bit_pos */
int read_bits(FILE *file, uint8_t *buffer, int *bit_pos, int num_bits)
{
    int result = 0;

    for (int i = 0; i < num_bits; i++) {
        if (*bit_pos == 0) {
            if (fread(buffer, sizeof(uint8_t), 1, file) != 1) {
                return -1; /* Erro de leitura */
            }
        }

        result = (result << 1) | ((buffer[0] >> (7 - *bit_pos)) & 1);
        (*bit_pos)++;

        if (*bit_pos == 8) {
            *bit_pos = 0;
        }
    }

    return result;
}
