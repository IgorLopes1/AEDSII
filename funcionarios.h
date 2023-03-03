#ifndef FUNCIONARIOS_H_INCLUDED
#define FUNCIONARIOS_H_INCLUDED
#define TAM_MAX 100
#include <stdlib.h>
#include <stdio.h>

typedef struct Funcionario {
    int cod;
    char nome[50];
    char cpf[15];
    char data_nascimento[11];
    double salario;
} TFunc;

// Imprime funcionario
void imprime(TFunc *func);

// Cria funcionario. Lembrar de usar free(funcionario)
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario);

// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out);

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le(FILE *in);

// Retorna tamanho do funcionario em bytes
int tamanho();

void cria_funcionarios(FILE *out);

void imprime_arquivo(FILE *arq);

void busca_binaria(int chave, FILE* arq, int inicio, int fim);

void insertion_sort_disco(FILE *arq, int tam);

void busca_sequencial(FILE* out, int chave);

void intercalacao(FILE *f1, FILE *f2, FILE *saida);

void intercalacao_otima(FILE *arq_entrada, int tam_max, FILE *arq_saida);

void insertion_sort(FILE *arq);

#endif // FUNCIONARIOS_H_INCLUDED
