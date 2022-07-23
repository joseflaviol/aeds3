#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int numeroVertices;
int *coordenadas;
double *matrizAdj;

int leNumeroVertices(const char *nomeArq);
void montaMatrizAdj(const char *nomeArq);

int main(int argc, char **argv) {

    int i, j;

    if (argc < 2) {
        printf("Informe o nome do arquivo\n");
        exit(0);
    }

    montaMatrizAdj(argv[1]);
    
    /*for (i = 0; i < numeroVertices; i++) {
        for (j = 0; j < numeroVertices; j++) {
            printf("%.2lf ", matrizAdj[i * numeroVertices + j]);
        }
        printf("\n");
    }*/

    return 0;

}

int leNumeroVertices(const char *nomeArq) {
    int i;
    int nV = 0;

    for (i = 0; nomeArq[i] != '.'; i++) {
        if (nomeArq[i] > 47 && nomeArq[i] < 58) {
            nV *= 10;
            nV += (nomeArq[i] - 48);
        }
    }

    return nV;
}

void montaMatrizAdj(const char *nomeArq) {
    int i, j, k;
    double peso;
    FILE *arq;
    
    arq = fopen(nomeArq, "r");

    if (arq == NULL) {
        printf("Nao foi possivel abrir o arquivo\n");
        exit(0);
    }

    numeroVertices = leNumeroVertices(nomeArq);

    coordenadas = (int *) malloc(sizeof(int) * numeroVertices * 2);
    matrizAdj = (double *) malloc(sizeof(double) * numeroVertices * numeroVertices);

    while (fscanf(arq, "%d %d %d", &i, &j, &k) != EOF) {
        coordenadas[(i - 1) * 2] = j;
        coordenadas[(i - 1) * 2 + 1] = k;
    } 

    fclose(arq);

    for (i = 0; i < numeroVertices; i++) {
        for (j = i + 1; j < numeroVertices; j++) {
            peso = sqrt( pow(coordenadas[i * 2] - coordenadas[j * 2], 2) + pow( coordenadas[i * 2 + 1] - coordenadas[j * 2 + 1], 2) );
            matrizAdj[i * numeroVertices + j] = peso;
            matrizAdj[j * numeroVertices + i] = peso;
        }
    }
}