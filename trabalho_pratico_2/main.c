#include <stdio.h>
#include <stdlib.h>

int **matrizAdj;
int **mFloyd;
int **predecessores;

void iniciaMatrizes(int numeroVertices);
void montaMatriz(char *nomeArq, int *numeroVertices);
void floydWarshall(int numeroVertices);
void exibeCaminho(int inicio, int destino);

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Informe o nome do arquivo\n");
        exit(0);
    } 

    int numeroVertices;

    printf("%s\n", argv[1]);

    montaMatriz(argv[1], &numeroVertices);
    floydWarshall(numeroVertices);

    return 0;

}

void iniciaMatrizes(int numeroVertices) {
    int i;

    matrizAdj = (int **) calloc(numeroVertices, sizeof(int *));
    mFloyd = (int **) calloc(numeroVertices, sizeof(int *));
    predecessores = (int **) calloc(numeroVertices, sizeof(int *));

    for (i = 0; i < numeroVertices; i++) {
        matrizAdj[i] = (int *) calloc(numeroVertices, sizeof(int));
        mFloyd[i] = (int *) calloc(numeroVertices, sizeof(int));
        predecessores[i] = (int *) calloc(numeroVertices, sizeof(int));
    }
}

void montaMatriz(char *nomeArq, int *numeroVertices) {
    int i, j, peso;
    FILE *arq = fopen(nomeArq, "r");

    if (arq == NULL) {
        printf("Impossivel abrir arquivo\n");
        exit(0);
    }

    fscanf(arq, "%d", numeroVertices);

    iniciaMatrizes(*numeroVertices);

    while (fscanf(arq, "%d %d %d", &i, &j, &peso) != EOF) {
        matrizAdj[i][j] = peso;
        matrizAdj[j][i] = peso;
    }

}

void floydWarshall(int numeroVertices) {
    int i, j, k;
    int maiorJ, maiorK, maiorPeso;

    for (i = 0; i < numeroVertices; i++) {
        for (j = i + 1; j < numeroVertices; j++) {
            mFloyd[i][j] = i == j ? 0 : matrizAdj[i][j] == 0 ? 10000 : matrizAdj[i][j];
            mFloyd[j][i] = mFloyd[i][j]; 
            predecessores[i][j] = i;
            predecessores[j][i] = j;
        }
    }
    
    maiorPeso = 0;
    for (i = 0; i < numeroVertices; i++) {
        for (j = 0; j < numeroVertices; j++) {
            for (k = j + 1; k < numeroVertices; k++) {
                if (mFloyd[j][i] + mFloyd[i][k] < mFloyd[j][k]) {
                    mFloyd[j][k] = mFloyd[j][i] + mFloyd[i][k];
                    mFloyd[k][j] = mFloyd[j][k];
                    predecessores[j][k] = predecessores[i][k];
                    predecessores[k][j] = predecessores[i][j];
                }
                if (i + 1 == numeroVertices && mFloyd[j][k] > maiorPeso) {
                    maiorJ = j;
                    maiorK = k;
                    maiorPeso = mFloyd[j][k];
                } 
            }
        }
    }

    exibeCaminho(maiorJ, maiorK);
    printf("\n%d\n", maiorPeso);

}

void exibeCaminho(int inicio, int destino) {
    if (inicio != destino) {
        exibeCaminho(inicio, predecessores[inicio][destino]);
    }
    printf("%d ", destino);
}