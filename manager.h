typedef struct process{
	int id;
	int id_pai;
	int value;
	int priorit;
} Process;

typedef struct cpu{
	int PC;
	int value;
	int time;
	int real_time;
} CPU;

typedef struct fila{
   int indice; 
   struct fila *prox;
} Fila;

Fila *_pronto;
Fila *_bloqueado;
Fila *_executando;

Process *_PCB;

Fila *colocanafila (int y, Fila *fi);
int tiradafila (Fila *fi);