#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

//The program ends after each philosopher has eaten once.
//The numbering of philosopher is done as 0, P1, P2, P3 and P4.
//To avoid deadlock, the order in which P4 receives the fork is reversed, it first receives F0 and then F4, that is right fork first and then left fork but all the other philosophers take the left fork first and then right fork.

sem_t room;
sem_t chopstick[5];

void * philosopher(void *);
void eat(int);
int main()
{
	int i,a[5];
	pthread_t tid[5];
	
	sem_init(&room,0,5);
	
	for(i=0;i<5;i++)
		sem_init(&chopstick[i],0,1);
		
	for(i=0;i<5;i++){
		a[i]=i;
		pthread_create(&tid[i],NULL,philosopher,(void *)&a[i]);
	}
	for(i=0;i<5;i++)
		pthread_join(tid[i],NULL);
}

void * philosopher(void * num)
{
	int phil=*(int *)num;

	sem_wait(&room);
	printf("P%d is thinking\n",phil);
	if(phil==4){
		sem_wait(&chopstick[(phil+1)%5]);
		printf("P%d receives F%d\n",phil,(phil+1)%5);
		sem_wait(&chopstick[phil]);
		printf("P%d receives F%d\n",phil,phil);
	}
	else{
		sem_wait(&chopstick[phil]);
		printf("P%d receives F%d\n",phil,phil);
		sem_wait(&chopstick[(phil+1)%5]);
		printf("P%d receives F%d\n",phil,(phil+1)%5);
	}
	eat(phil);
	sleep(2);
	printf("P%d has finished eating\n",phil);

	sem_post(&chopstick[phil]);
	sem_post(&chopstick[(phil+1)%5]);
	sem_post(&room);
}

void eat(int phil)
{
	printf("P%d is eating\n",phil);
}
