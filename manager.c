#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "manager.h"

Fila *colocanafila (int y, Fila *fi) { 
   Fila *nova;
   nova = malloc (sizeof (Fila));
   nova->prox = fi->prox;
   fi->prox = nova;
   fi->indice = y;
   return nova;
}

int tiradafila (Fila *fi) {
   Fila *p;
   p = fi->prox;  // o primeiro da fila
   int x = p->indice;
   fi->prox = p->prox;
   free (p);
   return x;  
}

void manager(char *comando){
	_PCB = malloc(sizeof(Process));
	_pronto = malloc(sizeof(Fila));
	_bloqueado = malloc(sizeof(Fila));
	_executando = malloc(sizeof(Fila));
}