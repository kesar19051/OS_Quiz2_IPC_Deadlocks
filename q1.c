#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

//Type the input and press enter to see the result

int main() 
{ 
	int fd1[2]; // Used to store two ends of first pipe 
	int fd2[2]; // Used to store two ends of second pipe 

	char input_str[500]; 
	pid_t p; 

	if (pipe(fd1)==-1) 
	{ 
		//fprintf(stderr, "Pipe failed" ); 
		//Pipe fails but not printing as the instruction says we should not print anything except the expected output.
		return 0; 
	} 
	if (pipe(fd2)==-1) 
	{ 
		//fprintf(stderr, "Pipe failed" ); 
		return 0; 
	} 

	scanf("%[^\n]%*c", input_str); 
	p = fork(); 

	if (p < 0) { 
		//fprintf(stderr, "Fork failed" ); 
		return 0; 
	} 

	// Parent process 
	else if (p > 0) 
	{ 
		char upperCase_str[500]; 

		close(fd1[0]); 
		write(fd1[1], input_str, strlen(input_str)+1); 
		close(fd1[1]); 
		wait(NULL); 
		close(fd2[1]); 
		read(fd2[0], upperCase_str, 500); 
		printf("%s\n", upperCase_str); 
		//if(flag==1){printf("\n");}
		close(fd2[0]); 
	} 

	// child process 
	else
	{ 
		close(fd1[1]); 
		char upperCase_str[500]; 
		read(fd1[0], upperCase_str, 500); 
		int k = strlen(upperCase_str); 
		int i; 
		for (i=0; upperCase_str[i]!='\0'; i++){
			if(upperCase_str[i] >= 'a' && upperCase_str[i]<='z'){
         			upperCase_str[i] = upperCase_str[i] -32;
         			
      			}
		}
		
		close(fd1[0]); 
		close(fd2[0]); 

		write(fd2[1], upperCase_str, strlen(upperCase_str)+1); 
		close(fd2[1]); 

		exit(0); 
	} 
} 





//Referred this link for understanding the concept https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/
