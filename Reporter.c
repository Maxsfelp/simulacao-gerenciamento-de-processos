#include <stdio.h>
#include <stdlib.h>

#define Ex PM.Tabela[PM.EstadoExecutando]
#define Eb PM.Tabela[PM.EstadoBloqueado[i]]
#define Ep PM.Tabela[PM.EstadoPronto[i]]
#define ip id_pai
#define p prioridade
#define in inteiro
#define ti tempo_inicial
#define tu tempo_utilizado

int main(){
    printf ("****************************************************************\n");
    printf ("Estado do Sistema:");
    printf ("****************************************************************\n");
    printf ("TEMPO ATUAL: %i \n",PM.Tempo);
    printf ("PROCESSO EXECUTANDO: \n");
    printf ("pid: %i, ppid: %i, prioridade: %i, valor: %i, tempo inicio: %i, CPU usada até agora: %i\n",Ex.id, Ex.ip, Ex.p, Ex.in, Ex.ti, Ex.tu);
    printf ("BLOQUEADO:\n");
    printf ("Fila processos bloqueados:\n");
    for (int i = 0 ; PM.EstadoBloqueado[i] != -1; i++){
        printf ("pid: %i, ppid: %i, prioridade: %i, valor: %i, tempo inicio: %i, CPU usada até agora: %i\n",Eb.id, Eb.ip, Eb.p, Eb.in, Eb.ti, Eb.tu);
    }
    printf ("PROCESSOS PRONTOS:\n");
    for (int i = 0; PM.EstadoPronto[i] != -1; i++){
        printf ("pid: %i, ppid: %i, prioridade: %i, valor: %i, tempo inicio: %i, CPU usada até agora: %i\n",Ep.id, Ep.ip, Ep.p, Ep.in, Ep.ti, Ep.tu);
    }
    printf ("****************************************************************\n");
}