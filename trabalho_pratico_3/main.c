#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define TAM_POPULACAO 100
#define GERACOES 10000

int numeroVertices;
int *coordenadas;
double *matrizAdj;
int solucoes[TAM_POPULACAO][5];

int leNumeroVertices(const char *nomeArq);
void montaMatrizAdj(const char *nomeArq);
bool contido(int i, int v, int n);
void geraAleatorios();
int left(int i, int j);
int right(int i, int j);
double calculaPeso(int index);
int seleciona();
void combina(int s1, int s2);
int maior();
void exibe(); 

int main(int argc, char **argv) {

    int i, j, idxMaior;

    if (argc < 2) {
        printf("Informe o nome do arquivo\n");
        exit(0);
    }

    montaMatrizAdj(argv[1]);
    
    srand(1);
    
    geraAleatorios();
    
    printf("---------------Geracao 0------------\n");
    //exibe();
    idxMaior = maior();

    for (i = 0; i < 5; i++) {
        printf("%d ", solucoes[idxMaior][i]);
    }

    printf("\n%.2f\n", calculaPeso(idxMaior));
    printf("----------------------------------\n");
    
    for (i = 0; i < GERACOES; i++) {
        for (j = 0; j < TAM_POPULACAO; j++) {
            int s1 = seleciona();
            int s2 = seleciona();
            combina(s1, s2);
        }
        
        printf("---------------Geracao %d------------\n", i + 1);
        //exibe();
        idxMaior = maior();

        for (j = 0; j < 5; j++) {
            printf("%d ", solucoes[idxMaior][j]);
        }

        printf("\n%.2f\n", calculaPeso(idxMaior));
        printf("----------------------------------\n");
    }

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

bool contido(int i, int v, int n) {
    int j;
    for (j = 0; j < n; j++) {
        if (v == solucoes[i][j]) {
            return true;
        }
    }

    return false;
}

void geraAleatorios() {
    int i, j, v;
    for (i = 0; i < TAM_POPULACAO; i++) {
        for (j = 0; j < 5; j++) {
            v = rand() % numeroVertices;
            while (contido(i, v, j)) {
                v = rand() % numeroVertices;
            }
            solucoes[i][j] = v;
        }
    }
}

int left(int i, int j) {
    return solucoes[i][2 * j + 1];
}

int right(int i, int j) {
    return solucoes[i][2 * j + 2];
}

double calculaPeso(int index) {
    int i;
    double soma = 0;

    for ( i = 0; i < 2; i++ ){
        soma += matrizAdj[solucoes[index][i] * numeroVertices + left(index, i)];
        soma += matrizAdj[solucoes[index][i] * numeroVertices + right(index, i)];
    }

    return soma;
}

double distancia(int s1, int s2) {
    double soma = 0;

    for (int i = 0; i < 5; i++) {
        soma += pow(solucoes[s1][i] - solucoes[s2][i], 2);
    }

    return sqrt(soma);
}

int seleciona() {
    int s1 = rand() % TAM_POPULACAO;
    int s2 = rand() % TAM_POPULACAO;
    int s3 = rand() % TAM_POPULACAO;

    double ps1 = calculaPeso(s1);
    double ps2 = calculaPeso(s2);
    double ps3 = calculaPeso(s3);

    double ds1s2 = distancia(s1, s2);
    double ds1s3 = distancia(s1, s3);
    double ds2s3 = distancia(s2, s3);

    if (ps1 > ps2 && ps1 > ps3) {
        return s1;
    } else if (ps2 > ps1 && ps2 > ps3) {
        return s2;
    }

    return s3;
}

void combina(int s1, int s2) {
    int i;
    int n;
    int idx;

    if (calculaPeso(s1) > calculaPeso(s2)) {
        idx = s2;
    } else {
        idx = s1;
    }

    for ( i = 0; i < 5; i++ ) {
        if (i % 2 == 0) {
            n = solucoes[s1][i];    
        } else {
            n = solucoes[s2][i];
        }

        while (contido(idx, n, i)) {
            n = rand() % numeroVertices; 
        }

        solucoes[idx][i] = n;
    } 
}

int maior() {
    int i;
    int idxMaior = 0;
    double pesoMaior = calculaPeso(idxMaior);
    double pesoAux;

    for (i = 1; i < TAM_POPULACAO; i++) {
        pesoAux = calculaPeso(i);
        if (pesoAux > pesoMaior) {
            pesoMaior = pesoAux;
            idxMaior = i;
        }
    }

    return idxMaior;
}

void exibe() {
    int i, j;
    double soma = 0;
    for (i = 0; i < TAM_POPULACAO; i++) {
        for (j = 0; j < 5; j++) {
            printf("%d ", solucoes[i][j]);
        }
        
        printf("\n%.2lf\n", calculaPeso(i));
    }
}

