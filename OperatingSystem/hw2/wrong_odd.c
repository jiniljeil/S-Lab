#include <stdio.h>

int main(void) { 
	int num ;
	scanf("%d",&num) ; 
	
	if( num % 2 != 0 ) { 
		printf("THIS NUM IS ODD!\n");
	}else{ 
		printf("THIS NUM IS EVEN!\n"); 
	}
	while(1);		
	return 0;
}
