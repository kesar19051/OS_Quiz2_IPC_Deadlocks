// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT	 8080 
#define MAXLINE 1024 

// Driver code 
int main() {
    int sockfd; 
	struct sockaddr_in	 servaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 

    const char *delimiter_characters = " ";
    char *filename = "para1.txt";
    FILE *input_file = fopen( filename, "r" );
    char buffer[ MAXLINE];
    //char *last_token;
    if( input_file == NULL ){

        fprintf( stderr, "Unable to open file %s\n", filename );

    }
    char * word = NULL;
    while (fgets(buffer, sizeof(buffer), input_file) != NULL){
        word = strtok(buffer,delimiter_characters);
        while(word!=NULL) {
            sendto(sockfd, (const char *)word, strlen(word), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
            word = strtok(NULL,delimiter_characters);
        }
        //printf("print %s",word);
    }
    fclose(input_file);
	
	
	// int n, len; 
	
	// sendto(sockfd, (const char *)hello, strlen(hello), 
	// 	MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
	// 		sizeof(servaddr)); 
	// printf("Hello message sent.\n"); 
		
	// n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
	// 			MSG_WAITALL, (struct sockaddr *) &servaddr, 
	// 			&len); 
	// buffer[n] = '\0'; 
	// printf("Server : %s\n", buffer); 

	close(sockfd); 
	return 0; 
} 
