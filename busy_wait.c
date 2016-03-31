#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int turn=0;

void regiao_critica(int pid) {
	int s;
	s = rand()%10;
	printf("Processo %d na RC... dormindo %d s.\n", pid, s);
	sleep(s);
}

void regiao_NAO_critica(int pid) {
	printf("Processo %d fazendo qualquer fora da RC.\n", pid);
}

void *processo_0(void *args) {
	int pid = *(int *)args;
	while(1) {
		while(turn != 0);
		regiao_critica(pid);
		turn = 1;
		regiao_NAO_critica(pid);
	}
}

void *processo_1(void *args) {
        int pid = *(int *)args;
        while(1) {
                while(turn != 1);
                regiao_critica(pid);
                turn = 0;
                regiao_NAO_critica(pid);
        }
}
int main(int argc, char *argv[]) {
	pthread_t p0_tid, p1_tid;
	int p0=0, p1=1;
	pthread_create(&p0_tid, NULL, &processo_0, &p0);
	pthread_create(&p1_tid, NULL, &processo_1, &p1);
	pthread_join(p0_tid, NULL);
	pthread_join(p1_tid, NULL);
	return 0;
}
