#ifndef ANIMAL_H
#define ANIMAL_H

/* =====================================================
   animal.h — Definição das estruturas e protótipos
   do módulo de gerenciamento de animais.
   ===================================================== */

/* Estrutura aninhada: dados do tutor (dono) do animal */
typedef struct {
    char nome[100];
    char telefone[20];
    char cpf[15];
} Tutor;

/* Estrutura principal do animal */
typedef struct {
    int   id;
    char  nome[80];
    char  especie[40];   /* Ex: Cao, Gato, Coelho, Ave */
    char  raca[60];
    int   idade;         /* em anos */
    float peso;          /* em kg */
    Tutor tutor;         /* struct aninhada */
    int   ativo;         /* 1 = ativo | 0 = removido (soft delete) */
} Animal;

/* --- Gerenciamento de memória dinâmica --- */
void inicializar_animais(Animal **vetor, int *tamanho, int *capacidade);
void redimensionar_animais(Animal **vetor, int *capacidade);
void liberar_animais(Animal *vetor);

/* --- Operações CRUD --- */
void cadastrar_animal(Animal **vetor, int *tamanho, int *capacidade);
void listar_animais(const Animal *vetor, int tamanho);
void buscar_animal(const Animal *vetor, int tamanho);
void atualizar_animal(Animal *vetor, int tamanho);
void remover_animal(Animal *vetor, int tamanho);

/* --- Utilitários --- */
int proximo_id_animal(const Animal *vetor, int tamanho);
int encontrar_animal_por_id(const Animal *vetor, int tamanho, int id);

#endif /* ANIMAL_H */
