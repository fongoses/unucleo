#include <ucontext.h>

/* Estrutura que armazena os dados do PCB */
typedef struct PCB {
    int pid;
    ucontext_t contexto;
    int prio;
    int estado;
    int pid_wait; // Processo que ele aguardar� o termino
    //char all_stack[SIGSTKSZ];
    //char* all_stack;
} PCB;

/* Elemento da Fila de PCBs */
typedef struct FIFO_PCB {
    PCB pcb;
    struct FIFO_PCB* prox;
}FIFO_PCB;

/* Fila de PCBs */
typedef struct FIFO_DESC {
    FIFO_PCB* prim;
    FIFO_PCB* ult;
}FIFO_DESC;

/* Fun��es gerais para controle da Fila */
void cria_fifo(FIFO_DESC* f);
int insere_fifo(FIFO_DESC* f, PCB p);
FIFO_PCB* consulta_fifo(FIFO_DESC* f);
int remove_fifo(FIFO_DESC* f);
void destroi_fifo(FIFO_DESC* f);

/* Fun��es de DEBUG */
void imprime_pcb(PCB p);
void imprime_fifo(FIFO_DESC* f);

/* Fun��es para manipula��o das filas em fun��o dos PCBs */
int has_waiter(FIFO_DESC* f, int pid);                                          //verifica se a fila possui algum elemento com pid_wait=pid, se sim, retorna o pid desse cara, sen�o retorna -1
int busca_pcb(FIFO_DESC* f, int pid, PCB* result);                              //percorre fila buscando PCB com  pid=pid, resultado � armazenado em result
int remove_pcb(FIFO_DESC* f, int pid);                                          //remove pcb especifico