#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5
#define LEFT (id + N - 1)%N
#define RIGHT (id  + 1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int phi_state[N] = {0};
int phi_id[N] = { 0, 1, 2, 3, 4 };
pthread_t phi_ent[N];

sem_t mutex;
sem_t S[N];

void* philosopher(void* num);
void think(int);
void take_chop(int);
void eat(int);
void put_chop(int);
void print_state();

int main(void)
{
	int i;
	
	sem_init(&mutex, 0, 1);
	
	for(i = 0; i < N; i++){
		
		sem_init(&S[i], 0, 0);
	}
	
	for(i = 0; i < N; i++){
		
		pthread_create(&phi_ent[i], NULL, philosopher, &phi_id[i]);
	}
	
	for(i = 0; i < N; i++){
		
		pthread_join(phi_ent[i], NULL);
	}
	
	return 0;
}


void* philosopher(void* num)
{
	int* id = (int*)num;
	
	while(1){
		
		think(*id);
		
		take_chop(*id);
		
		eat(*id);
		
		put_chop(*id);
	}
}

void think(const int id)
{
	//printf("Philosopher %d: thinking\n", id); fflush(stdin);
	print_state();
	sleep(2);
}

void see(const int id)
{
	if(phi_state[id] == HUNGRY && phi_state[LEFT] != EATING && phi_state[RIGHT] != EATING){
		
		phi_state[id] = EATING;
		
		sem_post(&S[id]);
		
	}
}

void take_chop(int id)
{
	
	sem_wait(&mutex);
	
	phi_state[id] = HUNGRY;
	//printf("Philosopher %d: hungry\n", id); fflush(stdin);
	print_state();
	
	see(id);
	
	sem_post(&mutex);
	
	
	sem_wait(&S[id]);
	
}

void eat(const int id)
{
	//printf("Philosopher %d: eating\n", id); fflush(stdin);
	print_state();
	sleep(1);
}

void put_chop(int id)
{
	sem_wait(&mutex);
	
	phi_state[id] = THINKING;
	
	see(LEFT);
	see(RIGHT);
	
	sem_post(&mutex);
}

void print_state()
{
	int i;
	for(i = 0; i < N; i++) printf("  (%d)     ", i);
	printf("\n");
	
	for(i = 0; i < N; i++){
		
		if(phi_state[i] == THINKING) printf("THINKING  ");
		else if(phi_state[i] == HUNGRY) printf(" HUNGRY   ");
		else if(phi_state[i] == EATING) printf(" EATING   ");
	}
	printf("\n\n");
	fflush(stdin);
}




















