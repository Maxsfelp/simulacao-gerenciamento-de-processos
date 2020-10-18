#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "process.h"

char _pipe(char func){
	return func;
}

char _fork(char nome){
	return nome;
}

Process *colocanafila (int y, Process *fi) { 
   Process *nova;
   nova = malloc (sizeof (Process));
   nova->prox = fi->prox;
   fi->prox = nova;
   fi->conteudo = y;
   return nova;
}

int tiradafila (Process *fi) {
   Process *p;
   p = fi->prox;  // o primeiro da fila
   int x = p->conteudo;
   fi->prox = p->prox;
   free (p);
   return x;  
}