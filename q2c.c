// C Program for Message Queue (Reader Process) 
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <errno.h>

// structure for message queue 
struct mesg_buffer { 
	long mesg_type; 
	char mesg_text[100]; 
} message; 

int main() 
{ 
	key_t key; 
	int msgid; 

	// ftok to generate unique key 
	key = ftok("q2a.c", 65);
	//printf("The key for the queue is %d.\n", key); 

	// msgget creates a message queue 
	// and returns identifier 
	msgid = msgget(key, 0666 | IPC_CREAT); 

	// msgrcv to receive message 
	while(1){
		msgrcv(msgid, &message, sizeof(message), 1, 0);
		printf("%s ",message.mesg_text); 
	}
	

	// display the message 
	

	// to destroy the message queue 
	msgctl(msgid, IPC_RMID, NULL); 

	return 0; 
} 

