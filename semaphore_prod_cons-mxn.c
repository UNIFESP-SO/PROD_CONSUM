/*
Autores:
Bruno Ferreira 194790
Fernando Soares
*/
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NPROD 2
#define NCONS 2
#define N 10

int itens[N];
sem_t vazio; // <- N
sem_t cheio; // <- 0
int in = 0,  //posicao p/ inserir no buffer itens
    out= 0;  //posicao p/ remover do buffer itens
int sem[1];
sem_t mut, mut1;

int produz(void) {
	return rand()%100;
}

void insere_item(int item){
	itens[in] = item;
}

int remove_item(void){
	return itens[out];
}

void *produtor(void *args) {
	int item;
	while (1) {
		sleep(1);
		sem_wait(&vazio);
		sem_wait(&mut);
		item = produz();
		insere_item(item);
		printf("P[%d]: itens[%d] <<- %d\n", (int)pthread_self(), in, item);
		in = (in + 1) % N;
		sem_post(&mut);
		sem_post(&cheio);
	}
}

void *consumidor(void *args) {
	int item;
	while (1) {
		sem_wait(&cheio);
		sem_wait(&mut1);
		item = remove_item();
		printf("C[%d]: itens[%d] ->> %d\n",(int)pthread_self(), out, item);
		out = (out + 1) % N;
		sem_post(&mut1);
		sem_post(&vazio);
	}
}

int main(int argc, char *argv[]) {
	int i;
	sem_init(&vazio, 0, N);
	sem_init(&cheio, 0, 0);
	sem_init(&mut, 0, 1);
	sem_init(&mut1, 0, 1);
	
	pthread_t p_tid[NPROD], c_tid[NCONS];

	for(i = 0; i < NPROD; i++){
		pthread_create(&p_tid[i], NULL, produtor, NULL);
	}
	for(i = 0;i < NCONS; i++){
		pthread_create(&c_tid[i], NULL, consumidor, NULL);

	}	
	for(i = 0; i < NPROD; i++){
		pthread_join(p_tid[i], NULL);

	}
	for(i = 0; i < NCONS; i++){
		pthread_join(c_tid[i], NULL);

	}
	return 0;
}
