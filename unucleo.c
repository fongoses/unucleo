#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "unucleo.h"
#include "fifo.h"

/* Estruturas que armazenam os processos nos diferentes estados */
PCB         executando;                                                         // Como s� um processo usa o processador por vez, o estado executando � apenas um PCB
FIFO_DESC   fifo_aptos[PRIORIDADES];                                            // Os processos APTOS s�o separados em filas por prioridades
FIFO_DESC   fifo_bloqs;                                                         // Os processos bloqueados ficam numa lista

/* Vari�veis globais que armazenam inform��es do unucleo */
ucontext_t scheduler_context;                                                   // Armazena o contexto do escalonador para retorno das fun��es
int        pid_count;                                                            // N�mero de processos ativos, usado para gerar o pr�ximo PID

/* Fun��es reservadas do sistema operacional - INICIO */
//Verifica se tinha algum processo esperando o processo que esta executando, retorna -1 se n�o houver ninguem
int verifica_waiters(void) {
    int ret=-1;                                                                 // pid do waiter, retorna -1 se n�o existir
    printf("procurando waiters ### xx ###");                                    // debug
    if(fifo_bloqs.prim != NULL)
        ret = has_waiter(&fifo_bloqs,executando.pid);                           // verifica se possui alguem esperando pelo processo que esta no estado running
    return ret;
}

void troca_bloq_to_ready(void) {
    PCB pcb_bloq;
    int pid_bloq = verifica_waiters();
    while(pid_bloq>-1) {

        printf("procurando waiters ### %d ###",pid_bloq);                       // debug
        if(busca_pcb(&fifo_bloqs,pid_bloq,&pcb_bloq)!=-1) {                     // armazena pcb que vai ser liberado na variavel

            remove_pcb(&fifo_bloqs,pid_bloq);                                   // exclui determinado PCB da lista de bloqueados
            insere_fifo(&fifo_aptos[pcb_bloq.prio], pcb_bloq);
        }
        pid_bloq = verifica_waiters();
    }
}

// verifica proximo pcb que dever� ser executado
int pcb_caller(PCB* pcb_result) {
    if(fifo_aptos[P_ALTA].prim!=NULL) {
        *pcb_result = fifo_aptos[P_ALTA].prim->pcb;
        remove_fifo(&fifo_aptos[P_ALTA]);
    } else if(fifo_aptos[P_MEDIA].prim!=NULL) {
        *pcb_result = fifo_aptos[P_MEDIA].prim->pcb;
        remove_fifo(&fifo_aptos[P_MEDIA]);
    } else if(fifo_aptos[P_BAIXA].prim!=NULL) {
        *pcb_result = fifo_aptos[P_BAIXA].prim->pcb;
        remove_fifo(&fifo_aptos[P_BAIXA]);
    } else {
        return -1;                                                              // todas as filas est�o vazias
    }
    return pcb_result->pid;
}

int pcb_fim(void) {
    PCB pcb_ready;
    printf("\n\nVerifica fim.");
    troca_bloq_to_ready();                                                      // Processos que estavam esperando o processo em execu��o s�o liberados aqui

    if(pcb_caller(&pcb_ready)<0)
        return -1;

    executando = pcb_ready;
    return 0;
}
/* Fun��es reservadas do sistema operacional - FIM */

/* Inicializa o unucleo.
   Retorna "0" se inicializou corretamente ou "-1" caso contr�rio. */
int libsisop_init(void) {
    int i;
    pid_count = 1;

    cria_fifo(&fifo_bloqs);                                                     // Cria uma fila de bloqueados
    for(i = 0;i<PRIORIDADES-1;i++)                                              // Cria uma fila para cada prioridade
        cria_fifo(&fifo_aptos[i]);

    //getcontext(&scheduler_context);

	return 0;
}

/* Cria um novo processo de usu�rio com a prioridade "prio".
   A prioridade deve ser 1 ou 2 sen�o a fun��o retorna erro.
   O c�digo a ser executado � apontado por "start_routine".
   Retorna o PID (Process Identification) do processo ou "-1" em caso de erro. */
int mproc_create(int prio, void *(*start_routine)(void*), void *arg) {
    PCB pcb_add;

    if (prio<1 || prio>2) {
 		printf("Sem permiss�o para inserir processos com prioridade de sistema (0). \n");
		return -1;
	}
	pcb_add.pid = pid_count;
    pid_count++;

	pcb_add.prio= prio;
	pcb_add.pid_wait = -1;
    
    printf("CALL: mproc_create() -> getcontext(&pcb_fifo->contexto)\n");
    getcontext(&pcb_add.contexto);
    pcb_add.contexto.uc_link           = &scheduler_context;
    pcb_add.contexto.uc_stack.ss_sp    = (char *)malloc(SIGSTKSZ*sizeof(char));
	pcb_add.contexto.uc_stack.ss_size  = SIGSTKSZ*sizeof(char);

    printf("CALL: mproc_create() -> makecontext(&pcb_fifo->contexto, (void (*)(void)) start_routine, 1, arg)\n");
	makecontext(&pcb_add.contexto, (void (*)(void)) start_routine, 1, arg);

    printf("CALL: mproc_create() -> insere_fifo(&fifo_aptos[prio], pcb_add)\n");
    insere_fifo(&fifo_aptos[prio], pcb_add);

	return pcb_add.pid;
}

/* O processo retorna o controle ao escalonador (ced�ncia volunt�ria)
   e vai para o final da fila de APTOS para executar. */
void mproc_yield(void) {
    PCB prox_pcb, ant_pcb;

    if (pcb_caller(&prox_pcb)>-1) {
        insere_fifo(&fifo_aptos[executando.prio], executando);
        ant_pcb    = executando;
        executando = prox_pcb;
        swapcontext(&ant_pcb.contexto,&executando.contexto);
    }
}

/* Aguarda at� que o processo identificado por "pid" seja encerrado.
   Retorna "0" se funcionou corretamente ou "-1" caso contr�rio. */
int mproc_join(int pid) {
//    PCB prox_pcb, ant_pcb;
//
//    if (pcb_caller(&prox_pcb)>-1) {
//        executando.pid_wait = pid;
//        ant_pcb    = executando;
//        executando = prox_pcb;
//
//        insere_fifo(&fifo_bloqs,ant_pcb);
//        swapcontext(&ant_pcb.contexto,&executando.contexto);
//    }

    executando.pid_wait = pid;

    insere_fifo(&fifo_bloqs,executando);
    swapcontext(&executando.contexto,&scheduler_context);

    return 0;
}

/* Inicia o esclonador de processos, deve ser chamado apenas na fun��o
   main() ap�s as chamadas de mproc_create() */
void scheduler(void) {
    getcontext(&scheduler_context);
    printf("SCHEDULER\n");
    if(pcb_fim()<0)
        return;
    //while(pcb_fim()>=0) {
        //setcontext(&executando.contexto);
        //swapcontext(&scheduler_context,&executando.contexto);
        printf("\npassou depois\n");
     //   }
    setcontext(&executando.contexto);
}