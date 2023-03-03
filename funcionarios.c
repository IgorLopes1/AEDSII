#include "funcionarios.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

// Imprime funcionario
void imprime(TFunc *func) {
    printf("\n\n**********************************************");
    printf("\nFuncionario de codigo ");
    printf("%d", func->cod);
    printf("\nNome: ");
    printf("%s", func->nome);
    printf("\nCPF: ");
    printf("%s", func->cpf);
    printf("\nData de Nascimento: ");
    printf("%s", func->data_nascimento);
    printf("\nSalario: ");
    printf("%4.2f", func->salario);
    printf("\n**********************************************\n\n");
}

// Cria funcionario. Lembrar de usar free(func)
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario) {
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    //inicializa espaço de memória com ZEROS
    if (func) memset(func, 0, sizeof(TFunc));
    //copia valores para os campos de func
    func->cod = cod;
    strcpy(func->nome, nome);
    strcpy(func->cpf, cpf);
    strcpy(func->data_nascimento, data_nascimento);
    func->salario = salario;
    return func;
}

// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out) {
    fwrite(&func->cod, sizeof(int), 1, out);
    //func->nome ao invés de &func->nome, pois string já é ponteiro
    fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
    fwrite(func->cpf, sizeof(char), sizeof(func->cpf), out);
    fwrite(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), out);
    fwrite(&func->salario, sizeof(double), 1, out);
}

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le(FILE *in) {
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    if (0 >= fread(&func->cod, sizeof(int), 1, in)) {
        free(func);
        return NULL;
    }
    fread(func->nome, sizeof(char), sizeof(func->nome), in);
    fread(func->cpf, sizeof(char), sizeof(func->cpf), in);
    fread(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), in);
    fread(&func->salario, sizeof(double), 1, in);
    return func;
}


// Retorna tamanho do funcionario em bytes
int tamanho() {
    return sizeof(int)  //cod
           + sizeof(char) * 50 //nome
           + sizeof(char) * 15 //cpf
           + sizeof(char) * 11 //data_nascimento
           + sizeof(double); //salario
}
 void cria_funcionarios(FILE *out) {
    printf("Inserindo funcionarios no arquivo...");
    int cod_usados[TAM_MAX] = {0};
    srand(time(NULL));
    TFunc *func = (TFunc*) malloc(TAM_MAX * sizeof(TFunc));
    for(int i = 0; i < TAM_MAX; i++){
        int cod = rand()%TAM_MAX;
        while(cod_usados[cod] == 1){
            cod = rand()%TAM_MAX;
        }
        cod_usados[cod] = 1;
        func[i].cod = cod;
        func[i].salario = rand()%20000;
        sprintf(func[i].nome, "Funcionario %d", func[i].cod);
        sprintf(func[i].cpf, "xxx.xxx.xxx-xx");
        sprintf(func[i].data_nascimento, "xx/xx/xxxx");
    }
    for (int i = 0; i < TAM_MAX; i++){
        TFunc *f = funcionario(func[i].cod,func[i].nome, func[i].cpf, func[i].data_nascimento, func[i].salario);
        salva(f, out);
        free(f);
    }
}

void imprime_arquivo(FILE *arq) {
    //le o arquivo e coloca no vetor
    rewind(arq); //posiciona cursor no inicio do arquivo
    TFunc *f = le(arq);
    while (!feof(arq)) {
        imprime(f);
        f = le(arq);
    }
}

void busca_binaria(int chave, FILE* arq, int inicio, int fim) {
    clock_t start, end;
    double tempo_exc;
    TFunc* f = NULL;
    int cod = -1, qtd_int = 0;
    start = clock();
    while (inicio <= fim && cod != chave) {
        qtd_int++;
        int meio = (inicio + fim) / 2;
        printf("Inicio: %d; Meio: %d; Fim: %d\n", inicio, meio, fim);
        fseek(arq, meio * sizeof(TFunc), SEEK_SET);
        f = le(arq);
        if (f) {
            cod = f->cod;
            if (cod > chave) {
                fim = meio - 1;
            } else {
                inicio = meio + 1;
            }
        }
    }
    end = clock();
    if (f && cod == chave) {
        imprime(f);
        tempo_exc = (double)(end - start) / CLOCKS_PER_SEC;
        printf("\nTempo de execucao : %lf segundos", tempo_exc);
        printf("\nQuantidade de comparacoes : %d", qtd_int);
        FILE *arq_bin = fopen("buscabinaria.dat", "wb");
        salva(f, arq_bin);
        fwrite(&tempo_exc, sizeof(double), 1, arq_bin);
        fwrite(&qtd_int, sizeof(int), 1, arq_bin);
        fclose(arq_bin);
    } else {
        if (f) {
            free(f);
        }
        printf("Funcionario inexistente!\n");
    }
}

void insertion_sort_disco(FILE *arq, int tam) {
    TFunc *funcionarios = (TFunc *) malloc(tam * sizeof(TFunc));
    int i, j;
    TFunc chave;
    clock_t start, end;
    double tempo_exc;
    int qtd_int = 0;
    // Lê os dados do arquivo e armazena no vetor
    rewind(arq);
    for (i = 0; i < tam; i++) {
        funcionarios[i] = *le(arq);
    }
    start = clock();
    // Ordena o vetor por código
    for (i = 1; i < tam; i++) {
        chave = funcionarios[i];
        j = i - 1;
        while (j >= 0 && funcionarios[j].cod > chave.cod) {
            qtd_int++;
            funcionarios[j + 1] = funcionarios[j];
            printf("Funcionario %d foi armazenado na posicao %d\n", funcionarios[j].cod, j + 1);
            j--;
        }
        funcionarios[j + 1] = chave;
        printf("Funcionario %d foi armazenado na posicao %d\n", chave.cod, j + 1);
    }
    end = clock();
    // Reescreve os dados ordenados no arquivo
    rewind(arq);
    for (i = 0; i < tam; i++) {
        salva(&funcionarios[i], arq);
    }
    FILE* arq_isrt = fopen("insertion.dat", "w+b");
    tempo_exc = (double) (end - start) / CLOCKS_PER_SEC;
    printf("\nTempo de execucao : %lf segundos", tempo_exc);
    printf("\nQuantidade de comparacoes : %d", qtd_int);
    fwrite(&tempo_exc, sizeof(double), 1, arq_isrt);
    fwrite(&qtd_int, sizeof(int), 1, arq_isrt);

    free(funcionarios);
    fclose(arq_isrt);
}


void busca_sequencial(FILE* out, int chave) {
    clock_t start, end;
    double tempo_exc;
    TFunc *f = (TFunc*) malloc(TAM_MAX * sizeof(TFunc));
    FILE *arq_seq = fopen("buscasequencial.dat", "w+b");
    rewind(out); // posiciona o ponteiro no início do arquivo
    int qtd_int = 0;

    start = clock();
    while (fread(f, sizeof(TFunc), 1, out) == 1) {
        if (f->cod == chave) {
            imprime(f);
            salva(f, arq_seq);
            free(f);

            break;
        }
        qtd_int++;
    }
    end = clock();
    tempo_exc = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTempo de execucao : %lf segundos", tempo_exc);
    printf("\nQuantidade de comparacoes : %d", qtd_int);
    fwrite(&tempo_exc, sizeof(double), 1, arq_seq);
    fwrite(&qtd_int, sizeof(int), 1, arq_seq);
}

void insertion_sort(FILE *arq) {
    TFunc *v[TAM_MAX - 1];
    //le o arquivo e coloca no vetor
    rewind(arq); //posiciona cursor no inicio do arquivo
    TFunc *f = le(arq);
    int i = 0;
    while (!feof(arq)) {
        v[i] = f;
        f = le(arq);
        i++;
    }
    //faz o insertion sort
    for (int j = 1; j < TAM_MAX; j++) {
        TFunc *f = v[j];
        i = j - 1;
        while ((i >= 0) && (v[i]->cod > f->cod)) {
            v[i + 1] = v[i];
            i = i - 1;
        }
        v[i+1] = f;
    }
    //grava vetor no arquivo, por cima do conteÃºdo anterior
    rewind(arq);
    for (int i = 0; i < TAM_MAX; i++) {
        salva(v[i], arq);
    }
    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);

}
int compara_cod(const void *a, const void *b) {
    TFunc *elem1 = (TFunc*)a;
    TFunc *elem2 = (TFunc*)b;
    return (elem1->cod - elem2->cod);
}


void intercala(TFunc *v, int inicio, int meio, int fim) {
    int posicao = 0, i = inicio, j = meio + 1;
    TFunc *aux = malloc((fim - inicio + 1) * sizeof(TFunc));

    while (i <= meio && j <= fim) {
        if (v[i].cod < v[j].cod) {
            aux[posicao++] = v[i++];
        } else {
            aux[posicao++] = v[j++];
        }
    }

    while (i <= meio) {
        aux[posicao++] = v[i++];
    }

    while (j <= fim) {
        aux[posicao++] = v[j++];
    }

    for (i = inicio; i <= fim; i++) {
        v[i] = aux[i - inicio];
    }

    free(aux);
}

void intercalacao_otima(FILE *arq_entrada, int tam_max, FILE *arq_saida) {
    clock_t start = clock();
    double tempo_exc;

    // Variáveis para controle de arquivos temporários
    int num_arquivos = 0;
    char nome_arq[20];

    // Buffers para leitura e escrita dos registros
    TFunc *buffer = malloc(tam_max * sizeof(TFunc));
    TFunc *saida = malloc(tam_max * sizeof(TFunc));

    // Array de ponteiros para arquivos temporários
    FILE **arquivos = malloc(sizeof(FILE *));

    // Loop para leitura e ordenação dos blocos de registros do arquivo de entrada
    while (!feof(arq_entrada)) {
        // Criação de um novo arquivo temporário
        sprintf(nome_arq, "temp%d.dat", num_arquivos);
        FILE *arq_temp = fopen(nome_arq, "w+b");
        if (arq_temp == NULL) {
            printf("Erro ao criar arquivo temporario.\n");
            exit(1);
        }
        arquivos[num_arquivos] = arq_temp;
        num_arquivos++;

        // Leitura de um bloco de registros
        int tam_buffer = fread(buffer, sizeof(TFunc), tam_max, arq_entrada);
        // Cálculo do número de blocos
        int num_blocos = tam_buffer / tam_max;
        if (tam_buffer % tam_max != 0) {
            num_blocos++;
        }
        // Ordenação e escrita dos blocos no arquivo temporário
        for (int i = 0; i < num_blocos; i++) {
            int inicio = i * tam_max;
            int fim = (i + 1) * tam_max - 1;
            if (fim >= tam_buffer) {
                fim = tam_buffer - 1;
            }

            qsort(&buffer[inicio], fim - inicio + 1, sizeof(TFunc), compara_cod);

            fwrite(&buffer[inicio], sizeof(TFunc), fim - inicio + 1, arq_temp);
        }
    }

    // Loop para intercalação dos arquivos temporários
    while (num_arquivos > 1) {
        int i, j;
        for (i = 0, j = 0; j < num_arquivos; i++, j += 2) {
            if (j + 1 < num_arquivos) {
                FILE *arq_saida = arquivos[i];
                FILE *arq_esq = arquivos[j];
                FILE *arq_dir = arquivos[j+1];

                // Leitura dos blocos dos arquivos esquerdo e direito
                int tam_esq = fread(buffer, sizeof(TFunc), tam_max, arq_esq);
                int tam_dir = fread(&buffer[tam_esq], sizeof(TFunc), tam_max, arq_dir);
                int pos_esq = 0, pos_dir = 0, pos_saida = 0;
                // Loop para intercalação dos registros dos arquivos esquerdo e direito
                while (pos_esq < tam_esq && pos_dir < tam_dir) {
                    if (compara_cod(&buffer[pos_esq], &buffer[tam_esq + pos_dir]) < 0) {
                        saida[pos_saida++] = buffer[pos_esq++];
                    } else {
                        saida[pos_saida++] = buffer[tam_esq + pos_dir++];
                }

                    if (pos_saida == tam_max) {
                        fwrite(saida, sizeof(TFunc), tam_max, arq_saida);
                        pos_saida = 0;
                }
            }
                // Copia dos registros restantes do arquivo esquerdo
                while (pos_esq < tam_esq) {
                    saida[pos_saida++] = buffer[pos_esq++];

                    if (pos_saida == tam_max) {
                        fwrite(saida, sizeof(TFunc), tam_max, arq_saida);
                        pos_saida = 0;
                    }
                }
                // Copia dos registros restantes do arquivo direito
                while (pos_dir < tam_dir) {
                    saida[pos_saida++] = buffer[tam_esq + pos_dir++];

                    if (pos_saida == tam_max) {
                        fwrite(saida, sizeof(TFunc), tam_max, arq_saida);
                        pos_saida = 0;
                    }
                }

                // Escrita dos registros intercalados no arquivo de saída
                if (pos_saida > 0) {
                    fwrite(saida, sizeof(TFunc), pos_saida, arq_saida);
                }

                // Posicionamento dos ponteiros dos arquivos esquerdo e direito no início
                rewind(arq_esq);
                rewind(arq_esq);
                rewind(arq_dir);

                int pos = 0;
                 // Intercalação dos blocos dos arquivos esquerdo e direito
                while (!feof(arq_esq)) {
                    int tam_buffer = fread(buffer, sizeof(TFunc), tam_max, arq_esq);
                    if (tam_buffer > 0) {
                        // Ordenação do bloco lido do arquivo esquerdo
                        qsort(buffer, tam_buffer, sizeof(TFunc), compara_cod);
                        // Escrita do bloco ordenado no arquivo temporário correspondente ao arquivo esquerdo
                        fwrite(buffer, sizeof(TFunc), tam_buffer, arquivos[j]);

                         // Verificação se o bloco lido é menor que o tamanho máximo e se sua soma com o número de registros já lidos não ultrapassa o tamanho máximo
                        if (tam_buffer < tam_max && pos + tam_buffer < tam_max) {
                            fseek(arquivos[j], 0, SEEK_END);
                            // Escrita dos registros excedentes do bloco lido no final do arquivo esquerdo
                            fwrite(&buffer[tam_buffer], sizeof(TFunc), tam_max - tam_buffer, arquivos[j]);
                            break;
                        } else {
                            pos += tam_buffer;
                        }
                    }
                }

                pos = 0;
                while (!feof(arq_dir)) {
                    int tam_buffer = fread(buffer, sizeof(TFunc), tam_max, arq_dir);
                    if (tam_buffer > 0) {

                        // Ordenação do bloco lido do arquivo direito
                        qsort(buffer, tam_buffer, sizeof(TFunc), compara_cod);
                        // Escrita do bloco ordenado no arquivo temporário correspondente ao arquivo direito
                        fwrite(buffer, sizeof(TFunc), tam_buffer, arquivos[j+1]);

                        // Verificação se o bloco lido é menor que o tamanho máximo e se sua soma com o número de registros já lidos não ultrapassa o tamanho máximo
                        if (tam_buffer < tam_max && pos + tam_buffer < tam_max) {
                            fseek(arquivos[j+1], 0, SEEK_END);
                            // Escrita dos registros excedentes do bloco lido no final do arquivo direito
                            fwrite(&buffer[tam_buffer], sizeof(TFunc), tam_max - tam_buffer, arquivos[j+1]);
                            break;
                        } else {
                            pos += tam_buffer;
                        }
                    }
                }
            } else {
                 // Cópia do arquivo sobrevivente para a posição do arquivo intercalado
                arquivos[i] = arquivos[j];
            }
        }
        num_arquivos = (num_arquivos + 1) / 2;
}

    fclose(arquivos[0]);
    // Renomear o arquivo temporário para o nome original do arquivo de entrada
    rename(nome_arq, "funcionario.dat");

    free(arquivos);
    free(buffer);
    free(saida);

    clock_t end = clock();
    tempo_exc = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTempo de execucao : %lf segundos", tempo_exc);
}
