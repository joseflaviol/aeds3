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
    Lista   **listaAdj;
} Grafo;

int *distancia;
int *pai;

void inicializa(Grafo **grafo, char *caminhoArq);
int leNumeroVertices(FILE *arq);
void adicionaAresta(Grafo *grafo, int vertice1, int vertice2, int peso);

void inicializaDijkstra(Grafo *grafo, int s);
void relaxa(int u, int v, int w);
void dijkstra(Grafo *grafo, int s);

bool vazia(int *filaPrioridade, int tFila);
int extraiMinimo(int *filaPrioridade, int tFila);

int main(/*int argc, char *argv[]*/) {

    Grafo *grafo;

    /*if (argc < 1) {
        exit(0);
    }*/

    inicializa(&grafo, /*argv[0]*/ "grafo.txt");

    int i;
    Item *p;

    for (i = 0; i < grafo->numeroVertices; i++) {
        p = grafo->listaAdj[i]->inicio;
        while (p != NULL) {
            printf("%d -> %d %d\n", p->verticeInicial, p->verticeFinal, p->peso);
            p = p->proximo;
        }
    }

    dijkstra(grafo, 0);

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

void inicializaDijkstra(Grafo *grafo, int s) {
    int i;

    distancia = (int *) malloc(sizeof(int) * grafo->numeroVertices);
    pai = (int *) malloc(sizeof(int) * grafo->numeroVertices);

    for (i = 0; i < grafo->numeroVertices; i++) {
        distancia[i] = 10000;
        pai[i] = -1;
    }

    distancia[s] = 0;
}

void relaxa(int u, int v, int w) {
    if (distancia[v] > distancia[u] + w) {
        distancia[v] = distancia[u] + w;
        pai[v] = u;
    }
}

void dijkstra(Grafo *grafo, int s) {
    int i;
    int u;
    int *definidos;
    int *filaPrioridade;
    Item *p;

    inicializaDijkstra(grafo, s);
    definidos = (int *) malloc(sizeof(int) * grafo->numeroVertices);
    filaPrioridade = (int *) malloc(sizeof(int) * grafo->numeroVertices);

    for (i = 0; i < grafo->numeroVertices; i++) {
        definidos[i] = 0;
        filaPrioridade[i] = distancia[i];
    }

    while ( !vazia(filaPrioridade, grafo->numeroVertices) ) {
        u = extraiMinimo(filaPrioridade, grafo->numeroVertices);
        definidos[u] = 1;

        p = grafo->listaAdj[u]->inicio;
        while (p != NULL) {
            relaxa(p->verticeInicial, p->verticeFinal, p->peso);
            filaPrioridade[p->verticeFinal] = filaPrioridade[p->verticeFinal] > -1 ? distancia[p->verticeFinal] : -1; 
            printf("%d %d\n", p->verticeInicial, p->verticeFinal);
            p = p->proximo;
        }
    }

    for (i = 0; i < grafo->numeroVertices; i++) {
        printf("distancia[%d] = %d\n", i, distancia[i]);
        printf("pai[%d] = %d\n", i, pai[i]);       
    }
}

bool vazia(int *filaPrioridade, int tFila) {
    int i;

    for (i = 0; i < tFila; i++) {
        if (filaPrioridade[i] > -1) {
            return false;
        }
    }

    return true;
}

int extraiMinimo(int *filaPrioridade, int tFila) {
    int i;
    int minimo;

    for (i = 0, minimo = 0; i < tFila; i++) {
        if (filaPrioridade[i] != -1 && filaPrioridade[i] < filaPrioridade[minimo] || filaPrioridade[i] != -1 && filaPrioridade[minimo] == -1) {
            minimo = i;
        }
    }

    filaPrioridade[minimo] = -1;

    return minimo;
}