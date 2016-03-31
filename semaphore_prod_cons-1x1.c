#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10
int itens[N];
sem_t vazio; // <- N
sem_t cheio; // <- 0
int in = 0,  //posicao p/ inserir no buffer itens
    out= 0;  //posicao p/ remover do buffer itens

int produz(void) {
	return rand()%100;
}

void insere_item(int item){
	sleep(1);
	itens[in] = item;
}

int remove_item(void){
	return itens[out];
}

void *produtor(void *args) {
	int item;
	while (1) {
		sem_wait(&vazio);
		item = produz();
		insere_item(item);
		printf("P: itens[%d] <<- %d\n", in, item);
		in = (in + 1) % N;
		sem_post(&cheio);
	}
}

void *consumidor(void *args) {
	int item;
	while (1) {
		sem_wait(&cheio);
		item = remove_item();
		printf("C: itens[%d] ->> %d\n", out, item);
		out = (out + 1) % N;
		sem_post(&vazio);
	}
}

int main(int argc, char *argv[]) {
	sem_init(&vazio, 0, N);
	sem_init(&cheio, 0, 0);
	pthread_t p_tid, c_tid;
	pthread_create(&p_tid, NULL, produtor, NULL);
	pthread_create(&c_tid, NULL, consumidor, NULL);
	pthread_join(p_tid, NULL);
	pthread_join(c_tid, NULL);
	return 0;
}
