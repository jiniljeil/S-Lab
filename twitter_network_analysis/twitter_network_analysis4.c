#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
typedef struct user {  
	int id; 
	int follower_size; 
	int follower[250];
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

	/*for(int i = 0 ; i < size ;i++) { 
		printf("ID[%d] ", users[i]->id);
		for(int j = 0 ; j < users[i]->follower_size; j++){ 
			printf("%d ", users[i]->follower[j]);  
		}
		printf("\n");
	}*/
	int * friends = (int*)malloc(sizeof(int) * size); 
	memset(friends, 0, sizeof(friends)); 
	int partition = 1 ; 
	
	
	for(int i = 0 ; i < size; i++) { 
		if( friends[i] == 0 ) { 
			int front = 0, tail = 0 ;
			int * queue = (int*)malloc(sizeof(int) * 5000); 
			int q_size = 0 ; 
			if ( users[i]->follower_size == 0 ) { // 그 자체로의 Partition  
				friends[i] = partition; 
				partition++;
				continue;
			}
			queue[tail++] = i; q_size++;  
                        friends[i] = partition; 
                        while(q_size > 0) { 
                        	int curr_v = queue[front] ; 
                                front++ ;
                                q_size--; 

                                for(int k = 0 ; k < users[curr_v]->follower_size; k++) { 
                                	int idx = find_user_idx( users[curr_v]->follower[k], size); 
                                	int isfriend = 0 ; 
                                	for(int p = 0 ; p < users[idx]->follower_size; p++){ 
                                        	if( users[idx]->follower[p] == users[curr_v]->id ) { // 서로 연결된게 존재하는 경우.
                                                	isfriend = 1; 
                                                        break;
                                                }
                                        }
                                	if( isfriend && !friends[idx] ){ 
                                		queue[tail++] = idx;
                                                friends[idx] = partition;
                                                q_size++;
					}
                              	}
                	}
			partition++;
			free(queue) ; 	
		}	
	}

	partition--; // 마지막에 더한거 빼기 
	for(int i = 1 ; i <= partition ; i++) {
		printf("Partition[%d]: ", i); 
		for(int j = 0 ; j < size; j++) { 
			if( friends[j] == i) { 
				//printf("%d ", j);
				printf("%d ", users[j]->id); 	
			}	
		}
		printf("\n"); 
	}
	printf("Partition: %d\n", partition); 	

	free(friends);  
	for(int i = 0 ; i < size ;i++) { 
		free(users[i]);
	}	
	fclose(fp); 
	return 0; 
}
