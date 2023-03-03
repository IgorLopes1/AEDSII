#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "funcionarios.h"
#include <time.h>


int main(int argc, char** argv) {
    int opcao, chave;
    FILE *out;
    //abre arquivo
    if ((out = fopen("funcionario.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    else {
        cria_funcionarios(out);
        fflush(out);
        do{
            printf("\n\n*******************MENU*******************\n");
            printf("1 - BUSCA SEQUENCIAL.\n");
            printf("2 - BUSCA BINARIA.\n");
            printf("3 - ORDENACAO INSERTION NA MEMORIA. \n");
            printf("4 - ORDENACAO INSERTION NO DISCO.\n");
            printf("5 - IMPRIMIR LISTA DE FUNCIONARIOS.\n");
            printf("6 - INTERCALACAO OTIMA.\n");
            printf("7 - SAIR.\n");
            printf("INFORME SUA OPCAO : ");
            scanf("%d", &opcao);

            switch(opcao){
                case 1 :
                    printf("INFORME A CHAVE : ");
                    scanf("%d", &chave);
                    busca_sequencial(out, chave);
                    break;
                case 2 :
                    printf("INFORME A CHAVE : ");
                    scanf("%d", &chave);
                    busca_binaria(chave, out, 0, TAM_MAX-1);
                    break;
                case 3 :
                    insertion_sort(out);
                    break;
                case 4 :
                    insertion_sort_disco(out, TAM_MAX);
                    break;
                case 5 :
                    imprime_arquivo(out);
                    break;
                case 6 :
                    intercalacao_otima(out, TAM_MAX, out);
                    break;
            }
        }while(opcao != 7);
    }
     fclose(out);
}
