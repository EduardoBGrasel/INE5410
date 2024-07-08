// Eduardo Bischoff Grasel
// Perguntas
/*
1 - Qual a relação entre n_threads e a diferença entre "Contador" e "Esperado"?
R: Quanto mais threads mais partes críticas do programa estarão a ser executadas ao mesmo tempo, gerando assim uma maior discrepância no contador em relação ao esperado

2- Qual a relação entre n_loops e a diferença entre "Contador" e "Esperado"?
R: Quanto mais loops, maior a chance de alguma thread executar uma região crítica ao mesmo tempo de outra

3 - Existe alguma regra ou padrão na diferença entre "Contador" e "Esperado"?
R: Não existe, o código vai ser executado da maneira que o sistema operancional escolher, ou seja, são muitas variáveis para levar em consideração, o que torna aos olhos humanos como algo "aleatório"
*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

//                 (main)      
//                    |
//    +----------+----+------------+
//    |          |                 |   
// worker_1   worker_2   ....   worker_n


// ~~~ argumentos (argc, argv) ~~~
// ./program n_threads

// ~~~ printfs  ~~~
// main thread (após término das threads filhas): "Contador: %d\n"
// main thread (após término das threads filhas): "Esperado: %d\n"

// Obs:
// - pai deve criar n_threds (argv[1]) worker threads 
// - cada thread deve incrementar contador_global (operador ++) n_loops vezes
// - pai deve esperar pelas worker threads  antes de imprimir!

int contador_global = 0;

void *routine(void *n_loops) {
    int loops = *(int*) n_loops; 
    for (int i = 0; i < loops; i++) {
        contador_global++;
    } 
}

int main(int argc, char* argv[]) {

    if (argc < 3) {
        printf("n_threads é obrigatório!\n");
        printf("Uso: %s n_threads n_loops\n", argv[0]);
        return 1;
    }

    int n_threads = atoi(argv[1]);
    int n_loops = atoi(argv[2]);

    pthread_t thread[n_threads];
    for (int i = 0; i < n_threads; i++) {
        if (pthread_create(&thread[i], NULL, routine, (int*) &n_loops) != 0) {
            perror("Não foi possível criar a thread");
            return 1;
        }
    }
    for (int i = 0; i < n_threads; i++) {
        if (pthread_join(thread[i], NULL) != 0) {
            return 1;
        }
    }
    
    
    printf("Contador: %d\n", contador_global);
    printf("Esperado: %d\n", n_threads*n_loops);
    return 0;
}
