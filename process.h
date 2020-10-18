typedef struct fila {
   int indice; 
   struct fila *prox;
} Fila;

Fila *fi;
fi = malloc (sizeof (Process));
fi->prox = fi;

char _pipe(char func);
char _fork(char nome);