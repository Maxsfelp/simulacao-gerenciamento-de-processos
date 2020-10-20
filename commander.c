#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "commander.h"
#include "manager.h"

char *entrada(){
	FILE *arquivo = fopen("com", "r");
	char comando;
	int i = 0;
	char *comandos;

	while(getc(arquivo) != EOF)
		i++;

	comandos = malloc(i*sizeof(char));
	i = 0;
	rewind(arquivo);

	while(comando != EOF){
		comando = getc(arquivo);
		comandos[i] = comando;
		i++;
	}
	comandos[i-1] = '\0';
	return comandos;
}

