#include <stdio.h>
#include <stdlib.h>
#include "unucleo.h"

void teste4();

void teste1() {
    printf("INICIO: teste1()\n");

    int i = 30;

    printf("CALL: teste1() -> mproc_create(2,(void*)teste4,0)\n");
    int p = mproc_create(2,(void*)teste4,0);
    printf("RET: teste1() -> mproc_create(2,(void*)teste4,0) = %d\n", p);

    for(;i>0;i--) {
        printf("Sou o teste 1 de prio 1 e vou executar mais %d vezes (chamei %d)\n",i,p);
        if (i==15) {
            printf("CALL: teste1() -> mproc_join(3)\n");
            mproc_join(3);
        }
    }

    printf("FIM: teste1()\n");
}

void teste2() {
    printf("INICIO: teste2()\n");
    
    int i = 25;
    for(;i>0;i--) {
        printf("Sou o teste 2 de prio 1 e vou executar mais %d vezes\n",i);
        if (i==5) {
            printf("CALL: teste2() -> mproc_yeld()\n");
            mproc_yield();
        }
    }

    printf("FIM: teste2()\n");
}

void teste5();

void teste3() {
    printf("INICIO: teste3()\n");

    int i = 20;

    printf("CALL: teste3() -> mproc_create(2,(void*)teste5,0)\n");
    int p = mproc_create(2,(void*)teste5,0);
    printf("RET: teste3() -> mproc_create(2,(void*)teste5,0) = %d\n", p);

    for(;i>0;i--) {
        printf("Sou o teste 3 de prio 1 e vou executar mais %d vezes (chamei %d)\n",i,p);
    }

    printf("FIM: teste3()\n");
}

void teste4() {
    printf("INICIO: teste4()\n");

    int i = 15;
    for(;i>0;i--) {
        printf("Sou o teste 4 de prio 2 e vou executar mais %d vezes\n",i);
    }

    printf("FIM: teste4()\n");
}

void teste6() {
    printf("INICIO: teste6()\n");

    int i = 15;
    for(;i>0;i--) {
        printf("Sou o teste 6 de prio 2 e vou executar mais %d vezes\n",i);
    }

    printf("FIM: teste6()\n");
}

void teste5() {
    printf("INICIO: teste5()\n");

    int i = 15;

    printf("CALL: teste5() -> mproc_create(2,(void*)teste6,0)\n");
    int p = mproc_create(1,(void*)teste6,0);
    printf("RET: teste5() -> mproc_create(2,(void*)teste6,0) = %d\n", p);

    for(;i>0;i--) {
        printf("Sou o teste 5 de prio 2 e vou executar mais %d vezes(chamei %d)\n",i,p);
    }

    printf("FIM: teste5()\n");
}

void teste7() {
    printf("INICIO: teste7()\n");

    int i = 9;
    for(;i>0;i--) {
        printf("Sou o teste 7 de prio 1 e vou executar mais %d vezes\n",i);
    }

    printf("FIM: teste7()\n");
}

void teste8() {
    printf("INICIO: teste8()\n");

    int i = 8;
    for(;i>0;i--) {
        printf("Sou o teste 8 de prio 1 e vou executar mais %d vezes\n",i);
    }

    printf("FIM: teste8()\n");
}

void teste9() {
    printf("INICIO: teste9()\n");

    int i = 7;
    for(;i>0;i--) {
        printf("Sou o teste 9 de prio 1 e vou executar mais %d vezes\n",i);
    }

    printf("FIM: teste9()\n");
}

void teste10() {
    printf("INICIO: teste10()\n");

    int i = 6;
    for(;i>0;i--) {
        printf("Sou o teste 10 de prio 1 e vou executar mais %d vezes\n",i);
    }

    printf("FIM: teste10()\n");
}

int main() {
    printf("INICIO: main()\n");
    
    /****************
     * Inicializa�o *
     ****************/
    printf("CALL: main() -> libsisop_init()\n");
    int init = libsisop_init();
    printf("RET: main() -> libsisop_init() = %d\n", init);
    /* Fim Inicializa��o */
    
    
    /*************************
     * Cria��o dos Processos *
     *************************/  
    printf("CALL: main() -> mproc_create(1,(void*)teste1,0)\n");
    int p1 = mproc_create(1,(void*)teste1,0);
    printf("RET: main() -> mproc_create(1,(void*)teste1,0) = %d\n", p1);

    printf("CALL: main() -> mproc_create(1,(void*)teste2,0)\n");
    int p2 = mproc_create(1,(void*)teste2,0);
    printf("RET: main() -> mproc_create(1,(void*)teste2,0) = %d\n", p2);

    printf("CALL: main() -> mproc_create(1,(void*)teste3,0)\n");
    int p3 = mproc_create(1,(void*)teste3,0);
    printf("RET: main() -> mproc_create(1,(void*)teste3,0) = %d\n", p3);

    printf("CALL: main() -> mproc_create(1,(void*)teste7,0)\n");
    int p7 = mproc_create(1,(void*)teste7,0);
    printf("RET: main() -> mproc_create(1,(void*)teste7,0) = %d\n", p7);

    printf("CALL: main() -> mproc_create(1,(void*)teste8,0)\n");
    int p8 = mproc_create(1,(void*)teste8,0);
    printf("RET: main() -> mproc_create(1,(void*)teste8,0) = %d\n", p8);

    printf("CALL: main() -> mproc_create(1,(void*)teste9,0)\n");
    int p9 = mproc_create(1,(void*)teste9,0);
    printf("RET: main() -> mproc_create(1,(void*)teste9,0) = %d\n", p9);

    printf("CALL: main() -> mproc_create(1,(void*)teste10,0)\n");
    int p10 = mproc_create(1,(void*)teste10,0);
    printf("RET: main() -> mproc_create(1,(void*)teste10,0) = %d\n", p10);
    /* Fim Cria��o dos Processos */


    /**************************
     * Chamada do Escalonador *
     **************************/      
    printf("CALL: main() -> scheduler()\n");
    scheduler();
    /* Fim Chamada do Escalonador */
    
    printf("FIM: main()\n");
    return init;
}
