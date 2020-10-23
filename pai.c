#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int writepipe[2] = {-1, -1};
    pid_t commander;

    if(pipe(writepipe) < 0) {
        perror("pipe");
        exit(1);
    }

    if((commander = fork()) == -1) {
        perror("fork");
    }

    if(commander == 0) { // Process Manager Executando
        close(writepipe[1]);
        dup2(writepipe[0], STDIN_FILENO);
        close(writepipe[0]);
        execlp("./filho", "./filho", NULL); // Troca de contexto, instrução R (execlp (Arquivo que será utilizado, executável, NULL));

    } else { // Commander Executando
        char s;
        FILE *input;
        input = fopen("commander.txt","r");
        fscanf (input,"%c",&s);
        while (!(feof (input))){
		    close(writepipe[0]);
            write(writepipe[1], &s, 1);
            fscanf (input,"%c",&s);
            sleep(1);
        }
    }
    wait(0);
}
