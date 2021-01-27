#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#define INF 1e8
typedef struct user {  
	int id; 
	int follower_size; 
	int follower[1000];
} USER; 

USER * users [5128]; 

int find_user_idx(int id, int size ) { 
	for(int i = 0 ; i < size; i++) { 
		if( users[i]->id == id){ 
			return i;
		}
	}
	printf("NOTHING\n");
	return -1;
}

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

	// 모두 연결 시킨다. 
	for(int i = 0 ; i < size; i++) { 
	 	for(int j = 0 ; j < users[i]->follower_size; j++) { 
			int idx = find_user_idx(users[i]->follower[j], size); 
			int follower_each = 0 ; 

			for(int k = 0 ; k < users[idx]->follower_size; k++) { 
				if (users[i]->id == users[idx]->follower[k]) { 
					follower_each = 1; 
					break; 
				}
			}
			// 맞팔이 아닌 경우 
			if( !follower_each ) { 		
				users[idx]->follower[users[idx]->follower_size] = users[i]->id;  	
				users[idx]->follower_size++; 
			}
		}
		printf("Connecting: [%d/%d]\n", i, size);	
	}

	for(int i = 0 ; i < size; i++) { 
		printf("ID[%d]", users[i]->id); 
		for(int j = 0 ; j < users[i]->follower_size; j++) { 
			printf("%d ", users[i]->follower[j]); 
		}
		printf("\n");
	}
	int * friends = (int*)malloc(sizeof(int) * size);
	memset(friends, 0, sizeof(friends));
	int partition = 1 ;

	int m = 0 ; 
	for(int i = 0 ; i < size; i++) {
		int c = 0 ; 
		if( friends[i] == 0 ) {
			int front = 0, tail = 0 ;
			int * queue = (int*)malloc(sizeof(int) * 5000);
			int q_size = 0 ;
			friends[i] = partition; 
			queue[tail++] = i; q_size++; 
			c++; 
			while(q_size > 0) { 
				int curr_v = queue[front] ; 
				front++; 
				q_size--; 

				for(int k = 0 ; k < users[curr_v]->follower_size; k++) {
					int idx = find_user_idx( users[curr_v]->follower[k], size); 
					if( !friends[idx] ) { 
						queue[tail++] = idx; 
						q_size++;
						friends[idx] = partition;
						c++; 
					}
				}
			}
			partition++;
			if( m < c) { 
				m = c; 
			}
			free(queue) ;
		}
	}
	partition--; // 마지막에 더한거 빼기
	for(int i = 1 ; i <= partition ; i++) {
		printf("GROUP[%d]: ", i);
		for(int j = 0 ; j < size; j++) {
			if( friends[j] == i) {
				//printf("%d ", j);
				printf("%d ", users[j]->id);
			}
		}
		printf("\n");
	}
	printf("\n");
	free(friends);
	if( partition == 1 ) { 
		printf("Connected: True\n");
	}else{ 
		printf("Connected: False\n");
	}
	printf("The number of vertex of the largest connected graph: %d\n",m );

	for(int i = 0 ; i < size ;i++) { 
		free(users[i]);
	}	
	//free(arr) ; 
	fclose(fp); 
	return 0; 
}
