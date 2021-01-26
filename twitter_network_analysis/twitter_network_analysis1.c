#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
typedef struct user {  
	int id; 
	int follower_size; 
	int follower[250];
} USER; 

USER * users [5128]; 

int main(void){ 

	FILE * fp = fopen("sample.txt", "r"); 
	int k = 0 ; 
	int size = 0 ; 
	while( !feof(fp)){ 
		int userid, follower_id; 
		fscanf(fp, "%d %d", &userid, &follower_id);
		
		int check = 0; 
		for(int i = 0 ; i < size; i++) { 
			if( users[i]->id == userid ) {
				int exist = 0 ;
			       	for(int j = 0 ; j < users[i]->follower_size; j++) { 
					if( users[i]->follower[j] == follower_id ) { // 이미 존재하는 경우  
						exist = 1 ; 
						break; 	
					}
				}	
				if( exist == 0 ) { 
					users[i]->follower[users[i]->follower_size] = follower_id; 
					users[i]->follower_size++;
				}
				check = 1 ;
				break; 
			}
		}
		// printf("SIZE: %d\n", size); 		
		// 새로운 유저 생성 
		if( check == 0) { 
			users[size] = (USER*)malloc(sizeof(USER)); 
		        users[size]->id = userid ;
			users[size]->follower[users[size]->follower_size] = follower_id; 
			users[size]->follower_size++; 
			size++; 
		}
		
		check = 0; 
		for(int i = 0 ; i < size; i++) { 
			if( users[i]->id == follower_id) { 
				check = 1; 
				break; 	
			}
		}
		if( check == 0 ) { 
			users[size] = (USER*)malloc(sizeof(USER));
                        users[size]->id = follower_id ;
                        size++;
		}
	}	

	
	
	// followd users [1]  
	int * arr = (int*)malloc(sizeof(int) * size); 
	memset(arr, 0 ,sizeof(arr)) ;
	for(int i = 0 ; i < size; i++) { 
		for(int j = 0 ; j < size; j++) { 
			if( i == j) continue; 
			for(int k = 0 ; k < users[j]->follower_size; k++) { 
				if( users[i]->id == users[j]->follower[k] ) { 
					arr[i]++; 
				}	
			}	
		}	
	}

	//printf("%d\n",size);
	/*for(int i = 0 ; i < size; i++) {
	       printf("USER[%d]", users[i]->id);	
		for(int j = 0 ; j < users[i]->follower_size; j++) { 
			printf("%d ", users[i]->follower[j]) ; 
		}
		printf("\n");
	}*/
	for(int i = 0 ; i < size; i++) { 
		printf("USER[%d]: ",users[i]->id); 
		printf("%d | %d\n", users[i]->follower_size, arr[i]); 
	}
	for(int i = 0 ; i < size ;i++) { 
		free(users[i]);
	}	
	free(arr) ; 
	fclose(fp); 
	return 0; 
}
