#include <stdio.h>
#include <stdlib.h>
#include "arquivo.h"

/* =====================================================
   arquivo.c — Módulo de persistência em arquivos binários.

   Funções obrigatórias utilizadas:
     fopen  — abrir arquivo para leitura/escrita
     fwrite — gravar structs no arquivo
     fread  — ler structs do arquivo
     fseek  — posicionar o cursor no arquivo
     ftell  — obter posição atual (para calcular tamanho)
     fclose — fechar arquivo e liberar recurso
   ===================================================== */

/* =====================================================
   Animais
   ===================================================== */

/*
 * Salva todo o vetor de animais no arquivo binário.
 * Sobrescreve completamente o arquivo a cada chamada.
 */
void salvar_animais(const Animal *vetor, int tamanho) {
    FILE *fp = fopen(ARQUIVO_ANIMAIS, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s' para escrita.\n",
                ARQUIVO_ANIMAIS);
        return;
    }

    /* Grava todos os registros de uma vez */
    size_t gravados = fwrite(vetor, sizeof(Animal), (size_t)tamanho, fp);
    if ((int)gravados != tamanho) {
        fprintf(stderr, "Aviso: esperado gravar %d registros, gravados %zu.\n",
                tamanho, gravados);
    }

    fclose(fp);
    printf("[Arquivo] %d animal(is) salvo(s) em '%s'.\n",
           tamanho, ARQUIVO_ANIMAIS);
}

/*
 * Carrega animais do arquivo binário para o vetor dinâmico.
 * Usa fseek + ftell para descobrir quantos registros existem
 * sem precisar armazenar o tamanho separadamente.
 */
void carregar_animais(Animal **vetor, int *tamanho, int *capacidade) {
    FILE *fp = fopen(ARQUIVO_ANIMAIS, "rb");
    if (fp == NULL) {
        /* Arquivo ainda não existe — situação normal na primeira execução */
        printf("[Arquivo] '%s' nao encontrado. Iniciando com cadastro vazio.\n",
               ARQUIVO_ANIMAIS);
        return;
    }

    /* Determina o número de registros via tamanho do arquivo */
    fseek(fp, 0L, SEEK_END);
    long tamanho_bytes = ftell(fp);
    fseek(fp, 0L, SEEK_SET);   /* reposiciona no início para leitura */

    int num_registros = (int)(tamanho_bytes / (long)sizeof(Animal));

    if (num_registros <= 0) {
        fclose(fp);
        return;
    }

    /* Amplia o vetor dinâmico se necessário */
    if (num_registros > *capacidade) {
        int nova_cap = num_registros + 8;
        Animal *temp = (Animal *) realloc(*vetor, nova_cap * sizeof(Animal));
        if (temp == NULL) {
            fprintf(stderr, "Erro: falha ao realocar memoria ao carregar animais.\n");
            fclose(fp);
            return;
        }
        *vetor      = temp;
        *capacidade = nova_cap;
    }

    /* Lê todos os registros */
    size_t lidos = fread(*vetor, sizeof(Animal), (size_t)num_registros, fp);
    *tamanho = (int)lidos;

    fclose(fp);
    printf("[Arquivo] %d animal(is) carregado(s) de '%s'.\n",
           *tamanho, ARQUIVO_ANIMAIS);
}

/* =====================================================
   Consultas
   ===================================================== */

/* Salva todo o vetor de consultas no arquivo binário. */
void salvar_consultas(const Consulta *vetor, int tamanho) {
    FILE *fp = fopen(ARQUIVO_CONSULTAS, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s' para escrita.\n",
                ARQUIVO_CONSULTAS);
        return;
    }

    size_t gravados = fwrite(vetor, sizeof(Consulta), (size_t)tamanho, fp);
    if ((int)gravados != tamanho) {
        fprintf(stderr, "Aviso: esperado gravar %d consultas, gravadas %zu.\n",
                tamanho, gravados);
    }

    fclose(fp);
    printf("[Arquivo] %d consulta(s) salva(s) em '%s'.\n",
           tamanho, ARQUIVO_CONSULTAS);
}

/*
 * Carrega consultas do arquivo binário para o vetor dinâmico.
 * Usa fseek + ftell para dimensionar corretamente o vetor.
 */
void carregar_consultas(Consulta **vetor, int *tamanho, int *capacidade) {
    FILE *fp = fopen(ARQUIVO_CONSULTAS, "rb");
    if (fp == NULL) {
        printf("[Arquivo] '%s' nao encontrado. Iniciando com agenda vazia.\n",
               ARQUIVO_CONSULTAS);
        return;
    }

    fseek(fp, 0L, SEEK_END);
    long tamanho_bytes = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    int num_registros = (int)(tamanho_bytes / (long)sizeof(Consulta));

    if (num_registros <= 0) {
        fclose(fp);
        return;
    }

    if (num_registros > *capacidade) {
        int nova_cap = num_registros + 8;
        Consulta *temp = (Consulta *) realloc(*vetor, nova_cap * sizeof(Consulta));
        if (temp == NULL) {
            fprintf(stderr, "Erro: falha ao realocar memoria ao carregar consultas.\n");
            fclose(fp);
            return;
        }
        *vetor      = temp;
        *capacidade = nova_cap;
    }

    size_t lidos = fread(*vetor, sizeof(Consulta), (size_t)num_registros, fp);
    *tamanho = (int)lidos;

    fclose(fp);
    printf("[Arquivo] %d consulta(s) carregada(s) de '%s'.\n",
           *tamanho, ARQUIVO_CONSULTAS);
}
