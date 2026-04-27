#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "animal.h"

/* =====================================================
   animal.c — Implementação do módulo de animais.
   Contém: alocação dinâmica com ponteiro-para-ponteiro,
   CRUD completo e utilitários de busca.
   ===================================================== */

/* -----------------------------------------------------
   Gerenciamento de memória dinâmica
   ----------------------------------------------------- */

/*
 * Inicializa o vetor dinâmico de animais.
 * Usa ponteiro-para-ponteiro (Animal **) para alocar
 * memória e atualizar o ponteiro da chamadora.
 */
void inicializar_animais(Animal **vetor, int *tamanho, int *capacidade) {
    *tamanho    = 0;
    *capacidade = 8;
    *vetor = (Animal *) malloc((*capacidade) * sizeof(Animal));
    if (*vetor == NULL) {
        fprintf(stderr, "Erro fatal: nao foi possivel alocar memoria inicial para animais.\n");
        exit(EXIT_FAILURE);
    }
}

/*
 * Dobra a capacidade do vetor usando realloc.
 * Mantém o endereço base seguro via ponteiro-para-ponteiro.
 */
void redimensionar_animais(Animal **vetor, int *capacidade) {
    int nova_cap = (*capacidade) * 2;
    Animal *temp = (Animal *) realloc(*vetor, nova_cap * sizeof(Animal));
    if (temp == NULL) {
        fprintf(stderr, "Erro: falha ao redimensionar vetor de animais.\n");
        return;
    }
    *vetor     = temp;
    *capacidade = nova_cap;
    printf("[Sistema] Capacidade do cadastro ampliada para %d registros.\n", nova_cap);
}

/* Libera toda a memória do vetor de animais. */
void liberar_animais(Animal *vetor) {
    free(vetor);
}

/* -----------------------------------------------------
   Utilitários internos
   ----------------------------------------------------- */

/* Retorna o próximo ID disponível (maior ID existente + 1). */
int proximo_id_animal(const Animal *vetor, int tamanho) {
    int max = 0;
    for (int i = 0; i < tamanho; i++) {
        if (vetor[i].id > max) max = vetor[i].id;
    }
    return max + 1;
}

/*
 * Busca sequencial pelo ID do animal.
 * Retorna o índice no vetor ou -1 se não encontrado.
 */
int encontrar_animal_por_id(const Animal *vetor, int tamanho, int id) {
    for (int i = 0; i < tamanho; i++) {
        if (vetor[i].id == id && vetor[i].ativo) return i;
    }
    return -1;
}

/* Exibe os dados completos de um único animal. */
static void exibir_animal(const Animal *a) {
    const char *status_str = a->ativo ? "Ativo" : "Removido";
    printf("  +--------------------------------------------------+\n");
    printf("  | ID: %-5d %-38s |\n", a->id, status_str);
    printf("  | Nome: %-43s |\n", a->nome);
    printf("  | Especie: %-40s |\n", a->especie);
    printf("  | Raca: %-43s |\n", a->raca);
    printf("  | Idade: %-3d anos  |  Peso: %-6.2f kg             |\n",
           a->idade, a->peso);
    printf("  | Tutor: %-42s |\n", a->tutor.nome);
    printf("  | Tel: %-44s |\n", a->tutor.telefone);
    printf("  | CPF: %-44s |\n", a->tutor.cpf);
    printf("  +--------------------------------------------------+\n");
}

/* -----------------------------------------------------
   Operações CRUD
   ----------------------------------------------------- */

/* Cadastra um novo animal no vetor dinâmico. */
void cadastrar_animal(Animal **vetor, int *tamanho, int *capacidade) {
    if (*tamanho >= *capacidade) {
        redimensionar_animais(vetor, capacidade);
    }

    Animal novo;
    memset(&novo, 0, sizeof(Animal));

    novo.id    = proximo_id_animal(*vetor, *tamanho);
    novo.ativo = 1;

    printf("\n--- Cadastro de Animal ---\n");
    printf("Nome do animal : "); scanf(" %[^\n]", novo.nome);
    printf("Especie        : "); scanf(" %[^\n]", novo.especie);
    printf("Raca           : "); scanf(" %[^\n]", novo.raca);
    printf("Idade (anos)   : "); scanf("%d",       &novo.idade);
    printf("Peso (kg)      : "); scanf("%f",        &novo.peso);

    printf("\n--- Dados do Tutor ---\n");
    printf("Nome do tutor  : "); scanf(" %[^\n]", novo.tutor.nome);
    printf("Telefone       : "); scanf(" %[^\n]", novo.tutor.telefone);
    printf("CPF            : "); scanf(" %[^\n]", novo.tutor.cpf);

    (*vetor)[*tamanho] = novo;
    (*tamanho)++;

    printf("\nAnimal cadastrado com sucesso! ID: %d\n", novo.id);
}

/* Lista todos os animais ativos. */
void listar_animais(const Animal *vetor, int tamanho) {
    printf("\n========== Lista de Animais Cadastrados ==========\n");
    int encontrados = 0;
    for (int i = 0; i < tamanho; i++) {
        if (vetor[i].ativo) {
            exibir_animal(&vetor[i]);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        printf("  Nenhum animal cadastrado.\n");
    } else {
        printf("  Total: %d animal(is) ativo(s).\n", encontrados);
    }
}

/* Busca um animal por ID ou por nome (substring). */
void buscar_animal(const Animal *vetor, int tamanho) {
    int opcao;
    printf("\nBuscar por: 1 - ID  |  2 - Nome: ");
    if (scanf("%d", &opcao) != 1) { puts("Entrada invalida."); return; }

    if (opcao == 1) {
        int id;
        printf("ID do animal: ");
        scanf("%d", &id);
        int idx = encontrar_animal_por_id(vetor, tamanho, id);
        if (idx >= 0) {
            exibir_animal(&vetor[idx]);
        } else {
            printf("Animal com ID %d nao encontrado ou removido.\n", id);
        }
    } else {
        char busca[80];
        printf("Digite parte do nome: ");
        scanf(" %[^\n]", busca);
        int encontrados = 0;
        for (int i = 0; i < tamanho; i++) {
            if (vetor[i].ativo && strstr(vetor[i].nome, busca) != NULL) {
                exibir_animal(&vetor[i]);
                encontrados++;
            }
        }
        if (encontrados == 0) printf("Nenhum animal encontrado com \"%s\".\n", busca);
    }
}

/* Atualiza peso, idade e dados do tutor de um animal existente. */
void atualizar_animal(Animal *vetor, int tamanho) {
    int id;
    printf("\nID do animal para atualizar: ");
    if (scanf("%d", &id) != 1) { puts("Entrada invalida."); return; }

    int idx = encontrar_animal_por_id(vetor, tamanho, id);
    if (idx < 0) {
        printf("Animal com ID %d nao encontrado.\n", id);
        return;
    }

    Animal *a = &vetor[idx];
    int sub;
    printf("\nO que deseja atualizar?\n");
    printf("  1 - Peso\n  2 - Idade\n  3 - Dados do tutor\n  4 - Todos\n");
    printf("Opcao: ");
    scanf("%d", &sub);

    if (sub == 1 || sub == 4) {
        printf("Novo peso (%.2f kg): ", a->peso);
        scanf("%f", &a->peso);
    }
    if (sub == 2 || sub == 4) {
        printf("Nova idade (%d anos): ", a->idade);
        scanf("%d", &a->idade);
    }
    if (sub == 3 || sub == 4) {
        printf("Novo nome do tutor  (%s): ", a->tutor.nome);
        scanf(" %[^\n]", a->tutor.nome);
        printf("Novo telefone (%s): ", a->tutor.telefone);
        scanf(" %[^\n]", a->tutor.telefone);
    }
    printf("Animal ID %d atualizado com sucesso.\n", id);
}

/* Realiza a remoção lógica (soft delete) de um animal. */
void remover_animal(Animal *vetor, int tamanho) {
    int id;
    printf("\nID do animal para remover: ");
    if (scanf("%d", &id) != 1) { puts("Entrada invalida."); return; }

    int idx = encontrar_animal_por_id(vetor, tamanho, id);
    if (idx < 0) {
        printf("Animal com ID %d nao encontrado.\n", id);
        return;
    }

    char confirm;
    printf("Confirma remocao de \"%s\"? (s/n): ", vetor[idx].nome);
    scanf(" %c", &confirm);
    if (confirm == 's' || confirm == 'S') {
        vetor[idx].ativo = 0;
        printf("Animal removido com sucesso (registro mantido no historico).\n");
    } else {
        printf("Remocao cancelada.\n");
    }
}
