typedef struct process{
	int id;
} Process;

typedef struct fila {
   int indice; 
   struct fila *prox;
} Fila;

//Fila *_pronto;
//Fila *estadoBloqueado = (*Fila)malloc(sizeof(Fila));
//Fila *estadoExecutando = (*Fila)malloc(sizeof(Fila));

//estadoBloqueado->prox = estadoBloqueado;

//estadoExecutando->prox = estadoExecutando;


char _pipe(char func);
char _fork(char nome);
Fila *colocanafila (int y, Fila *fi);
int tiradafila (Fila *fi);