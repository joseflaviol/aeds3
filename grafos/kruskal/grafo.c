#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct no {
    int verticeInicial;
    int   verticeFinal;
    int           peso;
    struct no *proximo;
} Item; // Neste caso, um Item é uma Aresta

typedef struct {
    Item *inicio;
} Lista;

typedef struct {
    int numeroVertices;
    Lista     *arestas;
    Lista   **listaAdj;
} Grafo;

int *altura;
int *chefe;

void inicializa(Grafo **grafo, char *caminhoArq);
int leNumeroVertices(FILE *arq);
void adicionaAresta(Grafo *grafo, int vertice1, int vertice2, int peso);

void inicializaUF(Grafo *grafo);
int find(int vertice);
void unionByRank(int vertice1, int vertice2);
void mstKruskal(Grafo *grafo);

int main() {

    Grafo *grafo;

    inicializa(&grafo, "grafo.txt");

    int i;
    Item *p;

    for (i = 0; i < grafo->numeroVertices; i++) {
        p = grafo->listaAdj[i]->inicio;
        while (p != NULL) {
            printf("%d -> %d %d\n", p->verticeInicial, p->verticeFinal, p->peso);
            p = p->proximo;
        }
    }

    free(grafo->listaAdj);
    free(grafo);

    return 0;

}

void inicializa(Grafo **grafo, char *caminhoArq) {
    int i, j, k;
    int numeroVertices;
    FILE *arq;
   
    arq = fopen(caminhoArq, "r+");
    numeroVertices = leNumeroVertices(arq);
    fclose(arq);

    *grafo = malloc( sizeof( Grafo ) );
    ( *grafo )->numeroVertices = numeroVertices;

    // Inicializa Lista
    ( *grafo )->listaAdj = malloc( sizeof( Lista* ) * numeroVertices );
    for (i = 0; i < numeroVertices; i++) {
        ( *grafo )->listaAdj[i] = malloc( sizeof ( Lista ) );
        ( *grafo )->listaAdj[i]->inicio = NULL;
    }

    arq = fopen(caminhoArq, "r+");

    while (fscanf(arq, "%d %d %d", &i, &j, &k) != EOF) {
        adicionaAresta(*grafo, i, j, k);
    }

    fclose(arq);

}

int leNumeroVertices(FILE *arq) {
    int i, j, k;
    int maior = -1;

    while (fscanf(arq, "%d %d %d", &i, &j, &k) != EOF) {
        if (i > maior) {
            maior = i;
        }
    } 

    return maior + 1;
}

void adicionaAresta(Grafo *grafo, int vertice1, int vertice2, int peso) {
    Item *novoItem;
    novoItem = malloc( sizeof ( Item ) );

    novoItem->verticeInicial = vertice1;
    novoItem->verticeFinal = vertice2;
    novoItem->peso = peso;
    novoItem->proximo = grafo->listaAdj[vertice1]->inicio;

    grafo->listaAdj[vertice1]->inicio = novoItem;
}

void inicializaUF(Grafo *grafo) {
    int i;
    
    altura = (int *) malloc(sizeof(int) * grafo->numeroVertices);
    chefe = (int *) malloc(sizeof(int) * grafo->numeroVertices);

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

void mstKruskal(Grafo *grafo) {
    Lista *listaArestas;
}