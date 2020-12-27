#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <semaphore.h>
#include <pthread.h>

sem_t wrt;
pthread_mutex_t mutex;
int numreader = 0;

int main(){
	pid_t pid;
	pthread_mutex_init(&mutex, NULL);
    	sem_init(&wrt,0,1);
	pid = fork();
	//parent is the reader
	if(pid>0){
		//wait(NULL);
		key_t key1 = ftok("para1.txt",65); 
		//key1 for text
		int shmid1 = shmget(key1,1024,0666|IPC_CREAT); 
		char *ptr = (char*) shmat(shmid1,(void*)0,0); 
		printf("Reader read as %s\n",ptr);
		key_t key2 = ftok("para2.txt",65); 
		//key2 for numbers
		int shmid2 = shmget(key2,1024,0666|IPC_CREAT); 
		long int *str = (long int*) shmat(shmid2,(void*)0,0); 
		for(int i = 0; i<10; i++){
		pthread_mutex_lock(&mutex);
    		numreader++;
    		if(numreader == 1) {
        		sem_wait(&wrt); 
    		}
    		pthread_mutex_unlock(&mutex);
    		//read
		printf("Reader read as %ld\n",*str);
		pthread_mutex_lock(&mutex);
    		numreader--;
    		if(numreader == 0) {
        		sem_post(&wrt);
    		}
    		pthread_mutex_unlock(&mutex);}
    		shmctl(shmid1,IPC_RMID,NULL);
		shmctl(shmid2,IPC_RMID,NULL);
	}
	//child is the writer
	else if(pid==0){
		//printf("enter child process\n");
		key_t key1 = ftok("para1.txt",65); 
		//key1 for text
		int shmid1 = shmget(key1,1024,0666|IPC_CREAT); 
		char *ptr = (char*) shmat(shmid1,(void*)0,0); 
      		key_t key2 = ftok("para2.txt",65); 
		//key2 for numbers
		int shmid2 = shmget(key2,1024,0666|IPC_CREAT); 
		long int *str = (long int*) shmat(shmid2,(void*)0,0); 
		for(int i = 0; i<10; i++){
		sem_wait(&wrt);
		//write
		
      		strcat(ptr, "Z");
		printf("Writer modified text to %s\n",ptr);
		
		*str = (*str)+10;
		printf("Writer modified number to %ld\n",*str);
		sem_post(&wrt);}
		shmctl(shmid1,IPC_RMID,NULL);
		shmctl(shmid2,IPC_RMID,NULL);
	}
	else{perror("Fork failed\n");}
	pthread_mutex_destroy(&mutex);
    	sem_destroy(&wrt);
    	//shmctl(shmid1,IPC_RMID,NULL);
	return 0;
}
