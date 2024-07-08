#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>


//                          (principal)
//                               |
//              +----------------+--------------+
//              |                               |
//           filho_1                         filho_2
//              |                               |
//    +---------+-----------+          +--------+--------+
//    |         |           |          |        |        |
// neto_1_1  neto_1_2  neto_1_3     neto_2_1 neto_2_2 neto_2_3

// ~~~ printfs  ~~~
//      principal (ao finalizar): "Processo principal %d finalizado\n"
// filhos e netos (ao finalizar): "Processo %d finalizado\n"
//    filhos e netos (ao inciar): "Processo %d, filho de %d\n"

// Obs:
// - netos devem esperar 5 segundos antes de imprmir a mensagem de finalizado (e terminar)
// - pais devem esperar pelos seu descendentes diretos antes de terminar

int main(int argc, char** argv) {

    pid_t pid_1 = fork();
    if (pid_1 == 0) { //  criação do primeiro filho
        printf("Processo %d, filho de %d\n", getpid(), getppid());

        // criando os três netos
        for (int i = 0; i < 3; i++) {
            pid_t pid_2 = fork();

            if (pid_2 == 0) { // criando os netos
                printf("Processo %d, filho de %d\n", getpid(), getppid());
                sleep(5); 
                printf("processo %d finalizado\n", getpid());
                exit(EXIT_SUCCESS);
            } else if (pid_2 < 0) {
                perror("Não foi possivel criar o processo neto\n");
                exit(EXIT_FAILURE);
            }

        }
    // aguarda os netos
    wait(NULL);
    wait(NULL);
    wait(NULL);
    printf("Processo %d finalizado\n", getpid());
    exit(EXIT_SUCCESS);

    } else if (pid_1 < 0) {  // caso não seja possivel criar o filho
        perror("Não foi possivel criar o filho\n");
        exit(EXIT_FAILURE);
    }


    pid_t pid_1_1 = fork();
    if (pid_1_1 == 0) { //  criação do segundo filho
        printf("Processo %d, filho de %d\n", getpid(), getppid());

        // criando os três netos
        for (int i = 0; i < 3; i++) {
            pid_t pid_2_2 = fork();

            if (pid_2_2 == 0) { // criando os netos
                printf("Processo %d, filho de %d\n", getpid(), getppid());
                sleep(5); 
                printf("processo %d finalizado\n", getpid());
                exit(EXIT_SUCCESS);
            }  else if (pid_2_2 < 0) {
                perror("Não foi possivel criar o processo neto\n");
                exit(EXIT_FAILURE);
            }

        }
    // aguarda os netos
    wait(NULL);
    wait(NULL);
    wait(NULL);
    printf("Processo %d finalizado\n", getpid());
    exit(EXIT_SUCCESS);

    } else if (pid_1_1 < 0) {  // caso não seja possivel criar o filho
        perror("Não foi possivel criar o filho\n");
        exit(EXIT_FAILURE);
    }

    //aguarda os processos filhos
    wait(NULL);    
    wait(NULL);

    /*************************************************
     * Dicas:                                        *
     * 1. Leia as intruções antes do main().         *
     * 2. Faça os prints exatamente como solicitado. *
     * 3. Espere o término dos filhos                *
     *************************************************/

    printf("Processo principal %d finalizado\n", getpid());    
    return 0;
}
