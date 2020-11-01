#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct CPU{
    int valor_inteiro;
    int tempo; // Tempo do processo
    FILE *apontador; // Arquivo a ser executado
    int contador_de_programa;
    int Tempo_Atual; // Tempo total
}CPU;

typedef struct TabelaPcb{
    int id;
    int id_pai;
    int contador_de_programa;
    FILE *arquivo_do_programa;
    int inteiro;
    int prioridade;
    int estado; // 0 - Bloqueado // 1 - Pronto // 2 - Executando
    int tempo_incial;
    int tempo_utilizado;
    struct TabelaPcb *prox;
    struct TabelaPcb *anterior;
}TabelaPcb;

typedef struct ApontaTabela {
    TabelaPcb *apontador; // Ira apontar para o processo na TabelaPcb
    struct ApontaTabela *prox;
    struct ApontaTabela *anterior;
}ApontaTabela;

typedef struct ProcessManager{
    CPU CPU;
    int qtd;
    TabelaPcb *Tabela; // Fila
    ApontaTabela *EstadoPronto; // Ponteiro de índices para prontos
    ApontaTabela *EstadoBloqueado; // Ponteiro de índices para bloqueados
    ApontaTabela *EstadoExecutando; // Ponteiro de índices para executando
}ProcessManager;


// __________________________________________________ FUNÇÕES __________________________________________________ //

ProcessManager Inicializa_Dados(FILE *init){
    ProcessManager PM;
    // ProcessManager
    PM.qtd = 0;
    PM.Tabela = (TabelaPcb*) calloc (1, sizeof(TabelaPcb));
    PM.EstadoBloqueado = (ApontaTabela*) calloc (1, sizeof(ApontaTabela));
    PM.EstadoPronto = (ApontaTabela*) calloc (1, sizeof(ApontaTabela));
    PM.EstadoExecutando = (ApontaTabela*) calloc (1, sizeof(ApontaTabela));
    // Tabela
    PM.Tabela->anterior = NULL;
    PM.Tabela->arquivo_do_programa = init;
    PM.Tabela->contador_de_programa = 0;
    PM.Tabela->estado = 2;
    PM.Tabela->id = 0;
    PM.Tabela->id_pai = 0;
    PM.Tabela->inteiro = 0;
    PM.Tabela->prioridade = 0;
    PM.Tabela->prox = NULL;
    PM.Tabela->tempo_incial = 0;
    PM.Tabela->tempo_utilizado = 0;
    // Estado Bloqueado
    PM.EstadoBloqueado->apontador = NULL;
    PM.EstadoBloqueado->prox = NULL;
    PM.EstadoBloqueado->anterior = NULL;
    // Estado Pronto
    PM.EstadoPronto->apontador = PM.Tabela;
    PM.EstadoPronto->prox = NULL;
    PM.EstadoPronto->anterior = NULL;
    // Estado Executando
    PM.EstadoExecutando->apontador = PM.Tabela;
    PM.EstadoExecutando->prox = NULL;
    PM.EstadoExecutando->anterior = NULL;
    // CPU
    PM.CPU.apontador = PM.Tabela->arquivo_do_programa;
    PM.CPU.contador_de_programa = PM.EstadoExecutando->apontador->contador_de_programa;
    PM.CPU.tempo = PM.EstadoExecutando->apontador->tempo_utilizado;
    PM.CPU.Tempo_Atual = 0;
    PM.CPU.valor_inteiro = PM.EstadoExecutando->apontador->inteiro;

    PM.qtd+=1; // Proximo ID
    return PM;
}

void Criacao_de_processo(ProcessManager *PM){
    TabelaPcb *auxiliar;
    ApontaTabela *auxiliar2;
    // Adicionar processo na TabelaPcb
    if (PM->Tabela->prox == NULL){ // Apenas um processo na tabela
        PM->Tabela->prox = (TabelaPcb*) calloc (1,sizeof(TabelaPcb));
        auxiliar = PM->Tabela->prox;
        auxiliar->arquivo_do_programa = PM->EstadoExecutando->apontador->arquivo_do_programa;
        auxiliar->contador_de_programa = PM->EstadoExecutando->apontador->contador_de_programa;
        auxiliar->estado = 1; // Pronto
        auxiliar->id_pai = PM->EstadoExecutando->apontador->id;
        auxiliar->id = PM->qtd;
        auxiliar->inteiro = PM->EstadoExecutando->apontador->inteiro;
        auxiliar->prioridade = PM->EstadoExecutando->apontador->prioridade;
        auxiliar->tempo_incial = PM->CPU.Tempo_Atual;
        auxiliar->tempo_utilizado = 0;
        auxiliar->prox = NULL;
        auxiliar->anterior = PM->Tabela;
        PM->qtd += 1;
    }
    else{ // Ja existem processos na tabela
        for (auxiliar = PM->Tabela; auxiliar->prox != NULL ; auxiliar = auxiliar -> prox); // Chegar na ultima celula
        auxiliar->prox = (TabelaPcb*) calloc (1,sizeof(TabelaPcb));
        auxiliar->prox->anterior = auxiliar;
        auxiliar = auxiliar -> prox;
        auxiliar->prox = NULL;
        auxiliar->arquivo_do_programa = PM->EstadoExecutando->apontador->arquivo_do_programa;
        auxiliar->contador_de_programa = PM->EstadoExecutando->apontador->contador_de_programa;
        auxiliar->estado = 1; // Pronto
        auxiliar->id_pai = PM->EstadoExecutando->apontador->id;
        auxiliar->id = PM->qtd;
        auxiliar->inteiro = PM->EstadoExecutando->apontador->inteiro;
        auxiliar->prioridade = PM->EstadoExecutando->apontador->prioridade;
        auxiliar->tempo_incial = PM->CPU.Tempo_Atual;
        auxiliar->tempo_utilizado = 0;
        PM->qtd += 1;
    }
    /* Adicionar processo na fila de prontos (auxiliar aponta para o processo adicionado na tabela pcb)
     A criação foi feita usando uma fila com circulo, ou seja, o ultimo aponta pro primeiro e o primeiro tem um apontador pro ultimo*/
    if (PM->EstadoPronto->prox == NULL){ // Apenas um processo pronto
        // Monta um circulo
        PM->EstadoPronto->prox = (ApontaTabela*) calloc (1,sizeof(ApontaTabela));
        PM->EstadoPronto->anterior = PM->EstadoPronto->prox;
        PM->EstadoPronto->prox->prox = PM->EstadoPronto;
        PM->EstadoPronto->prox->anterior = PM->EstadoPronto;
        PM->EstadoPronto->prox->apontador = auxiliar;
    }
    else{ // Existem mais processos prontos
        auxiliar2 = PM->EstadoPronto->anterior;
        PM->EstadoPronto->anterior = (ApontaTabela*) calloc (1,sizeof(ApontaTabela));
        PM->EstadoPronto->anterior->prox = PM->EstadoPronto;
        auxiliar2->prox = PM->EstadoPronto->anterior;
        PM->EstadoPronto->anterior->anterior = auxiliar2;
        PM->EstadoPronto->anterior->apontador = auxiliar;
    }
    
}

void Troca_de_Imagem(ProcessManager *PM, FILE *input){ // Altera o programa de execução do processo (R)
    PM->CPU.apontador = input;
}

void Gerenciamento_de_transicao(ProcessManager *PM, int value){ // Gerencia as transições entre os estados dos processos, entre bloqueado, pronto e terminado
    ApontaTabela *auxiliar, *auxiliar3;
    TabelaPcb *auxiliar2;
    switch (value){
        case 1: // Bloquear um Processo (Instrução 'B' vindo de um processo simulado)
            // Arrumando Lista de Bloqueados
            if (PM->EstadoBloqueado->apontador == NULL){ // Nenhum Processo na fila
                PM->EstadoBloqueado->apontador = PM->EstadoExecutando->apontador;
                PM->EstadoBloqueado->apontador->estado = 0; // Bloqueado
            }
            else if (PM->EstadoBloqueado->prox == NULL){ // Um processo na fila
                PM->EstadoBloqueado->prox = (ApontaTabela*) calloc (1,sizeof(ApontaTabela));
                auxiliar = PM->EstadoBloqueado->prox;
                auxiliar->anterior = PM->EstadoBloqueado;
                auxiliar->prox = NULL;
                auxiliar->apontador = PM->EstadoExecutando->apontador;
                auxiliar->apontador->estado = 0;
            }
            else{ // N processos na fila
                for(auxiliar = PM->EstadoBloqueado; auxiliar->prox != NULL; auxiliar = auxiliar->prox);
                auxiliar->prox = (ApontaTabela*) calloc (1,sizeof(ApontaTabela));
                auxiliar->prox->anterior = auxiliar;
                auxiliar = auxiliar->prox;
                auxiliar->prox = NULL;
                auxiliar->apontador = PM->EstadoExecutando->apontador;
                auxiliar->apontador->estado = 0;
            }
            
            // Arrumando Lista de Prontos   
            if (PM->EstadoPronto->prox == NULL){ // A lista de prontos irá ficar vazia depois que tirar o processo
                PM->EstadoPronto->anterior = NULL;
                PM->EstadoPronto->prox = NULL;
                PM->EstadoPronto->apontador = NULL;
            }
            else{ // Sobrará processos lá ainda
                auxiliar = PM->EstadoPronto;
                PM->EstadoPronto->anterior->prox = PM->EstadoPronto->prox; // Processo executando no momento
                PM->EstadoPronto->prox->anterior = PM->EstadoPronto->anterior;
                PM->EstadoPronto = PM->EstadoPronto->prox;
                free (auxiliar);
                if (PM->EstadoPronto->prox == PM->EstadoPronto){ // Se sobrar apenas um processo na fila de prontos
                    PM->EstadoPronto->prox = NULL;
                    PM->EstadoPronto->anterior = NULL;
                }
            }
           
            break;
        case 2: // Terminar um Processo (Instrução 'E' vindo de um processo simulado)
            // Tirar ele da lista de Prontos
            if (PM->EstadoPronto->prox == NULL){ // A lista de prontos irá ficar vazia depois que tirar o processo
                PM->EstadoPronto->anterior = NULL;
                PM->EstadoPronto->prox = NULL;
                PM->EstadoPronto->apontador = NULL;
            }
            else{ // Sobrará processos lá ainda
                auxiliar = PM->EstadoPronto;
                PM->EstadoPronto->anterior->prox = PM->EstadoPronto->prox; // Processo executando no momento
                PM->EstadoPronto->prox->anterior = PM->EstadoPronto->anterior;
                PM->EstadoPronto = PM->EstadoPronto->prox;
                free (auxiliar);
                if (PM->EstadoPronto->prox == PM->EstadoPronto){ // Se sobrar apenas um processo na fila de prontos
                    PM->EstadoPronto->prox = NULL;
                    PM->EstadoPronto->anterior = NULL;
                }
            }
            
            // Tirar ele da TabelaPcb
            if (PM->EstadoExecutando->apontador->anterior == NULL && PM->EstadoExecutando->apontador->prox == NULL){ // Não existem mais processos na Tabela
                free(PM->Tabela);
            }
            else if (PM->EstadoExecutando->apontador->prox == NULL){ // Ultimo Processo da fila
                auxiliar2 = PM->EstadoExecutando->apontador;
                auxiliar2->anterior->prox = NULL;
                free(auxiliar2);
            }
            else if (PM->EstadoExecutando->apontador->anterior == NULL){ // Primeiro processo da fila
                auxiliar2 = PM->EstadoExecutando->apontador;
                auxiliar2->prox->anterior = NULL;
                free(auxiliar2);
            }
            else{ //Processo no meio da fila
                auxiliar2 = PM->EstadoExecutando->apontador;
                auxiliar2->prox->anterior = auxiliar2->anterior;
                auxiliar2->anterior->prox = auxiliar2->prox;
                free(auxiliar2);
            }
            
            break;
        case 3: // Desbloquear um processo (Instrução 'U' vindo do Commander)
            //Atualizando Lista de Bloqueados
            if (PM->EstadoBloqueado->apontador == NULL){ // Não existem processos para serem desbloqueados
                printf ("Error, nao possuem processos para serem desbloqueados.\n");
            }
            else if(PM->EstadoBloqueado->prox == NULL){ // Não terá mais processos bloqueados
                auxiliar = PM->EstadoBloqueado;
                auxiliar->apontador->estado = 1; // Pronto
                PM->EstadoBloqueado->anterior = NULL;
            }
            else{ // Existem N processos na fila de Bloqueados
                auxiliar = PM->EstadoBloqueado;
                auxiliar->apontador->estado = 1;
                PM->EstadoBloqueado = PM->EstadoBloqueado->prox;
                auxiliar->prox->anterior = NULL;
            }

            // Atualizando Lista de Prontos
            if (PM->EstadoPronto->apontador == NULL){ // Não existem processos na fila de prontos
                PM->EstadoPronto->apontador = auxiliar->apontador;
                PM->EstadoPronto->prox = NULL;
                PM->EstadoPronto->anterior = NULL;
            }
            else if (PM->EstadoPronto->prox == NULL){ // Apenas um processo pronto
                PM->EstadoPronto->prox = (ApontaTabela*) calloc (1,sizeof(ApontaTabela));
                PM->EstadoPronto->anterior = PM->EstadoPronto->prox;
                PM->EstadoPronto->prox->prox = PM->EstadoPronto;
                PM->EstadoPronto->prox->anterior = PM->EstadoPronto;
                PM->EstadoPronto->prox->apontador = auxiliar->apontador;
            }
            else{ // Existem N processos Prontos na fila
                auxiliar3 = PM->EstadoPronto->anterior;
                PM->EstadoPronto->anterior = (ApontaTabela*) calloc (1,sizeof(ApontaTabela));
                PM->EstadoPronto->anterior->prox = PM->EstadoPronto;
                auxiliar3->prox = PM->EstadoPronto->anterior;
                PM->EstadoPronto->anterior->anterior = auxiliar3;
                PM->EstadoPronto->anterior->apontador = auxiliar->apontador;
            }
            
            break;
    }
}

void Troca_de_contexo(ProcessManager *PM){ // Altera o processo em execução
    // Guarda os dados da CPU na TabelaPBC. (EstadoExecutando->apontador aponta para o processo na tabela PCB)
    PM->EstadoExecutando->apontador->arquivo_do_programa = PM->CPU.apontador;
    PM->EstadoExecutando->apontador->tempo_utilizado = PM->CPU.tempo;
    PM->EstadoExecutando->apontador->inteiro = PM->CPU.valor_inteiro;
    PM->EstadoExecutando->apontador->contador_de_programa = PM->CPU.contador_de_programa;
    // Substituicao de processo para executar
    if (PM->EstadoPronto->prox != NULL){ // Se existir outro processo muda o processo executando, do contrario mantem o unico processo da fila
        PM->EstadoExecutando->apontador = PM->EstadoPronto->prox->apontador;
        PM->EstadoPronto = PM->EstadoPronto->prox;
        PM->CPU.apontador = PM->EstadoExecutando->apontador->arquivo_do_programa;
        PM->CPU.contador_de_programa = PM->EstadoExecutando->apontador->contador_de_programa;
        PM->CPU.tempo = PM->EstadoExecutando->apontador->tempo_utilizado;
        PM->CPU.valor_inteiro = PM->EstadoExecutando->apontador->inteiro;
    }
}

void Escalonador(ProcessManager *PM){ // Politica adotada: Tipo FIFO ignorando prioridade
    Troca_de_contexo(PM);
}


// _________________________________________________ FIM FUNÇÕES _________________________________________________ //



int main() {
    FILE *init, *simulados;
    init = fopen ("init.txt","r");
    char string;
    char valor1, str[30];
    int valor2, contador = 0;
    struct ProcessManager PM;
    PM = Inicializa_Dados(init);
    do {
        scanf("%c", &string);
        switch(string){
            case 'Q':
                fscanf(PM.CPU.apontador, "%c",&valor1);
                switch(valor1){
                    case 'S':
                        fscanf(PM.CPU.apontador, "%i",&valor2);
                        PM.CPU.valor_inteiro = valor2;
                        break;
                    case 'A':
                        fscanf(PM.CPU.apontador, "%i",&valor2);
                        PM.CPU.valor_inteiro += valor2;
                        break;
                    case 'D':
                        fscanf(PM.CPU.apontador, "%i",&valor2);
                        PM.CPU.valor_inteiro -= valor2;
                        break;
                    case 'B':
                        //Gerenciamento_de_transicao(&PM,1);
                        break;
                    case 'E':
                        //Gerenciamento_de_transicao(&PM,2);
                        break;
                    case 'F':
                        //Criacao_de_processo(&PM);
                        fscanf(PM.CPU.apontador, "%i",&valor2);
                        while(contador != valor2){
                            fscanf (PM.CPU.apontador, "%s", str);
                            contador++;
                        }
                        contador = 0;
                        break;
                    case 'R':
                       // fscanf (PM.CPU.apontador, "%s",str);
                      //  simulados = fopen(str,"r");
                    //    Troca_de_Imagem(&PM,simulados);
                        break;
                }
                if (PM.EstadoPronto->apontador != NULL){
                    //Escalonador(PM);

                }else{
                    printf ("\nNão possuem processos para serem executados. Aguardando um processo ser desbloquado.\n");
                }
                break;
            case 'U':
                printf ("Desbloquear processo simulado.\n");
                // Gerenciamento_de_transicao(&PM,3);
                break;
            case 'P': // Cria Reporter
                printf ("Imprimir estado atual do sistema.\n");
                // Chamada do Reporter
                break;
            case 'T': // Cria outro Reporter
                // ApagaDados(&PM);
                // Chamda outro Reporter
                printf ("Fechar programa.\n");
                break;
        }
    } while(string != 'T');
}
