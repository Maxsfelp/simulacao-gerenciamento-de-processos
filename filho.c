#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define TabelaSize PM.qtd1
#define ProntoSize PM.qtd2
#define BloqueadoSize PM.qtd3


typedef struct CPU{
    int valor_inteiro;
    int tempo; // Tempo do processo
    FILE *apontador;
    int contador_de_programa;
    int Tempo_Atual; // Tempo total
}CPU;

typedef struct TabelaPcb{
    pid_t id;
    pid_t id_pai;
    int contador_de_programa;
    FILE *arquivo_do_programa;
    int inteiro;
    int prioridade;
    int estado; // 0 - Bloqueado // 1 - Em Espera // 2 - Executando
    int tempo_incial;
    int tempo_utilizado;
    struct TabelaPcb *prox;
    struct TabelaPcb *anterior;
}TabelaPcb;

typedef struct ApontaTabela {
    TabelaPcb *apontador;
    struct ApontaTabela *prox;
    struct ApontaTabela *anterior;
}ApontaTabela;

typedef struct ProcessManager{
    int Tempo;
    CPU CPU;
    int qtd1,qtd2,qtd3;
    TabelaPcb *Tabela; // Fila
    ApontaTabela *EstadoPronto; // Ponteiro de índices para prontos
    ApontaTabela *EstadoBloqueado; // Ponteiro de índices para bloqueados
    ApontaTabela *EstadoExecutando; // Ponteiro de índices para executando
}ProcessManager;


// __________________________________________________ FUNÇÕES __________________________________________________ //

ProcessManager Inicializa_Dados(FILE *init){
    ProcessManager PM;
    TabelaSize = 1;
    ProntoSize = 1;
    BloqueadoSize = 1;
    PM.Tabela = (TabelaPcb*) calloc (TabelaSize, sizeof(TabelaPcb));
    PM.EstadoBloqueado = (ApontaTabela*) calloc (BloqueadoSize, sizeof(ApontaTabela));
    PM.EstadoPronto = (ApontaTabela*) calloc (ProntoSize, sizeof(ApontaTabela));
    PM.EstadoExecutando = (ApontaTabela*) calloc (ProntoSize, sizeof(ApontaTabela));
    PM.Tabela->arquivo_do_programa = init;
    PM.Tabela->id_pai = getpid();
    PM.Tabela->estado = 2;
    PM.Tabela->prox = NULL;
    PM.Tabela->anterior = PM.Tabela;
    PM.EstadoBloqueado->apontador = NULL;
    PM.EstadoBloqueado->prox = NULL;
    PM.EstadoBloqueado->anterior = NULL;
    PM.EstadoPronto->apontador = PM.Tabela;
    PM.EstadoPronto->prox = NULL;
    PM.EstadoPronto->anterior = NULL;
    PM.EstadoExecutando->apontador = PM.Tabela;
    PM.EstadoExecutando->prox = NULL;
    PM.EstadoExecutando->anterior = NULL;
    PM.CPU.apontador = PM.Tabela->arquivo_do_programa;
    PM.CPU.contador_de_programa = 0;
    PM.CPU.tempo = 0;
    PM.CPU.Tempo_Atual = 0;
    PM.CPU.valor_inteiro = PM.Tabela->inteiro;
    return PM;
}

void Criacao_de_processo(){


}

void Substituicao_de_processo(ProcessManager *PM){
    PM->EstadoExecutando->apontador = PM->EstadoPronto->prox->apontador;
    PM->EstadoPronto = PM->EstadoPronto->prox;
}

void Gerenciamento_de_transicao(ProcessManager *PM){
    PM->EstadoExecutando->apontador->arquivo_do_programa = PM->CPU.apontador;
    PM->EstadoExecutando->apontador->tempo_utilizado = PM->CPU.tempo;
    PM->EstadoExecutando->apontador->inteiro = PM->CPU.valor_inteiro;
    PM->EstadoExecutando->apontador->contador_de_programa = PM->CPU.contador_de_programa;
}

void Troca_de_contexo(ProcessManager *PM){
    PM->CPU.valor_inteiro = PM->EstadoPronto->prox->apontador->inteiro;
    PM->CPU.contador_de_programa = PM->EstadoPronto->prox->apontador->contador_de_programa;
    PM->CPU.tempo = PM->EstadoPronto->prox->apontador->tempo_utilizado;
    PM->CPU.apontador = PM->EstadoPronto->prox->apontador->arquivo_do_programa;
}

void Escalonador(ProcessManager *PM){ // Politica adotada: Tipo FIFO ignorando prioridade
    /*  
        1 - Pegar os dados da CPU e mandar para a Tabela PCB (atualizar)
        2 - Pegar o próximo processo pronto e colocar na CPU
        3 - Atualizar os vetores Pronto e Executando
    */
    Gerenciamento_de_transicao(PM); // 1
    Troca_de_contexo(PM); // 2
    Substituicao_de_processo(PM); // 3 
}


// _________________________________________________ FIM FUNÇÕES _________________________________________________ //



int main() {
    pid_t ProcessManager;
    FILE *init;
    init = fopen ("init.txt","r");

    if((ProcessManager = fork()) == -1) {
        perror("fork");
    }

    if(ProcessManager == 0) { // Process Init Executando

    }else{ // Process Manager Executando
        char string;
        struct ProcessManager PM;
        PM = Inicializa_Dados(init);
        do {
            scanf("%c", &string);
            switch(string){
                case 'Q':
                    printf ("Troca de contexto.\n");
                    //Escalonador(PM);
                    break;
                case 'U':
                    printf ("Desbloquear processo simulado.\n");
                    break;
                case 'P': // Cria Reporter
                    printf ("Imprimir estado atual do sistema.\n");
                    break;
                case 'T': // Cria outro Reporter
                    printf ("Fechar programa.\n");
                    break;
            }
        } while(string != 'T');
    }
}
