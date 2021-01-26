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
				if (users[i]->follower[j] == users[idx]->follower[k]) { 
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
	int connected_check = 1; 
	int max_path = 0 ; 
	int Long_X = 0 , Long_Y = 0 ;

	for(int i = 0 ; i < size; i++) { 
		int *distance = (int*)malloc(sizeof(int) * size);  
		int *queue = (int*)malloc(sizeof(int) * 10000); 
		int *queue_dist = (int*)malloc(sizeof(int) * 10000); 
 		int q_size = 0, front = 0 ,tail = 0 ; 
		
		for(int j = 0 ; j < size; j++) distance[j] = INF; 
		
		distance[i] = 0; 
		queue[tail] = i; queue_dist[tail] = 0 ; tail++; q_size++; 
			
		while(q_size > 0) { 
			int curr_v = queue[front]; 
			int curr_dist = queue_dist[front]; 
			front++; 
			q_size--;  // pop 
		
			if( distance[curr_v] < curr_dist ) continue; 
			for(int j = 0 ; j < users[curr_v]->follower_size; j++) { 
				int next_v = users[curr_v]->follower[j]; 
				int nv = 0; 
				for(int k = 0 ; k < size; k++) { 
					if( next_v == users[k]->id){ 
						nv = k;
						break;	
					}
				}
				int next_w = 1; 
			        if( distance[nv] > curr_dist + next_w) { 
					distance[nv] =curr_dist + next_w; 
					queue[tail] = nv; 
					queue_dist[tail] = distance[nv]; 
					tail++; q_size++;
				}	
			}
		}	

		int m = 0 ; 
		int x = i; 
		int y = 0 ; 
		for(int i = 0; i < size; i++) { 
			if( distance[i] == INF) { 
				connected_check = 0; 
				break; 
			}	
		}
		//printf("Dijkstra Loading: [%d/%d]\n",i, size);  
		free(queue); free(queue_dist);
		free(distance);
		if( !connected_check ) break;
	}
	
	if( connected_check == 1 ) { 
		printf("Connected: True\n");
	}else{ 
		printf("Connected: False\n");
	}
	for(int i = 0 ; i < size ;i++) { 
		free(users[i]);
	}	
	//free(arr) ; 
	fclose(fp); 
	return 0; 
}
