#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "process.h"

// _pronto = malloc(sizeof(Fila));
// _pronto->prox = _pronto;
// _PCB = malloc(sizeof(Process));

char _pipe(char func){
	return func;
}

char _fork(char nome){
	return nome;
}

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