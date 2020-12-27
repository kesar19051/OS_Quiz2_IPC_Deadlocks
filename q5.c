#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h>

//there are 4 readers and 2 writers, it would not work for any other number as then the shared memory would not be destroyed, because the condition to destroy the shared memory lies in predefining the number of reader and writer, it is handled by the variables readerEntered and writerEntered

//once every operation is done, the program terminates

//every write operation increments the value in the shared memory by 10

sem_t wrt;
pthread_mutex_t mutex;
int writerEntered = 0;
int readerEntered = 0;
int numreader = 0;

void *writer(void *wno)
{   
    sem_wait(&wrt);
    
	key_t key = ftok("q5.c",65); 
	int shmid = shmget(key,8,0666|IPC_CREAT); 
	long int *str = (long int*) shmat(shmid,(void*)0,0); 
	*str = (*str)+10;
    printf("Writer %d modified cnt to %ld\n",(*((int *)wno)),*str);
    sem_post(&wrt);
    writerEntered++;
    if(writerEntered==2 && readerEntered==4){
    	//printf("destroyed\n");
    	shmctl(shmid,IPC_RMID,NULL); 
    }
}
void *reader(void *rno)
{   
    // Reader acquire the lock before modifying numreader
    pthread_mutex_lock(&mutex);
    numreader++;
    if(numreader == 1) {
        sem_wait(&wrt); // If this id the first reader, then it will block the writer
    }
    pthread_mutex_unlock(&mutex);
    // Reading Section
    key_t key = ftok("q5.c",65); 
	int shmid = shmget(key,8,0666|IPC_CREAT); 
	long int *str = (long int*) shmat(shmid,(void*)0,0); 
    printf("Reader %d: read cnt as %ld\n",*((int *)rno),*str);

    // Reader acquire the lock before modifying numreader
    pthread_mutex_lock(&mutex);
    numreader--;
    if(numreader == 0) {
        sem_post(&wrt); // If this is the last reader, it will wake up the writer.
    }
    pthread_mutex_unlock(&mutex);
    readerEntered++;
    if(writerEntered==2 && readerEntered==4){
    	//printf("destroyed\n");
    	shmctl(shmid,IPC_RMID,NULL); 
    }
}

int main()
{   
//printf("size of long int is: %ld\n", sizeof(long int));

    pthread_t read[4],write[2];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1);

    int a[10] = {1,2,3,4,5,6,7,8,9,10}; //Just used for numbering the producer and consumer
    for(int i = 0; i < 4; i++) {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]);
    }
    for(int i = 0; i < 2; i++) {
        pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
    }

    for(int i = 0; i < 4; i++) {
        pthread_join(read[i], NULL);
    }
    for(int i = 0; i < 2; i++) {
        pthread_join(write[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);
    //shmctl(shmid,IPC_RMID,NULL); 

    return 0;
    
}
