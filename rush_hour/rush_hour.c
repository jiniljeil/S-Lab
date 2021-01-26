#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0 
/* NOFIX --- */

typedef enum {
	start,
	left,
	right,
	up,
	down,
	quit,
	N_op 
} commands ;

typedef enum {
	vertical,
	horizontal 
} direction ;

typedef struct car_t{
	struct car_t * prev; 
	int id ;
	int y1, y2 ;	// y1: the minimum of y, y2: the maximum of y
	int x1, x2 ;	// x1: the minimum of x, x2: the maximum of x
	int span ;		// the number of cells 
	direction dir ;	// the direction of the car
} car_t ;

char * op_str[N_op] = {
	"start",
	"left",
	"right",
	"up",
	"down",
	"quit",
} ;

typedef struct state {
	struct state * prev; 
	int n_cars ;
	car_t * cars ;
	int cells[6][6] ;
} state ;

int n_cars = 0 ;
car_t * cars = 0x0 ;
int cells[6][6] ; // cells[Y][X]
char str[1000][6];
/* --- NOFIX */
int move (int id, int op, state * s) ;
void display() ; 

int update_states(state * s);
int loop = 0;
int dx[4] = {-1, 0, 1, 0}; 
int dy[4] = { 0, 1, 0, -1} ;
int focusOn_car = 1; 
int visited[6][6]; 
int existance[31]; // 인접한 차 

typedef struct Node{ 
	state * s ; 
	struct Node *next ;
} NODE; 


NODE * head = NULL; 
NODE * tail = NULL; 
NODE * visit = NULL; 
car_t * copy_cars_states;

state * alloc_state (car_t * cars_, state *cars_state) {
	
	state * c = (state*)malloc(sizeof(state));
	c->n_cars = loop; 
	// printf("%d",c->n_cars);
	c->cars = (car_t*)malloc(sizeof(car_t) * c->n_cars);
	for(int i = 0 ; i < loop; i++){ 
		c->cars[i].dir = cars_[i].dir;
		c->cars[i].id = cars_[i].id; 
		c->cars[i].span = cars_[i].span; 
		c->cars[i].x1 = cars_[i].x1; 
		c->cars[i].y1 = cars_[i].y1;
		c->cars[i].x2 = cars_[i].x2; 
		c->cars[i].y2 = cars_[i].y2; 
		// printf("%d", c->cars[i].dir);
	}	

	c->prev = cars_state; 
	return c ;
}

int move_check (car_t c, direction op ) {
	if( c.dir == horizontal) {
		if( op == left ){
			if( c.x1 - 1 >= 0 && cells[c.y1][c.x1 -1] == 0) {
				return 1; 
			}
		}
		if( op == right){
			if( c.x2 + 1 < 6 && cells[c.y1][c.x2 + 1] == 0) {
				return 1; 
			}
		}
	}else if( c.dir == vertical){ // vertical 
		if( op == up ){
			if( c.y1 + 1 < 6 && cells[c.y1 + 1][c.x1] == 0) {
				return 1; 
			}
		}
		if( op == down){
			if( c.y2 - 1 >= 0 && cells[c.y2 - 1][c.x1] == 0) {
				return 1; 
			}
		}
	}
	return 0 ;
}

int equal_state (state * a, state * b) {
	for(int i = 0; i < a->n_cars; i++) {
		if( a->cars[i].dir != b->cars[i].dir || a->cars[i].id != b->cars[i].id || a->cars[i].x1 != b->cars[i].x1 ||
		a->cars[i].span != b->cars[i].span || a->cars[i].x2 != b->cars[i].x2 || a->cars[i].y1 != b->cars[i].y1 || 
		a->cars[i].y2 != b->cars[i].y2){
			return 0;
		}
	} 
	return 1; 
}

int is_visited_state (state * cars_states){
	// memset(state_cells, 0, sizeof(int) * 36) ; // clear cells before the write.
	NODE* i ;
	for( i = head; i != NULL; i = i->next){
		if( equal_state(i->s, cars_states)) {
			return 1; 
		}
	}
	return 0;
}

void queue (state * cars_state) {
	if( head == NULL) {
		head = (NODE*)malloc(sizeof(NODE)); 
		head->s = cars_state; 
		head->next = NULL; 
		tail = head ;
		// update_cells(tail);
		visit = head; 
	}else {
		tail->next = (NODE*)malloc(sizeof(NODE));
		tail = tail->next; 
		tail->s = cars_state; 
		// update_cells(tail); 
		tail->next = NULL; 
	}
}

void queue_next_states (state *cars_state){ 
	state * next_state ; 

	for(int i = 0 ; i < loop; i++) {
		if( cars_state->cars[i].dir == horizontal) {
			if( move_check(cars_state->cars[i], right)) {
				next_state = alloc_state(cars_state->cars,cars_state); 
				for(int j = 0 ; j < 5; j++) {
					move(next_state->cars[i].id, right, next_state);
				}
				
				if(is_visited_state(next_state)) continue ;
				queue(next_state);
			}
			if( move_check(cars_state->cars[i], left)) {
				next_state = alloc_state(cars_state->cars, cars_state); 
				for(int j = 0 ; j < 5; j++) {
					move(next_state->cars[i].id, left, next_state); 
				}
				if(is_visited_state(next_state)) continue ;
				queue(next_state);
			}
		}else{ // vertical 
			if( move_check(cars_state->cars[i], up)) {
				next_state = alloc_state(cars_state->cars,cars_state);
				for(int j = 0 ; j < 5; j++) {
					move(next_state->cars[i].id, up, next_state); 
				}
				if(is_visited_state(next_state)) continue ;
				queue(next_state);
			}
			if( move_check(cars_state->cars[i], down)) {
				next_state = alloc_state(cars_state->cars,cars_state);
				for(int j = 0 ; j < 5; j++) {
					move(next_state->cars[i].id, down, next_state);
				}
				if(is_visited_state(next_state)) continue ;
				queue(next_state);
			}
		}
	}
}

commands
get_op_code (char * s)
{
	for(int i = 0; i < 6; i++) {
		if(!strcmp(op_str[i], s)){
			return i; 
		}
	}
	return 5;
	// return the corresponding number for the command given as s.
	// FIXME
}

int
load_game (char * filename)
{
	//FIXME
	// load_game returns 0 for a success, or return 1 for a failure.
	// Use fopen, getline, strtok, atoi, strcmp
	// Note that the last character of a line obtained by getline may be '\n'.
	FILE *fp;

	fp = fopen(filename, "r"); 
	
	if(fp == NULL){
		printf("Not Found the File\n");
		return 1;
	}else{ 
		char *tmp = NULL;
		size_t len = 30 ;
		fscanf(fp, "%d", &loop);
		
		cars = (car_t*)malloc(sizeof(car_t) * loop) ;
		
		for(int k = 0 ; k <= loop; k++) {
			getline(&tmp, &len, fp);

			if( k >= 1 ) {
				cars[k - 1].id = k ; 
				char *ptr = strtok(tmp, ":");  
				char location ;
				int num ;
				while(ptr != NULL) {
					if((ptr[0] >= 'A' && ptr[0] <= 'F')){
						location = ptr[0]; 
						num = atoi(&ptr[1]);
					}else if(!strcmp(ptr, "horizontal")){
						cars[k - 1].dir = horizontal; 
						
					}else if(!strcmp(ptr, "vertical")) {
						cars[k - 1].dir = vertical ;
						
					}else if(ptr[0] >= '0' && ptr[0] <= '9'){
						cars[k - 1].span = atoi(&ptr[0]); 
						if(cars[k - 1].span > 6) {
							printf("invalid data\n");
							return 1;
						}
					}
					
					ptr = strtok(NULL, ":");
				}
				if( cars[k - 1].dir == horizontal){
					if((location >= 'A' && location <= 'F') && (num >= 1 && num <= 6)) {
						cars[k - 1].x1 = location - 'A'; 
						cars[k - 1].y1 = num - 1; 
						cars[k - 1].x2 = location - 'A' + cars[k - 1].span - 1; 
						cars[k - 1].y2 = num - 1;
					}else{ 
						printf("invalid data\n");
						return 1; 
					}
				}else if(cars[k - 1].dir == vertical){
					if((location >= 'A' && location <= 'F') && (num >= 1 && num <= 6)) {
						cars[k-1].x1 = location - 'A';
						cars[k-1].y1 = num - 1;
						cars[k-1].x2 = location - 'A';
						cars[k-1].y2 = num - cars[k - 1].span;
					}else{ 
						printf("invalid data\n");
						return 1; 
					}
				}
			}
		}
	}
	fclose(fp);
	return 0;
}

void
display ()
{
	printf("\nThe beginning state of board1.txt must be shown as follows:\n");
	for(int i = 5 ; i >= 0; i--) {
		for(int j = 0 ; j < 6; j++ ){
			if( cells[i][j] == 0) {
				printf("+ ");
			}else{
				printf("%d ", cells[i][j]); 
			}
		}
		printf("\n");
	}
	//FIXME
	return ;
}

int 
update_states(state * s)
{
	memset(cells, 0, sizeof(int) * 36) ; // clear cells before the write.

	for(int i = 0 ; i < loop; i++) {
		if( s->cars[i].dir == horizontal) {
			for(int j = s->cars[i].x1 ; j <= s->cars[i].x2; j++ ) {
				cells[s->cars[i].y1][j] = s->cars[i].id; 
			}
		}else if( s->cars[i].dir == vertical) {
			for(int j = s->cars[i].y1; j >= s->cars[i].y2; j--) {
				cells[j][s->cars[i].x1] = s->cars[i].id; 
			}
		}else{
			return 1; 
		}
	}

	return 0;

}

int
move (int id, int op, state * s ) 
{
	if( id < 1) {
		printf("the minimum x value of car_id is greather than 0.\n");
		return 1; 
	}
	int distance = 0; 
	id -= 1;
	if(s->cars[id].dir == horizontal) {
		if(op == 1) { // left 
			if(s->cars[id].x1 - 1 >= 0 && cells[s->cars[id].y1][s->cars[id].x1 - 1] == 0) {
				s->cars[id].x1--;
				s->cars[id].x2--;
			}else{
				return 1;
			}
			
		}else if(op == 2) { // right 	 
			if(s->cars[id].x2 + 1 < 6 && cells[s->cars[id].y1][s->cars[id].x2 + 1] == 0) {
				s->cars[id].x1++; 
				s->cars[id].x2++;
			}else{
				return 1;
			}
		}else{ // Exception up, down
			printf("Invalid input\n");
			return 1; 
		}
	}
	if(s->cars[id].dir == vertical) {
		if(op == 3) { // up	  
			if (s->cars[id].y1 + 1 < 6 && cells[s->cars[id].y1 + 1][s->cars[id].x1] == 0) {
				s->cars[id].y1++; 
				s->cars[id].y2++;
			}else{
				return 1;
			}
		}else if(op == 4) { // down  
			if (s->cars[id].y2 -1 >= 0 && cells[s->cars[id].y2 - 1][s->cars[id].x1] == 0) {
				s->cars[id].y1--; 
				s->cars[id].y2--;
			}else{
				return 1;
			}
		}else{ // Exception left, right
			printf("invalid input\n");
			return 1; 
		}
	}
	
	return 1; 
}
void adj_finder(int k, int n_cars);

int direct_move ;
int curr_v = 1;

int
main ()
{
	char buf[128] ;
	int op ;
	int id ;

	// while (1) {
		printf("*************RUSH HOUR*************\n"); 
		printf("1. start\n2. left\n3. right\n4. up\n5. down\n6. quit\n") ;
		printf("***********************************\n");
		printf("Enter an executing command: ");
		scanf("%s", buf) ;
		
		switch (op = get_op_code(buf)) {
			printf("op: %d\n", op);
			case start:
				printf("Enter a file name (ex: board1.txt): "); 
				scanf("%s", buf) ;
				load_game(buf) ;
				state * init = alloc_state(cars, NULL);
				update_states(init) ;
				queue(init);
				display() ;

				while(visit != NULL) {
					state * curr_state = visit->s;
					if(curr_state == NULL) {
						printf("NULL\n"); 
						break;
					}
					update_states(curr_state) ;
					display();
					if( cells[3][5] == 1 ) {
						printf("SUCCESS\n");
						exit(0);
					}
					queue_next_states(curr_state) ;
					visit = visit->next; 
				}
			}
		}
	// }
