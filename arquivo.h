#ifndef ARQUIVO_H
#define ARQUIVO_H

/* =====================================================
   arquivo.h — Protótipos do módulo de persistência
   em arquivos binários (.bin).
   Usa: fopen, fread, fwrite, fseek, ftell, fclose
   ===================================================== */

#include "animal.h"
#include "consulta.h"

#define ARQUIVO_ANIMAIS   "animais.bin"
#define ARQUIVO_CONSULTAS "consultas.bin"

/* --- Animais --- */
void salvar_animais(const Animal *vetor, int tamanho);
void carregar_animais(Animal **vetor, int *tamanho, int *capacidade);

/* --- Consultas --- */
void salvar_consultas(const Consulta *vetor, int tamanho);
void carregar_consultas(Consulta **vetor, int *tamanho, int *capacidade);

#endif /* ARQUIVO_H */
