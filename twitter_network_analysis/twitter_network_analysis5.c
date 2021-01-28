#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <math.h>

typedef struct user {  
	int id; 
	int follower_size; 
	int follower[250];
} USER; 

USER * users [5128];
double matrix [5128][5128];
double calculation[5128]; 
double result [5128]; 

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
	int idx = 0; 	
	
	while( !feof(fp)){ 
		int userid, follower_id; 
		fscanf(fp, "%d %d", &userid, &follower_id);
		
		int check = 0; 
		for(int i = 0 ; i < size; i++) { 
			if( users[i]->id == userid ) {
				users[i]->follower[users[i]->follower_size] = follower_id; 
				users[i]->follower_size++;
				check = 1 ;
				break; 
			}
		}
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
	
	for(int i = 0 ; i < size; i++) { 
	 	if( users[i]->follower_size == 0 ) { 
			for(int j = 0 ; j < size ; j++) { 
				matrix[i][j] = (0.9)/size;
			}
		}
		for(int j = 0 ;j < users[i]->follower_size; j++) {
			idx = find_user_idx(users[i]->follower[j], size);
			matrix[i][idx] += ((0.9 / users[i]->follower_size));	
		}
	}
	
	for(int i = 0 ; i < size; i++) { 
		for(int j = 0 ; j < size; j++) { 
			matrix[i][j] += (0.1)/size; 
		}
	}

	double temp = 0; 
	calculation[0] = 1.0; 

	for(int k = 0 ; k < 20; k++) { 
		for(int i = 0 ; i < size; i++) { 
			temp = 0 ; 
			for(int j = 0; j < size; j++) { 
				temp += (matrix[j][i] * calculation[j]); 	 
			}	
			result[i] = temp;
		}
		for(int i = 0 ; i < size; i++) { 
			calculation[i] = result[i]; 
		}
	}
	double max = 0.00000 ; 
	double before_max = 0.00000;
	double user_max[5] = {0,};
	
	int user[5] = {0, };
	
	for(int i = 0 ; i < 5; i++) { 
		max = 0 ; 
		for(int j = 0 ; j < size; j++) { 
			if( i == 0 ) { 
				if( max < result[j]){  
					max = result[j]; 
					user[i] = j; 
				}
			}else{	
				if( max < result[j] && before_max > result[j]){ 
					max = result[j];
					user[i] = j ; 
				}
			}
		}
		user_max[i] = max; 
		before_max = max ; 
	}
	
	for(int i = 0 ; i < 5; i++) { 
		printf("USER[%d]: %f\n", users[user[i]]->id, user_max[i]);
	}
	
	//printf("%lf\n", max);
	//printf("%d\n", users[user]->id); 
	
	for(int i = 0 ; i < size ;i++) { 
		free(users[i]);
	}	
	fclose(fp); 
	return 0; 
}
