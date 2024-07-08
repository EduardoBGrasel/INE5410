#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

//       (pai)      
//         |        
//    +----+----+
//    |         |   
// filho_1   filho_2


// ~~~ printfs  ~~~
// pai (ao criar filho): "Processo pai criou %d\n"
//    pai (ao terminar): "Processo pai finalizado!\n"
//  filhos (ao iniciar): "Processo filho %d criado\n"

// Obs:
// - pai deve esperar pelos filhos antes de terminar!


int main(int argc, char** argv) {

    	
    pid_t pid = fork();
    if (pid == 0) {
        // dentro do processo filho
        printf("Processo filho %d criado\n", getpid());
        exit(EXIT_SUCCESS);
    } else if (pid < 0) {  // caso não consigamos criar o projeto filho
        perror("Falha ao criar o processo\n");
        exit(EXIT_FAILURE);
    } else {
        // processo pai
        printf("Processo pai criou %d\n", pid);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // dentro do processo filho
        printf("Processo filho %d criado\n", getpid());
        exit(EXIT_SUCCESS);
    } else if (pid2 < 0) {  // caso não consigamos criar o projeto filho
        perror("Falha ao criar o processo\n");
        exit(EXIT_FAILURE);
    } else {
        // processo pai
        printf("Processo pai criou %d\n", pid2);
    }
    /*************************************************
     * Dicas:                                        *
     * 1. Leia as intruções antes do main().         *
     * 2. Faça os prints exatamente como solicitado. *
     * 3. Espere o término dos filhos                *
     *************************************************/

    // aguarda a finalização dos dois processos filhos
    wait(NULL);
    wait(NULL);

    printf("Processo pai finalizado!\n");   
    return 0;
}
