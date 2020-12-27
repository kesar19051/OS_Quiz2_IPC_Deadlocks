// C Program for Message Queue (Writer Process) 
#include <stdio.h> 
#include <string.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#define BUFFER_SIZE 1024
// structure for message queue 
struct mesg_buffer { 
	long mesg_type; 
	char mesg_text[100]; 
} message; 

int count = 0;

int main() 
{ 
	key_t key; 
	int msgid; 

	// ftok to generate unique key 
	key = ftok("q2a.c", 65); 
	//printf("The key for the queue is %d.\n", key);

	// msgget creates a message queue 
	msgid = msgget(key, 0666 | IPC_CREAT); 
	//printf("the message id is: %d", msgid);
    	message.mesg_type = 1; 
	// and returns identifier 
	
	
    const char *delimiter_characters = " ";
    char *filename = "para1.txt";
    FILE *input_file = fopen( filename, "r" );
    char buffer[ BUFFER_SIZE ];
    char *last_token;

    if( input_file == NULL ){

        fprintf( stderr, "Unable to open file %s\n", filename );

    }else{

        // Read each line into the buffer
        while( fgets(buffer, BUFFER_SIZE, input_file) != NULL ){

            
            last_token = strtok( buffer, delimiter_characters );
            while( last_token != NULL ){
                //printf( "%s\n", last_token );
                strncpy(message.mesg_text, last_token, sizeof(last_token));
                msgsnd(msgid, &message, sizeof(message), 0); 
                count++;
                last_token = strtok( NULL, delimiter_characters );
            }

        }

        if( ferror(input_file) ){
            perror( "The following error occurred" );
        }

        fclose( input_file );

    }

	// msgsnd to send message 
	

	// display the message 
	//printf("Data send is : %d \n", count); 

	return 0; 
} 

