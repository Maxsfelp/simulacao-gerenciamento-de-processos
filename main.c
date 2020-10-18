#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "commander.h"
#include "manager.h"

int main(int argc, char const *argv[]){
	char pipe = _pipe('D');
	printf("%c\n", pipe);
	return 0;
}