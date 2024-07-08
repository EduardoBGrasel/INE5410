// Eduardo Bischoff Grasel
// Respostas
/*
1 - O speedup obtido está proximo do speedup ideal?
R: Como o programa é extremamente simples, a parte linear dele vai afetar mais o programa do que a parte concorrente em si, ao utilizar apenas 1 núcleo e um vetor de 2000000 posições temos um tempo de execução de 4seg e 484 milésimos, já com 2 núcleos é de 4 seg e 407 milésimos, ou seja, o speedup não foi nem perto do ideal, que seria 2 min e 242 seg.

2 - O programa escala, ou seja, o speedup aumenta se aumentarmos o número de threads?
R: Como é um programa simples e ele está limitado a parte linear, o programa não terá um aumento significativo com as threads, tanto que após um serto número, o programa irá parar de receber melhorias significativas e ficará limitado ao tempo linear.
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);


// Avalia o resultado no vetor c. Assume-se que todos os ponteiros (a, b, e c)
// tenham tamanho size.
void avaliar(double* a, double* b, double* c, int size);

typedef struct info {
    double *a;
    double *b;
    double *c;
    int init;
    int end;
} info;

void *routine(void *args) {
    info arg = *((info*) args); 

    for (int i = arg.init; i < arg.end; i++) {
        arg.c[i] = arg.a[i] * arg.b[i];
    }

    pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
    // Gera um resultado diferente a cada execução do programa
    // Se **para fins de teste** quiser gerar sempre o mesmo valor
    // descomente o srand(0)
    srand(time(NULL)); //valores diferentes
    //srand(0);        //sempre mesmo valor

    //Temos argumentos suficientes?
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n",
               argv[0]);
        return 1;
    }
  
    //Quantas threads?
    int n_threads = atoi(argv[1]);
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    }
    //Lê números de arquivos para vetores alocados com malloc
    int a_size = 0, b_size = 0;
    double* a = load_vector(argv[2], &a_size);
    if (!a) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    double* b = load_vector(argv[3], &b_size);
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    
    //Garante que entradas são compatíveis
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }
    //Cria vetor do resultado 
    double* c = malloc(a_size*sizeof(double));

    // Calcula com uma thread só. Programador original só deixou a leitura 
    // do argumento e fugiu pro caribe. É essa computação que você precisa 
    // paralelizar
    if (n_threads > a_size) {
        n_threads = a_size;
        printf("mais threads do que itens na lista, os threads foram atualizados para o mesmo número de itens na lista.\n");
    }

    pthread_t threads[n_threads];
    int thread_sum = a_size / n_threads;
    info infos[n_threads];

    for (int i = 0; i < n_threads; i++) {
        int start = i * thread_sum;
        int end = start + thread_sum;
        if (i == n_threads - 1) {
            end = a_size;
        }

        infos[i].a = a;
        infos[i].b = b;
        infos[i].c = c;
        infos[i].init = start;
        infos[i].end = end;

        pthread_create(&threads[i], NULL, routine, (void*) &infos[i]);
    }
    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }

        

    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, c, a_size);
    

    //Importante: libera memória
    free(a);
    free(b);
    free(c);

    return 0;
}
