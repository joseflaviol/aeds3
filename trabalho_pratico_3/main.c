#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define TAM_POPULACAO 50

int numeroVertices;
int *coordenadas;
int *maiores;
double *matrizAdj;
int solucoes[TAM_POPULACAO][5];

// funcoes auxiliares
int leNumeroVertices(const char *nomeArq);
void montaMatrizAdj(const char *nomeArq);
bool contido(int i, int v, int n);
int left(int i, int j);
int right(int i, int j);
double calculaPeso(int index);
int menor();
int maior();
void exibeSolucao(int idx);

// funcoes do algoritmo evolutivo
void geraPopulacao();
int seleciona();
int combina(int s1, int s2);
void muta(int idx);


int main(int argc, char **argv) {

    int i, j, idxMaior;
    int s1, s2;

    if (argc < 2) {
        printf("Informe o nome do arquivo\n");
        exit(0);
    }

    montaMatrizAdj(argv[1]);
    
    srand(1);
    
    geraPopulacao();
    
    clock_t start = clock(), diff;
    int msec = 0;    
     
    printf("%s\n", argv[1]); 

    while (msec <= 10) {
        for (j = 0; j < TAM_POPULACAO; j++) {
            s1 = seleciona();
            s2 = seleciona();
            muta(combina(s1, s2));
        }
        diff = clock() - start;
        msec = (diff * 1000 / CLOCKS_PER_SEC) / 1000;
    }
      
    printf("10 segundos: \n\t");
    exibeSolucao(maior());

    while (msec <= 30) {
        for (j = 0; j < TAM_POPULACAO; j++) {
            s1 = seleciona();
            s2 = seleciona();
            muta(combina(s1, s2));
        }
        diff = clock() - start;
        msec = (diff * 1000 / CLOCKS_PER_SEC) / 1000;
    }
    
    printf("30 segundos: \n\t");
    exibeSolucao(maior());
    
    while (msec <= 60) {
        for (j = 0; j < TAM_POPULACAO; j++) {
            s1 = seleciona();
            s2 = seleciona();
            muta(combina(s1, s2));
        }
        diff = clock() - start;
        msec = (diff * 1000 / CLOCKS_PER_SEC) / 1000;
    }
    
    printf("60 segundos: \n\t");
    exibeSolucao(maior());
    
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
    maiores = (int *) malloc(sizeof(int) * numeroVertices * 6);
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

void geraPopulacao() {
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

int seleciona() {
    int s1, s2, s3;
    double ps1, ps2, ps3;

    s1 = rand() % TAM_POPULACAO;
    s2 = rand() % TAM_POPULACAO;
    s3 = rand() % TAM_POPULACAO;

    ps1 = calculaPeso(s1);
    ps2 = calculaPeso(s2);
    ps3 = calculaPeso(s3);

    if (ps1 > ps2 && ps1 > ps3) {
        return s1;
    } else if (ps2 > ps1 && ps2 > ps3) {
        return s2;
    }

    return s3;
}

int combina(int s1, int s2) {
    int i;
    int n;
    int idx = menor();

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
    
    return idx;
}

void muta(int idx) {
    int x = rand() % 5;
    int y = rand() % 5;    
    int nx = rand() % numeroVertices;
    
    while (contido(idx, nx, 5)) {
        nx = rand() % numeroVertices;
    }
    
    solucoes[idx][x] = nx;
    
    int ny = rand() % numeroVertices;
    
    while (contido(idx, ny, 5)) {
        ny = rand() % numeroVertices;
    }
    
    solucoes[idx][y] = ny;
}

int menor() {
    int i;
    int idxMenor = 0;
    double pesoMenor = calculaPeso(idxMenor);
    double pesoAux;

    for (i = 1; i < TAM_POPULACAO; i++) {
        pesoAux = calculaPeso(i);
        if (pesoAux < pesoMenor) {
            pesoMenor = pesoAux;
            idxMenor = i;
        }
    }

    return idxMenor;
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

void exibeSolucao(int idx) {
    int i;
    for (i = 0; i < 5; i++) {
        printf("%d ", solucoes[idx][i]);
    }

    printf("\n\t%.2f\n", calculaPeso(idx));
}

