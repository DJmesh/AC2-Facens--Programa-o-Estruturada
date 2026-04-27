# 🐾 Sistema de Gerenciamento — Clínica Veterinária

> **Projeto AC2 — Programação Estruturada**  
> Professora: Tiemi Christine Sakata  
> **Dupla:** Eduardo & Paulo  

---

## 📋 Descrição do Projeto

Este sistema foi desenvolvido em linguagem **C** como parte da avaliação AC2 da disciplina de Programação Estruturada. O sistema gerencia as operações de uma **Clínica Veterinária**, permitindo o cadastro de animais e o controle de consultas médicas.

O projeto foi construído seguindo uma **arquitetura modular**, com separação clara de responsabilidades entre os módulos, uso de **alocação dinâmica de memória**, **ponteiros duplos (`**`)** e **persistência de dados em arquivos binários**.

---

## 👥 Equipe

| Nome     | Turma                    |
|----------|--------------------------|
| Eduardo  | Programação Estruturada  |
| Paulo    | Programação Estruturada  |

---

## 🗂️ Estrutura de Arquivos

```
AC2_Clinica/
├── main.c           → Menu principal, inicialização e encerramento
├── animal.h         → Definição das structs Tutor e Animal + protótipos
├── animal.c         → Implementação do CRUD de animais
├── consulta.h       → Definição da struct Consulta + protótipos
├── consulta.c       → Implementação das operações de consultas
├── arquivo.h        → Protótipos do módulo de arquivos binários
├── arquivo.c        → Leitura e escrita nos arquivos .bin
├── Makefile         → Script de compilação
├── animais.bin      → Arquivo binário de persistência dos animais (gerado em execução)
└── consultas.bin    → Arquivo binário de persistência das consultas (gerado em execução)
```

---

## ⚙️ Como Compilar e Executar

### Pré-requisito
- GCC instalado (`gcc --version` para verificar)

### Opção 1 — Usando o Makefile (recomendado)
```bash
# Entrar na pasta do projeto
cd ~/Documentos/9\ -\ Semestre/Programação\ Estrturada/AC2_Clinica

# Compilar e executar de uma vez
make run
```

### Opção 2 — Compilação manual com GCC
```bash
# Entrar na pasta do projeto
cd ~/Documentos/9\ -\ Semestre/Programação\ Estrturada/AC2_Clinica

# Compilar
gcc -Wall -Wextra -std=c99 main.c animal.c consulta.c arquivo.c -o clinica

# Executar
./clinica
```

### Outros comandos do Makefile
```bash
make        # apenas compila
make run    # compila e executa
make clean  # remove o executável e os arquivos .bin
```

---

## 🧩 Modelagem das Estruturas (Structs)

### `Tutor` — estrutura aninhada dentro de `Animal`
Representa o dono/responsável pelo animal.
```c
typedef struct {
    char nome[100];
    char telefone[20];
    char cpf[15];
} Tutor;
```

### `Animal` — entidade principal do sistema
Contém dados do animal e do seu tutor (struct aninhada).
```c
typedef struct {
    int   id;
    char  nome[80];
    char  especie[40];   // Ex: Cão, Gato, Coelho
    char  raca[60];
    int   idade;         // em anos
    float peso;          // em kg
    Tutor tutor;         // struct aninhada (Tutor dentro de Animal)
    int   ativo;         // 1 = ativo | 0 = removido (soft delete)
} Animal;
```

### `Consulta` — segunda entidade do sistema
Referencia um Animal pelo `id_animal` e controla o ciclo da consulta.
```c
typedef struct {
    int   id;
    int   id_animal;        // referência ao Animal cadastrado
    char  data[12];         // formato: DD/MM/AAAA
    char  hora[8];          // formato: HH:MM
    char  veterinario[80];
    char  diagnostico[300];
    float valor;
    int   status;           // 0=Agendada | 1=Realizada | 2=Cancelada
} Consulta;
```

---

## 🖥️ Funcionalidades do Menu

```
╔══════════════════════════════════════╗
║    CLINICA VETERINARIA — SISTEMA     ║
╠══════════════════════════════════════╣
║  --- ANIMAIS ---                     ║
║  1  Cadastrar animal                 ║
║  2  Listar animais                   ║
║  3  Buscar animal                    ║
║  4  Atualizar animal                 ║
║  5  Remover animal                   ║
╠══════════════════════════════════════╣
║  --- CONSULTAS ---                   ║
║  6  Agendar consulta                 ║
║  7  Listar consultas                 ║
║  8  Finalizar consulta               ║
║  9  Cancelar consulta                ║
║  10 Historico de um animal           ║
╠══════════════════════════════════════╣
║  0  Salvar e sair                    ║
╚══════════════════════════════════════╝
```

### Detalhamento das operações

| Opção | Operação | Descrição |
|-------|----------|-----------|
| 1 | Cadastrar animal | Insere novo animal com dados do tutor. Expande o vetor automaticamente se necessário. |
| 2 | Listar animais | Exibe todos os animais com status `ativo = 1`. |
| 3 | Buscar animal | Busca sequencial por ID ou por substring do nome. |
| 4 | Atualizar animal | Permite alterar peso, idade e/ou dados do tutor. |
| 5 | Remover animal | Remoção lógica (*soft delete*): marca `ativo = 0` sem apagar o registro. |
| 6 | Agendar consulta | Cria consulta vinculada a um animal existente. Valida se o animal existe. |
| 7 | Listar consultas | Exibe todas as consultas agrupadas por status. |
| 8 | Finalizar consulta | Registra diagnóstico e valor em uma consulta Agendada → muda para Realizada. |
| 9 | Cancelar consulta | Muda o status de Agendada → Cancelada. |
| 10 | Histórico do animal | Lista todas as consultas (qualquer status) de um animal específico. |
| 0 | Salvar e sair | Persiste os dados nos `.bin` e libera toda a memória antes de encerrar. |

---

## 🔧 Requisitos Técnicos Implementados

### ✅ Estruturas (`struct`)
- `Tutor` aninhada dentro de `Animal`
- `Animal` com 9 campos (id, nome, espécie, raça, idade, peso, tutor, ativo)
- `Consulta` com 9 campos (id, id_animal, data, hora, veterinário, diagnóstico, valor, status)

### ✅ Alocação Dinâmica com Ponteiro-para-Ponteiro (`**`)
As funções de inicialização e redimensionamento usam `Animal **` e `Consulta **` para alocar e realocar memória dinamicamente:

```c
// Inicializa vetor dinâmico — usa Animal ** para modificar o ponteiro da chamadora
void inicializar_animais(Animal **vetor, int *tamanho, int *capacidade);

// Dobra a capacidade com realloc — preserva o endereço base via **
void redimensionar_animais(Animal **vetor, int *capacidade);
```

### ✅ Arquivos Binários
Todas as funções obrigatórias são usadas em `arquivo.c`:

| Função | Uso no projeto |
|--------|----------------|
| `fopen` | Abre `animais.bin` e `consultas.bin` para leitura (`"rb"`) ou escrita (`"wb"`) |
| `fwrite` | Grava o vetor completo de structs no arquivo |
| `fread` | Lê o vetor de structs do arquivo para a memória |
| `fseek` | Posiciona o cursor no final do arquivo para medir o tamanho |
| `ftell` | Retorna a posição atual (em bytes) para calcular o número de registros |
| `fclose` | Fecha o arquivo após cada operação |

**Técnica de `fseek` + `ftell`** para calcular registros sem armazenar contador separado:
```c
fseek(fp, 0L, SEEK_END);          // vai para o final
long bytes = ftell(fp);            // captura o tamanho em bytes
fseek(fp, 0L, SEEK_SET);          // volta para o início
int num_registros = bytes / sizeof(Animal);  // calcula quantos registros existem
```

### ✅ Persistência entre Execuções
- Ao iniciar: `carregar_animais()` e `carregar_consultas()` leem os `.bin`
- Ao sair (opção 0): `salvar_animais()` e `salvar_consultas()` gravam os `.bin`
- Os dados são **mantidos entre sessões** sem necessidade de re-cadastrar

### ✅ Controle de Estado (Soft Delete)
- Animais: campo `ativo` (1 = ativo, 0 = removido)
- Consultas: campo `status` (0 = Agendada, 1 = Realizada, 2 = Cancelada)
- Registros removidos são preservados no arquivo para manter o histórico

### ✅ Liberação de Memória
Toda memória alocada é liberada antes do encerramento:
```c
liberar_animais(animais);     // free() no vetor dinâmico de animais
liberar_consultas(consultas); // free() no vetor dinâmico de consultas
```

---

## 📐 Arquitetura Modular

```
main.c
  ├── animal.h / animal.c      (CRUD de animais + memória dinâmica)
  ├── consulta.h / consulta.c  (operações de consultas)
  └── arquivo.h / arquivo.c    (persistência binária)
```

Cada módulo tem responsabilidade única e se comunica através dos cabeçalhos `.h`, seguindo o princípio de separação de responsabilidades.

---

## 🔁 Fluxo de Execução

```
Início
  │
  ├─► inicializar_animais()    → malloc() inicial
  ├─► inicializar_consultas()  → malloc() inicial
  ├─► carregar_animais()       → fopen + fseek + ftell + fread + fclose
  ├─► carregar_consultas()     → fopen + fseek + ftell + fread + fclose
  │
  ├─► [Loop do Menu]
  │     ├─ Opções 1–5: operações sobre animais
  │     └─ Opções 6–10: operações sobre consultas
  │
  ├─► salvar_animais()         → fopen + fwrite + fclose
  ├─► salvar_consultas()       → fopen + fwrite + fclose
  ├─► liberar_animais()        → free()
  └─► liberar_consultas()      → free()
        │
       Fim
```

---

## 🧪 Exemplo de Uso

```
# Cadastrar Rex (Cão Labrador) e agendar uma consulta

Opção 1 → Nome: Rex | Espécie: Cao | Raça: Labrador | Idade: 3 | Peso: 28.5
          Tutor: João Silva | Tel: (11)99999-1111 | CPF: 123.456.789-00

Opção 6 → ID do animal: 1 | Data: 27/04/2026 | Hora: 10:00 | Vet: Dr. Carlos

Opção 8 → ID da consulta: 1
          Diagnóstico: Eczema na orelha - tratado com antibiótico
          Valor: R$ 150,00

Opção 10 → ID: 1
           Exibe histórico completo do Rex com a consulta realizada

Opção 0 → Salva tudo e encerra
```

---

## 📌 Observações para a Apresentação

- **Ponto de entrada**: `main.c` — mostrar o loop do menu e como os módulos são chamados
- **Ponteiro-para-ponteiro**: demonstrar em `inicializar_animais()` e `redimensionar_animais()` em `animal.c`
- **Arquivo binário**: demonstrar `arquivo.c` com `fseek + ftell` para calcular registros
- **Soft delete**: mostrar campo `ativo` em `Animal` e como `listar_animais()` o filtra
- **Persistência**: fechar o programa e reabrir — os dados continuam carregados dos `.bin`
