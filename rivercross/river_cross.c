#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef enum { 
	left,
	right
} Location; 

typedef struct State {
	struct State * prev; 

	Location wolf; 
	Location sheep;
	Location cabbage; 
	Location human; 
} State; 

typedef struct Node {
	State * state; 
	struct Node * next; 
} NODE; 

NODE * head = NULL ;
NODE * tail = NULL ; 
NODE * visited = NULL ; 

void queue (State * s) { 
	if( head == NULL ) { 
		head = (NODE*)malloc(sizeof(NODE)); 
		head->state = s; 
		head->next = NULL; 
		visited = tail = head; 
	}else{ 
		tail->next = (NODE*)malloc(sizeof(NODE)); 
		tail = tail->next;
	        tail->state = s; 	
		tail->next = NULL;
	}
}

State * alloc_state (Location wolf, Location sheep, Location cabbage, Location human, State * prev) { 
	State *s = (State*)malloc(sizeof(State)); 
	s->prev = prev; 

	s->wolf = wolf; 
	s->sheep = sheep; 
	s->cabbage = cabbage; 
	s->human = human; 
}

int all_of_right_state (State * s) { 
	return (s->wolf == right) && (s->sheep == right) && (s->cabbage == right) && (s->human == right); 
}
int is_valid_state(State * s) { 
	if((s->human != s->wolf) && (s->human != s->sheep)) return 0;
	if((s->human != s->sheep) && (s->human != s->cabbage)) return 0; 
	return 1; 
}

State** queue_next_states (State * s){  
	State ** next_state = (State **)malloc(sizeof(State*) * 4); 
	next_state[0] = alloc_state(s->wolf, s->sheep, s->cabbage, !s->human, s);
	next_state[1] = alloc_state(!s->wolf, s->sheep, s->cabbage, !s->human, s); 	
	next_state[2] = alloc_state(s->wolf, !s->sheep, s->cabbage, !s->human, s);
       	next_state[3] = alloc_state(s->wolf, s->sheep, !s->cabbage, !s->human, s); 
	return next_state;
}

int equal_state(State * a, State * b) { 
	return (a->wolf == b->wolf) && (a->sheep == b->sheep) && (a->cabbage == b->cabbage) && (a->human == b->human); 
}

int is_visited_state (State * s) {
	NODE * i; 
	for (i = head ; i != NULL ; i = i->next) { 
		if( equal_state(i->state, s)) { 
			return 1; 
		}	
	}
	return 0 ;
}

void print_state(State * s){ 
	if( s == NULL ) return ;

	printf("------------------------------\n"); 
	printf("Wolf | Sheep | Cabbage | Human\n");
	printf(" %d   |   %d   |    %d    |   %d \n", s->wolf, s->sheep, s->cabbage, s->human); 
	printf("------------------------------\n");
}

void print_path(State * s) { 
	do{ 
		print_state(s); 
		s = s->prev;
	}while(s != NULL);
}
int main(void) {
	State * first = alloc_state(left, left, left, left, NULL);
	queue(first); 

	while(visited != NULL) { 
		State * curr_state = visited->state;	
		if( all_of_right_state(curr_state)) { 
			print_path(curr_state);	
			exit(0); 
		}	
		
		State **next_state = queue_next_states(curr_state);
			
		for(int i = 0 ; i < 4; i++) { 
			if( !is_valid_state(next_state[i]) || is_visited_state(next_state[i])) continue;
			queue(next_state[i]); 

		}
		visited = visited -> next; 
	}	
	return 0;
}

