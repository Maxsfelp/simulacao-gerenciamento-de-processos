#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commander.h"
#include "manager.h"

int main(int argc, char const *argv[]){
	char *comando = entrada();
	printf("%s\n", comando);
	return 0;
}