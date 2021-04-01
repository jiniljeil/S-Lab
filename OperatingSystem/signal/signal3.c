#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handler (int sig){ 
	printf("Do you want to quit?") ; 
	if (getchar() == 'y') { 
		exit(0); 
	}		
}

int main(){
	int d;
        // interrupt 발생시 실행. 	
	signal(SIGTERM, handler);
	scanf("%d",&d); 
}
