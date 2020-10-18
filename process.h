typedef struct process{
	int id;
} Process;

typedef struct fila{
   int indice; 
   struct fila *prox;
} Fila;

Fila *_pronto;
Fila *_bloqueado;
Fila *_executando;

Process *_PCB;

char _pipe(char func);
char _fork(char nome);
Fila *colocanafila (int y, Fila *fi);
int tiradafila (Fila *fi);