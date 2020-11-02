#define Tempo PM.CPU.Tempo_Atual
#define Ex auxiliar->apontador
#define Eb auxiliar->apontador
#define Ep auxiliar->apontador
#define ip id_pai
#define p prioridade
#define in inteiro
#define ti tempo_inicial
#define tu tempo_utilizado


typedef struct Vetor{
    char instrucao;
    int valor;
    char str[30];
    struct Vetor *prox;
}Arquivo;

typedef struct CPU{
    int valor_inteiro;
    int tempo; // Tempo do processo
    Arquivo *apontador; // Arquivo a ser executado
    int contador_de_programa;
    int Tempo_Atual; // Tempo total
}CPU;

typedef struct TabelaPcb{
    int id;
    int id_pai;
    int contador_de_programa;
    Arquivo *arquivo_do_programa;
    int inteiro;
    int prioridade;
    int estado; // 0 - Bloqueado // 1 - Pronto // 2 - Executando
    int tempo_inicial;
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

void Guarda_Arquivo(ProcessManager *PM, FILE *input);
ProcessManager Inicializa_Dados(FILE *init);
void Criacao_de_processo(ProcessManager *PM);
void Troca_de_Imagem(ProcessManager *PM, FILE *input);
void Gerenciamento_de_transicao(ProcessManager *PM, int value);
void Troca_de_contexo(ProcessManager *PM);
void Escalonador(ProcessManager *PM, char validade);
int Reportar (ProcessManager *PM);