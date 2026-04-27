/*
 * Sistema de Gerenciamento - Clinica Veterinaria
 *
 * Disciplina  : Programacao Estruturada — AC2
 * Professora  : Tiemi Christine Sakata
 *
 * Contexto    : Gerenciamento de animais e consultas
 *               de uma clinica veterinaria.
 *
 * Tecnologias usadas:
 *   - Estruturas (struct) com aninhamento (Tutor em Animal)
 *   - Vetores alocados dinamicamente com ponteiro-para-ponteiro (**)
 *   - Arquivo binario (.bin) com fopen/fread/fwrite/fseek/ftell/fclose
 *   - Menu textual interativo
 *   - Soft delete para remocao segura de registros
 */

#include <stdio.h>
#include <stdlib.h>
#include "animal.h"
#include "consulta.h"
#include "arquivo.h"

/* Limpa o buffer de entrada para evitar lixo em leituras subsequentes */
static void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Exibe o menu principal e retorna a opcao escolhida */
static int menu_principal(void) {
    int opc;
    printf("\n");
    printf("  ╔══════════════════════════════════════╗\n");
    printf("  ║    CLINICA VETERINARIA — SISTEMA     ║\n");
    printf("  ╠══════════════════════════════════════╣\n");
    printf("  ║  --- ANIMAIS ---                     ║\n");
    printf("  ║  1  Cadastrar animal                 ║\n");
    printf("  ║  2  Listar animais                   ║\n");
    printf("  ║  3  Buscar animal                    ║\n");
    printf("  ║  4  Atualizar animal                 ║\n");
    printf("  ║  5  Remover animal                   ║\n");
    printf("  ╠══════════════════════════════════════╣\n");
    printf("  ║  --- CONSULTAS ---                   ║\n");
    printf("  ║  6  Agendar consulta                 ║\n");
    printf("  ║  7  Listar consultas                 ║\n");
    printf("  ║  8  Finalizar consulta               ║\n");
    printf("  ║  9  Cancelar consulta                ║\n");
    printf("  ║  10 Historico de um animal           ║\n");
    printf("  ╠══════════════════════════════════════╣\n");
    printf("  ║  0  Salvar e sair                    ║\n");
    printf("  ╚══════════════════════════════════════╝\n");
    printf("  Escolha: ");

    if (scanf("%d", &opc) != 1) {
        limpar_buffer();
        return -1;
    }
    limpar_buffer();
    return opc;
}

int main(void) {
    /* Vetores dinâmicos — serão passados por ** às funções */
    Animal  *animais   = NULL;
    Consulta *consultas = NULL;

    int tam_animais    = 0, cap_animais    = 0;
    int tam_consultas  = 0, cap_consultas  = 0;

    /* 1. Inicializa vetores dinâmicos */
    inicializar_animais  (&animais,   &tam_animais,   &cap_animais);
    inicializar_consultas(&consultas, &tam_consultas, &cap_consultas);

    /* 2. Carrega dados persistidos em disco */
    carregar_animais  (&animais,   &tam_animais,   &cap_animais);
    carregar_consultas(&consultas, &tam_consultas, &cap_consultas);

    /* 3. Loop principal do menu */
    int opc;
    while ((opc = menu_principal()) != 0) {
        switch (opc) {
            /* --- Animais --- */
            case 1:
                cadastrar_animal(&animais, &tam_animais, &cap_animais);
                break;
            case 2:
                listar_animais(animais, tam_animais);
                break;
            case 3:
                buscar_animal(animais, tam_animais);
                break;
            case 4:
                atualizar_animal(animais, tam_animais);
                break;
            case 5:
                remover_animal(animais, tam_animais);
                break;

            /* --- Consultas --- */
            case 6:
                agendar_consulta(&consultas, &tam_consultas, &cap_consultas,
                                 animais, tam_animais);
                break;
            case 7:
                listar_consultas(consultas, tam_consultas,
                                 animais, tam_animais);
                break;
            case 8:
                finalizar_consulta(consultas, tam_consultas);
                break;
            case 9:
                cancelar_consulta(consultas, tam_consultas);
                break;
            case 10:
                historico_animal(consultas, tam_consultas,
                                 animais, tam_animais);
                break;

            default:
                printf("  Opcao invalida. Tente novamente.\n");
        }
    }

    /* 4. Salva dados antes de sair */
    salvar_animais  (animais,   tam_animais);
    salvar_consultas(consultas, tam_consultas);

    /* 5. Libera toda a memoria dinamica alocada */
    liberar_animais  (animais);
    liberar_consultas(consultas);

    printf("\nSistema encerrado. Ate logo!\n");
    return 0;
}
