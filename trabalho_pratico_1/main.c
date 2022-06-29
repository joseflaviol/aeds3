#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct no {
    int       vertice1;
    int       vertice2;
    double        peso;
    struct no *proximo;
} Aresta;

typedef struct {
    int       numeroVertices;
    int        numeroArestas;
    Aresta          *arestas;
    Aresta **listaAdjacencia;
} Grafo;

int *altura;
int *chefe;
Aresta *pq;
Aresta *mst;

void iniciaGrafo(Grafo **grafo, char *nomeArquivo);
void adicionaAresta(Grafo *grafo, int v1, int v2, double peso);

void inicializaUF(Grafo *grafo);
int find(int vertice);
void unionByRank(int vertice1, int vertice2);

void ordena(Aresta *pq, int tamanho);

void mstKruskal(Grafo *grafo);

int main() {
    
    Grafo *grafo;
    
    iniciaGrafo(&grafo, "grafo.txt");
    
    mstKruskal(grafo);
   
    for (int i = 0; i < grafo->numeroVertices; i++) {
        printf("%d %d %.2lf\n", mst[i].vertice1, mst[i].vertice2, mst[i].peso);
    }

    free(pq);
    free(mst);
    free(grafo);
    
    return 0;

}

void iniciaGrafo(Grafo **grafo, char *nomeArquivo) {
    int i, j, k, cont;
    double x, y, peso;
    double **coordenadas;
    FILE *arq;
    
    ( *grafo ) = malloc(sizeof(Grafo));
    
    arq = fopen(nomeArquivo, "r+");
    
    cont = 0;
    while (fscanf(arq, "%d %lf %lf", &i, &x, &y) != EOF) {
        cont++;
    }
    
    fclose(arq);
    
    ( *grafo )->numeroVertices = cont;
    ( *grafo )->numeroArestas = (cont - 1) * (cont) / 2;
    ( *grafo )->arestas = malloc(sizeof(Aresta) * ( *grafo )->numeroArestas );
    ( *grafo )->listaAdjacencia = malloc(sizeof(Aresta *) * cont);
    
    coordenadas = (double **) malloc(sizeof(double *) * cont);
    for (i = 0; i < cont; i++) {
        ( *grafo )->listaAdjacencia[i] = NULL;
        coordenadas[i] = (double *) malloc(sizeof(double) * 2);
    }
            
    arq = fopen(nomeArquivo, "r+");
    
    while (fscanf(arq, "%d %lf %lf", &i, &x, &y) != EOF) {
        coordenadas[i - 1][0] = x;
        coordenadas[i - 1][1] = y;
    }
    
    fclose(arq);
    
    for (i = 0, k = 0; i < cont; i++) {
        for (j = i + 1; j < cont; j++) {
            peso = sqrt( pow( coordenadas[j][0] - coordenadas[i][0] , 2 ) +  pow( coordenadas[j][1] - coordenadas[i][1] , 2 ) );
            adicionaAresta(*grafo, i, j, peso);
            adicionaAresta(*grafo, j, i, peso);
            ( *grafo )->arestas[k++] = ( * ( *grafo )->listaAdjacencia[i] );
        }
    }
    
}

void adicionaAresta(Grafo *grafo, int v1, int v2, double peso) {
    Aresta *novaAresta;
    
    novaAresta = malloc(sizeof(Aresta));
    novaAresta->vertice1 = v1;
    novaAresta->vertice2 = v2;
    novaAresta->peso = peso;
    novaAresta->proximo = grafo->listaAdjacencia[v1];
    
    grafo->listaAdjacencia[v1] = novaAresta;
}

void inicializaUF(Grafo *grafo) {
    int i;
    
    altura = (int *) malloc(sizeof(int) * grafo->numeroVertices);
    chefe  = (int *) malloc(sizeof(int) * grafo->numeroVertices);

    for (i = 0; i < grafo->numeroVertices; i++) {
        altura[i] = 0;
        chefe[i] = i;
    }
}

int find(int vertice) {
    while (chefe[vertice] != vertice) {
        vertice = chefe[vertice];
    }

    return vertice;
}

void unionByRank(int vertice1, int vertice2) {
    if (altura[vertice1] > altura[vertice2]) {
        chefe[vertice2] = vertice1;
    } else {
        chefe[vertice1] = vertice2;
        if (altura[vertice1] == altura[vertice2]) {
            altura[vertice2] = altura[vertice1] + 1;
        }
    }
}

void ordena(Aresta *pq, int tamanho) {
    int i, j;
    Aresta chave;

    for (i = 1; i < tamanho; i++) {
        chave = pq[i];
        j = i - 1;
        while (j > -1 && chave.peso < pq[j].peso) {
            pq[j + 1] = pq[j];
            j--;
        }
        pq[j + 1] = chave;
    }
}

void mstKruskal(Grafo *grafo) {
    int i, j;
    Aresta chave;
    
    mst = malloc(sizeof(Aresta) * (grafo->numeroVertices - 1));
    pq = malloc(sizeof(Aresta) * grafo->numeroArestas);
    
    inicializaUF(grafo);
    
    for (i = 0; i < grafo->numeroArestas; i++) {
        pq[i] = grafo->arestas[i];
    }
    
    ordena(pq, grafo->numeroArestas);

    for (i = 0, j = 0; i < grafo->numeroArestas; i++) {
        if (find(pq[i].vertice1) != find(pq[i].vertice2)) {
            mst[j++] = pq[i];
            unionByRank(pq[i].vertice1, pq[i].vertice2);
        }
    }   
}