#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consulta.h"

/* =====================================================
   consulta.c — Implementação do módulo de consultas.
   Inclui: agendamento, finalização, cancelamento,
   listagem e histórico por animal.
   ===================================================== */

/* Descrição textual do status da consulta */
static const char *status_texto(int s) {
    switch (s) {
        case STATUS_AGENDADA:  return "Agendada";
        case STATUS_REALIZADA: return "Realizada";
        case STATUS_CANCELADA: return "Cancelada";
        default:               return "Desconhecido";
    }
}

/* Exibe os dados resumidos de uma consulta,
   resolvendo o nome do animal quando possível. */
static void exibir_consulta(const Consulta *c,
                             const Animal *animais, int total_animais) {
    const char *nome_animal = "?";
    for (int i = 0; i < total_animais; i++) {
        if (animais[i].id == c->id_animal) {
            nome_animal = animais[i].nome;
            break;
        }
    }
    printf("  +--------------------------------------------------+\n");
    printf("  | Consulta ID: %-5d  Status: %-16s |\n",
           c->id, status_texto(c->status));
    printf("  | Animal: %-41s |\n", nome_animal);
    printf("  | Data/Hora: %-10s  %5s                     |\n",
           c->data, c->hora);
    printf("  | Veterinario: %-36s |\n", c->veterinario);
    if (c->status == STATUS_REALIZADA) {
        printf("  | Diagnostico: %-36s |\n", c->diagnostico);
        printf("  | Valor: R$ %-39.2f |\n", c->valor);
    }
    printf("  +--------------------------------------------------+\n");
}

/* -----------------------------------------------------
   Gerenciamento de memória dinâmica
   ----------------------------------------------------- */

void inicializar_consultas(Consulta **vetor, int *tamanho, int *capacidade) {
    *tamanho    = 0;
    *capacidade = 8;
    *vetor = (Consulta *) malloc((*capacidade) * sizeof(Consulta));
    if (*vetor == NULL) {
        fprintf(stderr, "Erro fatal: nao foi possivel alocar memoria para consultas.\n");
        exit(EXIT_FAILURE);
    }
}

void redimensionar_consultas(Consulta **vetor, int *capacidade) {
    int nova_cap = (*capacidade) * 2;
    Consulta *temp = (Consulta *) realloc(*vetor, nova_cap * sizeof(Consulta));
    if (temp == NULL) {
        fprintf(stderr, "Erro: falha ao redimensionar vetor de consultas.\n");
        return;
    }
    *vetor      = temp;
    *capacidade = nova_cap;
    printf("[Sistema] Capacidade de consultas ampliada para %d registros.\n", nova_cap);
}

void liberar_consultas(Consulta *vetor) {
    free(vetor);
}

/* -----------------------------------------------------
   Utilitário
   ----------------------------------------------------- */

int proximo_id_consulta(const Consulta *vetor, int tamanho) {
    int max = 0;
    for (int i = 0; i < tamanho; i++) {
        if (vetor[i].id > max) max = vetor[i].id;
    }
    return max + 1;
}

/* -----------------------------------------------------
   Operações
   ----------------------------------------------------- */

/* Agenda uma nova consulta para um animal já cadastrado. */
void agendar_consulta(Consulta **vetor, int *tamanho, int *capacidade,
                      const Animal *animais, int total_animais) {
    if (*tamanho >= *capacidade) {
        redimensionar_consultas(vetor, capacidade);
    }

    int id_animal;
    printf("\nID do animal para consulta: ");
    if (scanf("%d", &id_animal) != 1) { puts("Entrada invalida."); return; }

    /* Verifica se o animal existe e está ativo */
    int encontrado = 0;
    for (int i = 0; i < total_animais; i++) {
        if (animais[i].id == id_animal && animais[i].ativo) {
            encontrado = 1;
            printf("Animal: %s (%s)\n", animais[i].nome, animais[i].especie);
            break;
        }
    }
    if (!encontrado) {
        printf("Animal ID %d nao encontrado ou removido.\n", id_animal);
        return;
    }

    Consulta nova;
    memset(&nova, 0, sizeof(Consulta));
    nova.id        = proximo_id_consulta(*vetor, *tamanho);
    nova.id_animal = id_animal;
    nova.status    = STATUS_AGENDADA;
    nova.valor     = 0.0f;

    printf("Data (DD/MM/AAAA)  : "); scanf(" %[^\n]", nova.data);
    printf("Hora (HH:MM)       : "); scanf(" %[^\n]", nova.hora);
    printf("Veterinario        : "); scanf(" %[^\n]", nova.veterinario);

    (*vetor)[*tamanho] = nova;
    (*tamanho)++;

    printf("\nConsulta agendada com sucesso! ID: %d\n", nova.id);
}

/* Lista todas as consultas (todos os status). */
void listar_consultas(const Consulta *vetor, int tamanho,
                      const Animal *animais, int total_animais) {
    printf("\n========== Lista de Consultas ==========\n");
    if (tamanho == 0) {
        printf("  Nenhuma consulta registrada.\n");
        return;
    }
    /* Agrupa por status para facilitar leitura */
    for (int s = STATUS_AGENDADA; s <= STATUS_CANCELADA; s++) {
        int subtotal = 0;
        for (int i = 0; i < tamanho; i++) {
            if (vetor[i].status == s) subtotal++;
        }
        if (subtotal == 0) continue;
        printf("\n  [%s - %d registro(s)]\n", status_texto(s), subtotal);
        for (int i = 0; i < tamanho; i++) {
            if (vetor[i].status == s) {
                exibir_consulta(&vetor[i], animais, total_animais);
            }
        }
    }
}

/* Finaliza uma consulta agendada, registrando diagnóstico e valor. */
void finalizar_consulta(Consulta *vetor, int tamanho) {
    int id;
    printf("\nID da consulta a finalizar: ");
    if (scanf("%d", &id) != 1) { puts("Entrada invalida."); return; }

    for (int i = 0; i < tamanho; i++) {
        if (vetor[i].id == id && vetor[i].status == STATUS_AGENDADA) {
            printf("Diagnostico: "); scanf(" %[^\n]", vetor[i].diagnostico);
            printf("Valor (R$) : "); scanf("%f", &vetor[i].valor);
            vetor[i].status = STATUS_REALIZADA;
            printf("Consulta ID %d finalizada com sucesso.\n", id);
            return;
        }
    }
    printf("Consulta ID %d nao encontrada ou nao esta agendada.\n", id);
}

/* Cancela uma consulta que ainda está agendada. */
void cancelar_consulta(Consulta *vetor, int tamanho) {
    int id;
    printf("\nID da consulta a cancelar: ");
    if (scanf("%d", &id) != 1) { puts("Entrada invalida."); return; }

    for (int i = 0; i < tamanho; i++) {
        if (vetor[i].id == id && vetor[i].status == STATUS_AGENDADA) {
            vetor[i].status = STATUS_CANCELADA;
            printf("Consulta ID %d cancelada.\n", id);
            return;
        }
    }
    printf("Consulta ID %d nao encontrada ou nao pode ser cancelada.\n", id);
}

/* Exibe o histórico completo de consultas de um animal específico. */
void historico_animal(const Consulta *vetor, int tamanho,
                      const Animal *animais, int total_animais) {
    int id_animal;
    printf("\nID do animal para historico: ");
    if (scanf("%d", &id_animal) != 1) { puts("Entrada invalida."); return; }

    /* Verifica se o animal existe */
    const char *nome_animal = NULL;
    for (int i = 0; i < total_animais; i++) {
        if (animais[i].id == id_animal) {
            nome_animal = animais[i].nome;
            break;
        }
    }
    if (!nome_animal) {
        printf("Animal ID %d nao encontrado.\n", id_animal);
        return;
    }

    printf("\n  Historico de \"%s\" (ID %d):\n", nome_animal, id_animal);
    int total = 0;
    for (int i = 0; i < tamanho; i++) {
        if (vetor[i].id_animal == id_animal) {
            exibir_consulta(&vetor[i], animais, total_animais);
            total++;
        }
    }
    if (total == 0) printf("  Nenhuma consulta registrada para este animal.\n");
    else            printf("  Total de consultas: %d\n", total);
}
