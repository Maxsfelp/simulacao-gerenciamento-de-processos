#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "manager.h"

// __________________________________________________ FUNÇÕES __________________________________________________ //

void Guarda_Arquivo(ProcessManager *PM, FILE *input){ // Guarda o arquivo na CPU
    char str[30], str1[30];
    int i;
    Arquivo *auxiliar;
    fscanf (input, "%s",str);
    PM->CPU.apontador->instrucao = str[0];
    if (str[0] == 'R'){
        fscanf (input, "%s",str1);
        strcpy (PM->CPU.apontador->str, str1);
    }else if (str[0] != 'B' && str[0] != 'E'){
        fscanf (input, "%i",&i);
        PM->CPU.apontador->valor = i;
    }
    auxiliar = PM->CPU.apontador;
    auxiliar->prox = NULL;
    while(!feof(input)){
        auxiliar->prox = (Arquivo*) calloc (1,sizeof(Arquivo));
        auxiliar = auxiliar ->prox;
        auxiliar -> prox = NULL;
        fscanf (input, "%s",str);
        auxiliar->instrucao = str[0];
        if (str[0] == 'R'){
        fscanf (input, "%s",str1);
        strcpy (auxiliar->str, str1);
        }else if (str[0] != 'B' && str[0] != 'E'){
            fscanf (input, "%i",&i);
            auxiliar->valor = i;
        }
    }
}

ProcessManager Inicializa_Dados(FILE *init){
    ProcessManager PM;
    // ProcessManager
    PM.qtd = 0;
    PM.Tabela = (TabelaPcb*) calloc (1, sizeof(TabelaPcb));
    PM.EstadoBloqueado = (ApontaTabela*) calloc (1, sizeof(ApontaTabela));
    PM.EstadoPronto = (ApontaTabela*) calloc (1, sizeof(ApontaTabela));
    PM.EstadoExecutando = (ApontaTabela*) calloc (1, sizeof(ApontaTabela));
    PM.Tabela->arquivo_do_programa = (Arquivo*) calloc(1,sizeof(Arquivo));
    // Tabela
    PM.Tabela->anterior = NULL;
    PM.Tabela->contador_de_programa = 0;
    PM.Tabela->estado = 2;
    PM.Tabela->id = 0;
    PM.Tabela->id_pai = 0;
    PM.Tabela->inteiro = 0;
    PM.Tabela->prioridade = 0;
    PM.Tabela->prox = NULL;
    PM.Tabela->tempo_inicial = 0;
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
    PM.CPU.apontador = PM.EstadoExecutando->apontador->arquivo_do_programa;
    Guarda_Arquivo(&PM,init);
    PM.EstadoExecutando->apontador->arquivo_do_programa = PM.CPU.apontador;
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
        auxiliar->arquivo_do_programa = PM->EstadoExecutando->apontador->arquivo_do_programa->prox; // Executa na proxima instrução
        auxiliar->contador_de_programa = 0;
        auxiliar->estado = 1; // Pronto
        auxiliar->id_pai = PM->EstadoExecutando->apontador->id;
        auxiliar->id = PM->qtd;
        auxiliar->inteiro = 0;
        auxiliar->prioridade = PM->EstadoExecutando->apontador->prioridade;
        auxiliar->tempo_inicial = PM->CPU.Tempo_Atual;
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
        auxiliar->arquivo_do_programa = PM->EstadoExecutando->apontador->arquivo_do_programa->prox;
        auxiliar->contador_de_programa = 0;
        auxiliar->estado = 1; // Pronto
        auxiliar->id_pai = PM->EstadoExecutando->apontador->id;
        auxiliar->id = PM->qtd;
        auxiliar->inteiro = 0;
        auxiliar->prioridade = PM->EstadoExecutando->apontador->prioridade;
        auxiliar->tempo_inicial = PM->CPU.Tempo_Atual;
        auxiliar->tempo_utilizado = 0;
        PM->qtd += 1;
    }
    /* Adicionar processo na fila de prontos (auxiliar aponta para o processo adicionado na tabela pcb)
     A criação foi feita usando uma fila com circulo, ou seja, o ultimo aponta pro primeiro e o primeiro tem um apontador pro ultimo*/
    if (PM->EstadoPronto->apontador == NULL){ // Nenhum processo Pronto
        PM->EstadoPronto->apontador = auxiliar;
        PM->EstadoPronto->anterior = NULL;
        PM->EstadoPronto->prox = NULL;
    }  
    else if (PM->EstadoPronto->prox == NULL){ // Apenas um processo pronto
        // Monta um circulo
        PM->EstadoPronto->prox = (ApontaTabela*) calloc (1,sizeof(ApontaTabela));
        PM->EstadoPronto->anterior = PM->EstadoPronto->prox;
        PM->EstadoPronto->prox->prox = PM->EstadoPronto;
        PM->EstadoPronto->prox->anterior = PM->EstadoPronto;
        PM->EstadoPronto->prox->apontador = auxiliar;
    }
    else{ // Existem n processos prontos
        auxiliar2 = PM->EstadoPronto->anterior;
        PM->EstadoPronto->anterior = (ApontaTabela*) calloc (1,sizeof(ApontaTabela));
        PM->EstadoPronto->anterior->prox = PM->EstadoPronto;
        auxiliar2->prox = PM->EstadoPronto->anterior;
        PM->EstadoPronto->anterior->anterior = auxiliar2;
        PM->EstadoPronto->anterior->apontador = auxiliar;
    }
}

void Troca_de_Imagem(ProcessManager *PM, FILE *input){ // Altera o programa de execução do processo (R)
    free(PM->CPU.apontador); // Tira o que tinha na CPU
    Guarda_Arquivo(PM,input); // Guarda o novo arquivo na CPU 
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
            PM->EstadoExecutando->apontador->arquivo_do_programa =  PM->EstadoExecutando->apontador->arquivo_do_programa->prox;
           
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
                if (PM->EstadoPronto->prox == PM->EstadoPronto){ // Se sobrar apenas um processo na fila de prontos
                    PM->EstadoPronto->prox = NULL;
                    PM->EstadoPronto->anterior = NULL;
                }
            }
            // Tirar ele da TabelaPcb (EstadoExecutando->apontador = TabelaPcb)
            if (PM->EstadoExecutando->apontador->anterior == NULL && PM->EstadoExecutando->apontador->prox == NULL){ // Não existem mais processos na Tabela
                free(PM->Tabela);
                PM->CPU.Tempo_Atual =+1;
                PM->Tabela = NULL;
            }
            else if (PM->EstadoExecutando->apontador->prox == NULL){ // Ultimo Processo na Tabela
                auxiliar2 = PM->EstadoExecutando->apontador;
                auxiliar2->anterior->prox = NULL;
                free(auxiliar2);
            }
            else if (PM->EstadoExecutando->apontador->anterior == NULL){ // Primeiro processo na Tabela
                PM->Tabela = PM->Tabela->prox;
                auxiliar2 = PM->EstadoExecutando->apontador;
                auxiliar2->prox->anterior = NULL;
                free(auxiliar2);
            }
            else{ //Processo no meio da Tabela
                auxiliar2 = PM->EstadoExecutando->apontador;
                auxiliar2->prox->anterior = auxiliar2->anterior;
                auxiliar2->anterior->prox = auxiliar2->prox;
                free(auxiliar2);
            }
            PM->CPU.apontador = NULL;
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
                auxiliar->apontador->estado = 1; // Pronto
                PM->EstadoBloqueado = PM->EstadoBloqueado->prox;
                auxiliar->prox->anterior = NULL;
            }

            // Atualizando Lista de Prontos
            if (PM->EstadoBloqueado->apontador != NULL && PM->EstadoPronto->apontador == NULL && PM->EstadoBloqueado->apontador != NULL){ // Não existem processos na fila de prontos
                PM->EstadoPronto->apontador = auxiliar->apontador;
                PM->EstadoPronto->prox = NULL;
                PM->EstadoPronto->anterior = NULL;
            }
            else if (PM->EstadoBloqueado->apontador != NULL && PM->EstadoPronto->prox == NULL && PM->EstadoBloqueado->apontador != NULL){ // Apenas um processo pronto
                PM->EstadoPronto->prox = (ApontaTabela*) calloc (1,sizeof(ApontaTabela));
                PM->EstadoPronto->anterior = PM->EstadoPronto->prox;
                PM->EstadoPronto->prox->prox = PM->EstadoPronto;
                PM->EstadoPronto->prox->anterior = PM->EstadoPronto;
                PM->EstadoPronto->prox->apontador = auxiliar->apontador;
            }
            else if (PM->EstadoBloqueado->apontador != NULL && PM->EstadoBloqueado->apontador != NULL){ // Existem N processos Prontos na fila
                auxiliar3 = PM->EstadoPronto->anterior;
                PM->EstadoPronto->anterior = (ApontaTabela*) calloc (1,sizeof(ApontaTabela));
                PM->EstadoPronto->anterior->prox = PM->EstadoPronto;
                auxiliar3->prox = PM->EstadoPronto->anterior;
                PM->EstadoPronto->anterior->anterior = auxiliar3;
                PM->EstadoPronto->anterior->apontador = auxiliar->apontador;
            }

            //Reatualizando a Lista de Bloqueado
            if (PM->EstadoBloqueado->apontador != NULL && auxiliar->prox == NULL && PM->EstadoBloqueado->apontador != NULL){ // Não terá mais processos bloqueados
                auxiliar->apontador = NULL;
            }
            else if (PM->EstadoBloqueado->apontador != NULL && auxiliar != PM->EstadoBloqueado && PM->EstadoBloqueado->apontador != NULL){ // Apagando o processo que foi removido
                free(auxiliar);
            }
            if (PM->EstadoBloqueado->apontador != NULL && PM->CPU.apontador == NULL){ // Se nao tiver nenhum processo executando, o processo desbloqueado será executado
                PM->EstadoExecutando->apontador = PM->EstadoPronto->apontador;
                PM->EstadoExecutando->apontador->estado = 2;
                PM->CPU.apontador = PM->EstadoExecutando->apontador->arquivo_do_programa;
                PM->CPU.contador_de_programa = PM->EstadoExecutando->apontador->contador_de_programa;
                PM->CPU.tempo = PM->EstadoExecutando->apontador->tempo_utilizado;
                PM->CPU.valor_inteiro = PM->EstadoExecutando->apontador->inteiro;
            }
            break;
    }
}

void Troca_de_contexo(ProcessManager *PM){ // Altera o processo em execução
    // Guarda os dados da CPU na TabelaPBC. (EstadoExecutando->apontador aponta para o processo na TabelaPcb)
    if (PM->CPU.apontador != NULL){
        PM->EstadoExecutando->apontador->arquivo_do_programa = PM->CPU.apontador;
        PM->EstadoExecutando->apontador->tempo_utilizado = PM->CPU.tempo;
        PM->EstadoExecutando->apontador->inteiro = PM->CPU.valor_inteiro;
        PM->EstadoExecutando->apontador->contador_de_programa = PM->CPU.contador_de_programa;
    }
    // Substituicao de processo para executar
    if (PM->EstadoPronto->prox != NULL && PM->CPU.apontador != NULL){ // Se existir outro processo muda o processo executando, do contrario mantem o unico processo da fila
        PM->EstadoExecutando->apontador->estado = 1;
        PM->EstadoExecutando->apontador = PM->EstadoPronto->prox->apontador;
        PM->EstadoExecutando->apontador->estado = 2;
        PM->EstadoPronto = PM->EstadoPronto->prox;
        PM->CPU.apontador = PM->EstadoExecutando->apontador->arquivo_do_programa;
        PM->CPU.contador_de_programa = PM->EstadoExecutando->apontador->contador_de_programa;
        PM->CPU.tempo = PM->EstadoExecutando->apontador->tempo_utilizado;
        PM->CPU.valor_inteiro = PM->EstadoExecutando->apontador->inteiro;
    }else if (PM->CPU.apontador == NULL){
        PM->EstadoExecutando->apontador = PM->EstadoPronto->apontador;
        PM->EstadoExecutando->apontador->estado = 2;
        PM->CPU.apontador = PM->EstadoExecutando->apontador->arquivo_do_programa;
        PM->CPU.contador_de_programa = PM->EstadoExecutando->apontador->contador_de_programa;
        PM->CPU.tempo = PM->EstadoExecutando->apontador->tempo_utilizado;
        PM->CPU.valor_inteiro = PM->EstadoExecutando->apontador->inteiro;
    }
}

void Escalonador(ProcessManager *PM, char validade){ // Politica adotada: Fila Circular, sem reorganização dos processos por prioridade.
    if ((PM->CPU.apontador != NULL || validade == 'E') && (PM->Tabela != NULL)){
        PM->CPU.Tempo_Atual+=1; // Tempo do Sistema
        if (validade != 'R' && PM->CPU.apontador != NULL){
            PM->CPU.tempo+=1; // Tempo do Processo na CPU
            PM->CPU.apontador = PM->CPU.apontador->prox; // Proxima instrução   
            PM->CPU.contador_de_programa+=1; // Quantas instruções ja foram executadas
        }else if (PM->CPU.apontador != NULL){
            PM->CPU.tempo+=1; // Tempo do Processo na CPU
            PM->CPU.contador_de_programa = 0;
        }
    }
    if (PM->EstadoPronto->apontador != NULL){ // Se ainda houver processos que podem ser executados
        Troca_de_contexo(PM);
    }
    else if (PM->EstadoBloqueado->apontador != NULL){ // Existem processos, mas todos eles estão bloqueados
        PM->CPU.apontador = NULL;
        printf ("\nNão há processos prontos para serem executados. Aguardando desbloquear algum processo.\n");
    }
    else{ // Não possuem mais processos, nem prontos, nem bloqueados
        PM->CPU.apontador = NULL;
        printf ("\nNão existem mais processos no programa.\n");
    }
}

int Reportar (ProcessManager *PM){
    int verifica = 0;
    ApontaTabela *auxiliar, *auxiliar2;
    printf ("****************************************************************\n");
    printf ("Estado do Sistema:\n");
    printf ("****************************************************************\n");
    printf ("TEMPO ATUAL: %i \n",PM->CPU.Tempo_Atual);
    if (PM->CPU.apontador != NULL){
        printf ("PROCESSO EXECUTANDO: \n");
        auxiliar = PM->EstadoExecutando;
        printf ("pid: %i, ppid: %i, prioridade: %i, valor: %i, tempo inicio: %i, CPU usada até agora: %i\n",Ex->id, Ex->ip, Ex->p, Ex->in, Ex->ti, Ex->tu);
    }else{
        printf ("NAO TEM NENHUM PROCESSO EXECUTANDO. CPU OCIOSA.\n");
    }
    if (PM->EstadoBloqueado->apontador != NULL){
        printf ("BLOQUEADO:\n");
        auxiliar = PM->EstadoBloqueado;
        while (auxiliar->prox != NULL){
            printf ("pid: %i, ppid: %i, prioridade: %i, valor: %i, tempo inicio: %i, CPU usada até agora: %i\n",Eb->id, Eb->ip, Eb->p, Eb->in, Eb->ti, Eb->tu);
            auxiliar = auxiliar->prox;
        }
        printf ("pid: %i, ppid: %i, prioridade: %i, valor: %i, tempo inicio: %i, CPU usada até agora: %i\n",Eb->id, Eb->ip, Eb->p, Eb->in, Eb->ti, Eb->tu);
    }
    else{
        printf ("NÃO HÁ PROCESSOS BLOQUEADOS\n");
    }
    if (PM->EstadoPronto->apontador != NULL){
        printf ("PROCESSOS PRONTOS:\n");
        auxiliar = PM->EstadoPronto;
        if (PM->EstadoPronto->anterior != NULL){
            auxiliar2 = PM->EstadoPronto->anterior;
            while (verifica == 0){
                printf ("pid: %i, ppid: %i, prioridade: %i, valor: %i, tempo inicio: %i, CPU usada até agora: %i\n",Ep->id, Ep->ip, Ep->p, Ep->in, Ep->ti, Ep->tu);
                auxiliar = auxiliar->prox;
                if (auxiliar->apontador->id == auxiliar2->apontador->id){
                    verifica = 1;
                }
            }
        }
        printf ("pid: %i, ppid: %i, prioridade: %i, valor: %i, tempo inicio: %i, CPU usada até agora: %i\n",Ep->id, Ep->ip, Ep->p, Ep->in, Ep->ti, Ep->tu);
    }
    else{
        printf ("NÃO HÁ PROCESSOS PRONTOS\n");
    }
    printf ("****************************************************************\n");
    return 0;
}
// _________________________________________________ FIM FUNÇÕES _________________________________________________ //



int main() {
    FILE *init, *simulados;
    init = fopen ("init","r"); // ARRUMAR ESSA LINHA NO LINUX, TIRAR O .TXT
    char string;
    char valor1, arquivo[30];
    int valor2, contador = 0;
    struct ProcessManager PM;
    PM = Inicializa_Dados(init);
    do {
        scanf("%c", &string);
        switch(string){
            case 'Q': // Executar Unidade de Tempo
                if (PM.CPU.apontador != NULL){
                    valor1 = PM.CPU.apontador->instrucao;
                    switch(valor1){
                        case 'S': // Atribuir valor
                            valor2 = PM.CPU.apontador->valor;
                            PM.CPU.valor_inteiro = valor2;
                            break;
                        case 'A': // Somar valor ao inteiro
                            valor2 = PM.CPU.apontador->valor;
                            PM.CPU.valor_inteiro += valor2;
                            break;
                        case 'D': // Subtrair valor ao inteiro
                            valor2 = PM.CPU.apontador->valor;
                            PM.CPU.valor_inteiro -= valor2;
                            break;
                        case 'B': // Bloquear o processo executando
                            Gerenciamento_de_transicao(&PM, 1);
                            break;
                        case 'E': // Terminar o processo executando
                            Gerenciamento_de_transicao(&PM, 2);
                            break;
                        case 'F': // Criar um novo Processo
                            Criacao_de_processo(&PM);
                            valor2 = PM.CPU.apontador->valor;
                            PM.CPU.contador_de_programa += valor2;
                            contador = 0;
                            while(contador < valor2){ // Percorrer as 'n' linhas da instrução F
                            PM.CPU.apontador = PM.CPU.apontador->prox;
                            contador++;
                            }
                            break;
                        case 'R': // Trocar o arquivo que será executado pelo processo
                            strcpy (arquivo, PM.CPU.apontador->str);
                            //strcat(arquivo,".txt"); // COMENTAR ESSA LINHA NO LINUX 
                            simulados = fopen(arquivo, "r");
                            Troca_de_Imagem(&PM, simulados);
                            break;
                    }
                }
                Escalonador(&PM,valor1); // Após executar a instrução Q, escalonar um novo processo (se possível)
                break;
            case 'U': // Desbloquear primeiro processo simulado
                Gerenciamento_de_transicao(&PM, 3);
                break;
            case 'P': // Cria Processo Reporter
                Reportar(&PM);
                break;
            case 'T': 
                Reportar(&PM);
                free(PM.Tabela);
                free(PM.EstadoPronto);
                free(PM.EstadoBloqueado);
                free(PM.EstadoExecutando);
                break;            
        }
    } while(string != 'T');
}
