#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct CPU{
    int valor_inteiro;
    pid_t *contador_de_programa;
    pid_t *valor_Atual;
    int tempo;
}CPU;

typedef struct TabelaPcb{
    pid_t id;
    pid_t id_pai;
    pid_t *contador_do_programa;
    int inteiro;
    int prioridade;
    int estado; // 0 - Bloqueado // 1 - Em Espera // 2 - Executando
    int tempo_incial;
    int tempo_utilizado;
}TabelaPcb;

struct ProcessManager{
    int Tempo;
    CPU CPU;
    TabelaPcb *Tabela; // Fila
    int *EstadoPronto; // Ponteiro de índices para prontos
    int *EstadoBloqueado; // Ponteiro de índices para bloqueados
    int EstadoExecutando; // Ponteiro de índices para executando
};

TabelaPcb* Inicia (TabelaPcb *Tabela){
    return (TabelaPcb*) calloc (1, sizeof(TabelaPcb));
}

TabelaPcb* Esvaziar (TabelaPcb *Tabela){
    int x = 0;
    while (Tabela[x].id != 0){
        Tabela[x].contador_do_programa = NULL;
        Tabela[x].estado = -1;
        Tabela[x].id = -1;
        Tabela[x].id_pai = -1;
        Tabela[x].inteiro = -1;
        Tabela[x].prioridade = -1;
        Tabela[x].tempo_incial = -1;
        Tabela[x].tempo_utilizado = -1;
        x++;
    }
    return Tabela;
}

bool Vazia (TabelaPcb *Tabela){
    int x = 0;
    while (true){
        if (Tabela[x].id != -1){
            return false;
        }
    }
    return true;
}

TabelaPcb* Push (TabelaPcb *Tabela, TabelaPcb tabela){
    int x = 0;
    while (Tabela[x].id != 0){
        x++;
    }
    Tabela[x].id = tabela.id;
    Tabela[x].estado = tabela.estado;
    Tabela[x].contador_do_programa = tabela.contador_do_programa;
    Tabela[x].id_pai = tabela.id_pai;
    Tabela[x].inteiro = tabela.inteiro;
    Tabela[x].prioridade = tabela.prioridade;
    Tabela[x].tempo_incial = tabela.tempo_incial;
    Tabela[x].tempo_utilizado = tabela.tempo_utilizado;
    return Tabela;
}

char* Pop (TabelaPcb *Tabela, int id){
    int x = 0;
    while (Tabela[x].id != id && Tabela[x].id != 0){
        x++;
    }
    if (Tabela[x].id == id){
        // TODO: Remover Processo
        return "Processo Removido com sucesso.\n";
    }
    return "Processo especificado não encontra-se na lista.\n";
}


int main() {
	char string;
	do {
        scanf("%c", &string);
        switch(string){
            case 'Q':
                printf ("Troca de contexto.\n");
                break;
            case 'U':
                printf ("Desbloquear processo simulado.\n");
                break;
            case 'P':
                printf ("Imprimir estado atual do sistema.\n");
                break;
            case 'T':
                printf ("Fechar programa.\n");
                break;
        }
	} while(string != 'T');
}