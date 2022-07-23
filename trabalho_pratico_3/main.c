#include <stdio.h>
#include <stdlib.h>

int numeroVertices;
int **coordenadas;
double **matrizAdj;

int leNumeroVertices(FILE *arq);
void montaMatrizAdj(char *nomeArq);

int main(int argc, char **argv) {

    int i, j;

    if (argc < 2) {
        printf("Informe o nome do arquivo\n");
        exit(0);
    }

    montaMatrizAdj(argv[1]);

    for (i = 0; i < numeroVertices; i++) {
        for (j = 0; j < numeroVertices; j++) {
            printf("%d ", matrizAdj[i][j]);
        }
        printf("\n");
    }

    return 0;

}

int leNumeroVertices(FILE *arq) {
    int i, x, y;
    int cont = 0;

    while (fscanf(arq, "%d %d %d", &i, &x, &y) != EOF) {
        cont++;
    } 

    return cont;
}

void montaMatrizAdj(char *nomeArq) {
    int i, j, k;
    double peso;
    FILE *arq;
    
    arq = fopen(nomeArq, "r");

    if (arq == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");
        exit(0);
    }

    numeroVertices = leNumeroVertices(arq);

    coordenadas = (int **) malloc(sizeof(int *) * numeroVertices);
    matrizAdj = (double **) malloc(sizeof(double *) * numeroVertices);

    for (i = 0; i < numeroVertices; i++) {
        coordenadas[i] = (int *) malloc(sizeof(int) * 2);
        matrizAdj[i] = (double *) malloc(sizeof(double) * numeroVertices);
    }

    while (fscanf(arq, "%d %d %d", &i, &j, &k) != EOF) {
        coordenadas[i][0] = j;
        coordenadas[i][1] = k;
    } 

    for (i = 0; i < numeroVertices; i++) {
        for (j = i + 1; j < numeroVertices; j++) {
            peso = sqrt( pow(coordenadas[i][0] - coordenadas[j][0], 2) + pow( coordenadas[i][1] - coordenadas[j][1], 2) );
            matrizAdj[i][j] = peso;
            matrizAdj[j][i] = peso;
        }
    }
}