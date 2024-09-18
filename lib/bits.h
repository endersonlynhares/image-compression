/*
 * **************************************************
 * Aluno: Enderson Aguiar Rodrigues Linhares
 * Matrícula: 20232045050074
 * Aluno: Matheus Giovanni Sagioro
 * Matrícula: 20232045050201
 * Aluno: Erivando de Castro Batista
 * Matrícula: 20232045050120
 * Aluno: Ian Felipo Nascimento Oliveira
 * Matrícula: 20232045050082
 * Avaliação 04: Trabalho Final
 * 04.505.23-2024.1 - Prof. Daniel Ferreira
 * Compilador: gcc-12
 * **************************************************
 */

#ifndef BITS_H
#define BITS_H

#include <stdio.h>
#include <stdint.h>

/* Lê num_bits a partir do arquivo em buffer, começando na posição bit_pos */
int read_bits(FILE *file, uint8_t *buffer, int *bit_pos, int num_bits);

#endif /* BITS_H */
