#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int pipes[2]; 

void parent_proc() {
	char * buf = "gcc -o pipe2 pipe2.c\0"; 
	ssize_t s ;
	size_t len = 0 ; 

	close(pipes[0]); 

	int k = 0; 
	write(pipes[1], buf, strlen(buf) + 1);
	close(pipes[1]);
}

void child_proc() { 
	char buf[32]; 
	ssize_t s;
	
	close(pipes[1]); 

	while((s = read(pipes[0], buf, 31)) > 0) { 
		buf[s+1] = 0x0; 
		printf(">%s\n",buf);
	}
	exit(0);
}
int main() { 
	pid_t child_pid; 
	int exit_code ;

	if( pipe(pipes) != 0 ){ 
		perror("Error!");
		exit(1); 
	}
	printf("%d %d\n", pipes[0], pipes[1]);

	child_pid = fork(); 
	if( child_pid == 0) {
		child_proc();
	}else{ 
		parent_proc();
	}
	wait(&exit_code); 
	return 0 ;
}
