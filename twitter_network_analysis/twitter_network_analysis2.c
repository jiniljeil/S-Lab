#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#define INF 1e8
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
	
			
	int max_path = 0 ; 
	int Long_X = 0 , Long_Y = 0 ;

	for(int i = 0 ; i < size; i++) { 
		int *distance = (int*)malloc(sizeof(int) * size);  
		int *queue = (int*)malloc(sizeof(int) * 2500); 
		int *queue_dist = (int*)malloc(sizeof(int) * 2500); 
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
			if( distance[i] != INF && distance[i] > m) { 
				m = distance[i]; 
				y = i ; 
			}
		}
		if( m > max_path ){
			max_path = m; 
			Long_X = x; 
			Long_Y = y; 	
		}
		printf("Dijsktra Loading: [%d/%d]\n", i, size);
		free(queue); free(queue_dist);
		free(distance);
	}

	printf("USER_X:[%d], USER_Y:[%d]\n", users[Long_X]->id, users[Long_Y]->id); 
	printf("MAX PATH: %d\n", max_path);
	for(int i = 0 ; i < size ;i++) { 
		free(users[i]);
	}	
	//free(arr) ; 
	fclose(fp); 
	return 0; 
}
