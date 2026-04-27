#ifndef CONSULTA_H
#define CONSULTA_H

/* =====================================================
   consulta.h — Definição da struct Consulta e
   protótipos do módulo de gerenciamento de consultas.
   ===================================================== */

#include "animal.h"

/* Status possíveis de uma consulta */
#define STATUS_AGENDADA   0
#define STATUS_REALIZADA  1
#define STATUS_CANCELADA  2

/* Estrutura da consulta médica */
typedef struct {
    int   id;
    int   id_animal;         /* referência ao Animal */
    char  data[12];          /* formato: DD/MM/AAAA */
    char  hora[8];           /* formato: HH:MM */
    char  veterinario[80];
    char  diagnostico[300];
    float valor;
    int   status;            /* STATUS_AGENDADA / REALIZADA / CANCELADA */
} Consulta;

/* --- Gerenciamento de memória dinâmica --- */
void inicializar_consultas(Consulta **vetor, int *tamanho, int *capacidade);
void redimensionar_consultas(Consulta **vetor, int *capacidade);
void liberar_consultas(Consulta *vetor);

/* --- Operações --- */
void agendar_consulta(Consulta **vetor, int *tamanho, int *capacidade,
                      const Animal *animais, int total_animais);
void listar_consultas(const Consulta *vetor, int tamanho,
                      const Animal *animais, int total_animais);
void finalizar_consulta(Consulta *vetor, int tamanho);
void cancelar_consulta(Consulta *vetor, int tamanho);
void historico_animal(const Consulta *vetor, int tamanho,
                      const Animal *animais, int total_animais);

/* --- Utilitário --- */
int proximo_id_consulta(const Consulta *vetor, int tamanho);

#endif /* CONSULTA_H */
