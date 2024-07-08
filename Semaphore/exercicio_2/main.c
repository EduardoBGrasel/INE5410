#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* ---------- Definições Globais. ---------- */
#define TEMPO_BASE 1000000

typedef enum { 
	CONTINENTE, 
	ILHA 
} cabeceira_t;

typedef struct {
	int id;
	cabeceira_t cabeceira;
	pthread_t thread;
} veiculo_t;
/* ---------------------------------------- */

/* ---------- Variáveis Globais. ---------- */
char cabeceiras[2][11] = { { "CONTINENTE" }, { "ILHA" } };
int total_veiculos;
int veiculos_turno;
sem_t sem_ilha, sem_continente;
pthread_mutex_t mutex;
int contador_veiculos;
// ToDo: Adicione aque quaisquer outras variávels globais necessárias.
/* ---------------------------------------- */


/* Inicializa a ponte. */
void ponte_inicializar() {
	
	// ToDo: IMPLEMENTAR!
	/*Inicializando os semaforos*/
	sem_init(&sem_continente, 0, veiculos_turno);
	sem_init(&sem_ilha, 0 , 0);
	pthread_mutex_init(&mutex, NULL);
	/* Imprime direção inicial da travessia. NÃO REMOVER! */
	printf("\n[PONTE] *** Novo sentido da travessia: CONTINENTE -> ILHA. ***\n\n");
	fflush(stdout);
	/* Definindo para veiculos_turno sairem pelo continente*/
}

/* Função executada pelo veículo para ENTRAR em uma cabeceira da ponte. */
void ponte_entrar(veiculo_t *v) {
	// ToDo: IMPLEMENTAR!
	/*Veiculos entram na ponte (retira 1 do semaforo a cada veiculo até ficar em 0)*/
	if (v->cabeceira == 0) {
		sem_wait(&sem_continente);
	}
	else {
		sem_wait(&sem_ilha);
	}
}

/* Função executada pelo veículo para SAIR de uma cabeceira da ponte. */
void ponte_sair(veiculo_t *v) {
	// ToDo: IMPLEMENTAR!
	//  contador para controlar a saida
	pthread_mutex_lock(&mutex);
	contador_veiculos++;
	pthread_mutex_unlock(&mutex);
	if (contador_veiculos == veiculos_turno) {
		contador_veiculos = 0;
		printf("\n[PONTE] *** Novo sentido da travessia: %s -> %s. ***\n\n", cabeceiras[v->cabeceira], cabeceiras[!v->cabeceira]);
		fflush(stdout);

		for (int i = 0; i < veiculos_turno; i++) {
			// adicionando os posts no semaforo
			if (v->cabeceira == 0) {
				sem_post(&sem_continente);
			}
			else {
				sem_post(&sem_ilha);
			}
		}
	}
}

/* FINALIZA a ponte. */
void ponte_finalizar() {

	// ToDo: IMPLEMENTAR!
	/*Destruindo os semaforos*/
	sem_destroy(&sem_continente);
	sem_destroy(&sem_ilha);
	pthread_mutex_destroy(&mutex);
	/* Imprime fim da execução! */
	printf("[PONTE] FIM!\n\n");
	fflush(stdout);
}

/* Implementa o comportamento de um veículo. */
void * veiculo_executa(void *arg) {
	veiculo_t *v = (veiculo_t *) arg;
	
	printf("[Veiculo %3d] Aguardando para entrar na ponte pelo(a) %s.\n", v->id, cabeceiras[v->cabeceira]);
	fflush(stdout);

	/* Entra na ponte. */
	ponte_entrar(v);
	printf("[Veiculo %3d] ENTROU na ponte pelo(a) %s.\n", v->id, cabeceiras[v->cabeceira]);
	fflush(stdout);

	/* Faz a travessia. */
	usleep(TEMPO_BASE + rand() % 1000);
	
	/* Seta cabeceira oposta para sair. */
	v->cabeceira = !v->cabeceira;

	/* Sai da ponte na cabeceira oposta. */
	printf("[Veiculo %3d] SAIU da ponte pelo(a) %s.\n", v->id, cabeceiras[v->cabeceira]);
	fflush(stdout);
	ponte_sair(v);

	pthread_exit(NULL);
}

/* Função principal: NÃO PODE ALTERAR! */
int main(int argc, char **argv) {
 
	if (argc < 3) {
		printf("Indique a quantidade total de veiculos e o numero de veiculos que podem atravessar a ponte por turno:\n\n %s [QTD_TOTAL_VEICULOS] [VEICULOS_POR_TURNO]\n\n", argv[0]);
		return 1;
	}

	total_veiculos = atoi(argv[1]);
	veiculos_turno = atoi(argv[2]);

	if (total_veiculos % (veiculos_turno * 2)) {
		printf("ERRO: O valor [QTD_TOTAL_VEICULOS] deve ser divisivel por ([VEICULOS_POR_TURNO] * 2)!\n\n %s [QTD_TOTAL_VEICULOS] [VEICULOS_POR_TURNO]\n\n", argv[0]);
		return 1;
	}

	srand(time(NULL));

	/* Aloca os veículos. */
	veiculo_t veiculos[total_veiculos];

	ponte_inicializar();

	/* Cria os veículos. */
	for (int i = 0; i < total_veiculos; i++) {
		/* Define o id do veículo. */
		veiculos[i].id = i;
		
		/* Escolhe aleatoreamente se o veículo entra pela ILHA ou CONTINENTE. */
		veiculos[i].cabeceira = i % 2; 
		
		/* Cria a thread veículo. */
		pthread_create(&veiculos[i].thread, NULL, veiculo_executa, (void *) &veiculos[i]);		
	}

	/* Aguarda o término da execução de todos os veículos antes de finalizar o programa. */
	for (int i = 0; i < total_veiculos; i++) {
		pthread_join(veiculos[i].thread, NULL);
	}

	ponte_finalizar();

	return 0;
}
