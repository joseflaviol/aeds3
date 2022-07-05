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
    Aresta         **arestas;
    Aresta **listaAdjacencia;
} Grafo;

int *altura;
int *chefe;
Aresta *pq;
Grafo *mst;

void alocaGrafo(Grafo **grafo, int numeroVertices, int numeroArestas);
void iniciaGrafo(Grafo **grafo, char *nomeArquivo);
void adicionaAresta(Grafo *grafo, int v1, int v2, double peso);

void inicializaUF(Grafo *grafo);
int find(int vertice);
void unionByRank(int vertice1, int vertice2);

int separa (Aresta **v, int p, int r);
void qcksrt (Aresta **v, int p, int r);

void mstKruskal(Grafo *grafo);

void maiorSubGrafoInduzido(Grafo *grafo);

int main(int argc, char *argv[]) {
    
    if (argc <= 1) {
        printf("Informe o nome do arquivo\n");
        exit(0);
    }

    Grafo *grafo;
    
    iniciaGrafo(&grafo, argv[1]);
    
    mstKruskal(grafo);
 
    maiorSubGrafoInduzido(mst);
    
    return 0;

}

void alocaGrafo(Grafo **grafo, int numeroVertices, int numeroArestas) {
    int i;

    ( *grafo ) = malloc(sizeof(Grafo));
    ( *grafo )->numeroVertices = numeroVertices;
    ( *grafo )->numeroArestas = numeroArestas;
    ( *grafo )->arestas = malloc(sizeof(Aresta *) * numeroArestas);
    ( *grafo )->listaAdjacencia = malloc(sizeof(Aresta *) * numeroVertices);

    for (i = 0; i < numeroVertices; i++) {
        ( *grafo )->listaAdjacencia[i] = NULL;
    }
}

void iniciaGrafo(Grafo **grafo, char *nomeArquivo) {
    int i, j, k, cont;
    double x, y, peso;
    double *coordenadas;
    FILE *arq;
    
    arq = fopen(nomeArquivo, "r");

    if (arq == NULL) {
        printf("Arquivo nao encontrado\n");
        exit(0);
    }
    
    cont = 0;
    while (fscanf(arq, "%d %lf %lf", &i, &x, &y) != EOF) {
        cont++;
    }
    
    fclose(arq);
    
    alocaGrafo(grafo, cont, (cont - 1) * (cont) / 2);

    coordenadas = (double *) malloc(sizeof(double) * cont * 2);
    
    arq = fopen(nomeArquivo, "r+");
    
    while (fscanf(arq, "%d %lf %lf", &i, &x, &y) != EOF) {
        coordenadas[(i - 1) * 2] = x;
        coordenadas[(i - 1) * 2 + 1] = y;
    }
   
    fclose(arq);

    for (i = 0, k = 0; i < cont; i++) {
        for (j = i + 1; j < cont; j++) {
            peso = sqrt( pow( coordenadas[j * 2] - coordenadas[i * 2] , 2 ) +  pow( coordenadas[j * 2 + 1] - coordenadas[i * 2 + 1] , 2 ) );
            adicionaAresta(*grafo, i, j, peso);
            //adicionaAresta(*grafo, j, i, peso);
            ( *grafo )->arestas[k++] = ( *grafo )->listaAdjacencia[i];
        }
    }
 
}

void adicionaAresta(Grafo *grafo, int v1, int v2, double peso) {
    Aresta *a1;

    a1 = malloc(sizeof(Aresta));

    a1->vertice1 = v1;
    a1->vertice2 = v2;
    a1->peso = peso;
    a1->proximo = grafo->listaAdjacencia[v1];
    
    grafo->listaAdjacencia[v1] = a1;
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

int separa (Aresta **v, int p, int r) {
   Aresta *c = v[r]; // pivô
   Aresta *t; 
   int j = p;
   for (int k = p; /*A*/ k < r; ++k) {
      if (v[k]->peso <= c->peso) {
         t = v[j], v[j] = v[k], v[k] = t;
         ++j; 
      }
   }
   t = v[j], v[j] = v[r], v[r] = t;
   return j; 
}

void qcksrt (Aresta **v, int p, int r) {
   while (p < r) {          
      int j = separa (v, p, r); 
      qcksrt (v, p, j-1);
      p = j + 1;            
   }
}

void mstKruskal(Grafo *grafo) {
    int i, j;
    int findV1, findV2;

    alocaGrafo(&mst, grafo->numeroVertices, grafo->numeroVertices - 1);
    
    inicializaUF(grafo);
     
    qcksrt(grafo->arestas, 0, grafo->numeroArestas - 1);
    
    FILE *arq = fopen("mst.txt", "w+");

    for (i = 0, j = 0; j < grafo->numeroVertices - 1; i++) {
        findV1 = find(grafo->arestas[i]->vertice1);
        findV2 = find(grafo->arestas[i]->vertice2);
        if (findV1 != findV2) {
            fprintf(arq, "%d %d %.2lf\n", grafo->arestas[i]->vertice1, grafo->arestas[i]->vertice2, grafo->arestas[i]->peso);
            adicionaAresta(mst, grafo->arestas[i]->vertice1, grafo->arestas[i]->vertice2, grafo->arestas[i]->peso);
            adicionaAresta(mst, grafo->arestas[i]->vertice2, grafo->arestas[i]->vertice1, grafo->arestas[i]->peso);
            mst->arestas[j++] = grafo->arestas[i];  
            unionByRank(findV1, findV2);
        }
    }   

    fclose(arq);
    //exit(0);
}

void maiorSubGrafoInduzido(Grafo *grafo) {
    int i;
    int maiorV[3];
    double peso, maiorPeso = 0;
    Aresta *aux, *aux2;

    FILE *arq = fopen("subgrafos.txt", "w+");

    for (i = 0; i < grafo->numeroVertices; i++) {
        aux = grafo->listaAdjacencia[i];
        while (aux != NULL) {
            peso = aux->peso;

            aux2 = grafo->listaAdjacencia[aux->vertice2];
            while (aux2 != NULL) {
                fprintf(arq, "%d %d %d %.2lf = (%d %d %.2lf) + (%d %d %.2lf)\n", aux->vertice1, aux->vertice2, aux2->vertice2, peso + aux2->peso, aux->vertice1, aux->vertice2, aux->peso, aux2->vertice1, aux2->vertice2, aux2->peso);
                if (aux2->vertice2 > i && peso + aux2->peso > maiorPeso) {
                    maiorV[0] = aux->vertice1;
                    maiorV[1] = aux->vertice2;
                    maiorV[2] = aux2->vertice2;
                    maiorPeso = peso + aux2->peso;
                }
                aux2 = aux2->proximo;
            }
            
            aux = aux->proximo;
        }    
    }

    fclose(arq);

    /*int v1 = grafo->arestas[grafo->numeroArestas - 1]->vertice1;
    int v2 = grafo->arestas[grafo->numeroArestas - 1]->vertice2;
    int v3;
    double peso = grafo->arestas[grafo->numeroArestas - 1]->peso;
    double maiorPeso = 0;
    Aresta *aux = grafo->listaAdjacencia[v2];

    while (aux != NULL) {
        if (aux->vertice2 > v1 &&  aux->peso + peso > maiorPeso) {
            v3 = aux->vertice2;
            maiorPeso = aux->peso + peso;
        }
        aux = aux->proximo;
    }

    aux = grafo->listaAdjacencia[v1];

    while (aux != NULL) {
        if (aux->vertice2 != v2 &&  aux->peso + peso > maiorPeso) {
            v3 = aux->vertice2;
            maiorPeso = aux->peso + peso;
        }
        aux = aux->proximo;
    }*/

    printf("%d %d %d\n", maiorV[0], maiorV[1], maiorV[2]);
    //printf("%d %d %d\n", v1, v2, v3);
    printf("%.2lf\n", maiorPeso);

}