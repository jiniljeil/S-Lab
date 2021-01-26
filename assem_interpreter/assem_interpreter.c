#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char commands[100][5][7];
int input; 
char num[20];
void asm_read(int m) {
	scanf("%d", &input);
	sprintf(num, "%d", input);
	strcpy(commands[m][0],num); 
}
void asm_print(int m) {
	printf("%s\n", commands[m][0]);
}
void asm_assign(int mx, int my){
	strcpy(commands[mx][0], commands[my][0]);
}
void asm_define(int m, int c) {
	sprintf(num, "%d", c);
	strcpy(commands[m][0], num);
}
void asm_move(int md, int ms) {
	strcpy(commands[md][0], commands[ms][0]);
}
void asm_add(int md, int mx, int my) {
	sprintf(num, "%d", atoi(commands[mx][0]) + atoi(commands[my][0]));
	strcpy(commands[md][0], num);
}
void asm_minus(int md, int mx, int my) {
	sprintf(num, "%d", atoi(commands[mx][0]) - atoi(commands[my][0]));
	strcpy(commands[md][0], num);
}
void asm_mod(int md, int mx, int my) {
	sprintf(num, "%d", atoi(commands[mx][0]) % atoi(commands[my][0]));
	strcpy(commands[md][0], num );
}
void asm_eq(int md, int mx, int my) {
	// printf("TEST| 0: %s | 1: %s\n", commands[mx][0], commands[my][0]);
	if(commands[mx][0][0] == commands[my][0][0]) {
		// printf("EQUAL\n");
		commands[md][0][0] = '1';
	}else{
		commands[md][0][0] = '0';
	}
}
void asm_less(int md, int mx, int my) {
	if (atoi(commands[mx][0]) < atoi(commands[my][0])) {
		commands[md][0][0] = '1';
	}else{
		commands[md][0][0] = '0';
	}
}
int asm_jump(int m, int c, int curr) {
	// printf("%s\n", commands[m][0]);
	if( commands[m][0][0] == '1' ) {
		return c; 
	}else{
		return curr + 1;
	}
}

int main(int argc, char* argv[]) {
	char buf[25]; // filename	
	int count = 0 ;
	FILE * fp = fopen(argv[1],"r"); 
	if( fp == NULL) exit(0); 
	while(!feof(fp)){
		fgets(buf, 25, fp); 
		int start = 0;
		char tmp[30]; 
		int tmp_idx = 0, tmp_i = 0 ; 
		for(int i = 0 ; i < 25; i++) { // : 까지
			if( buf[i] == ':' ) {
				tmp_idx = i ;
				break;
			}
		} 
		char location[4]; // 0 ~ 100
		for(int i = 0 ; i < tmp_idx; i++) { // ':' 앞에 숫자   
			location[i] = buf[i];
		}
		location[tmp_idx] ='\0';

		tmp_idx += 2; // : 의 idx + 2 

		// : 뒤에 부분 추려내기 
		for(int i = tmp_idx ; i < sizeof(buf)/sizeof(char); i++) {
			tmp[tmp_i++] = buf[i];
		}
		char * ptr = strtok(tmp," ");
		while( ptr != NULL) {
			for(int i = 0 ; i < sizeof(ptr)/sizeof(char); i++) {
				if( ptr[i] == '\n') {
					ptr[i] = ' ';
				}
			}
			strcpy(commands[atoi(location)][start], ptr); 
			start++;
			ptr = strtok(NULL, " ");
		}
	}
	fclose(fp); 
	int curr_idx = 0 ;
	int k = 0;
	
	while(strcmp(commands[curr_idx][0], "TERM") && curr_idx <= 100) {
		// printf("VALUE 21: %s | VALUE 22: %s | VALUE 23: %s\n", commands[21][0], commands[22][0], commands[23][0]);
		if(!strcmp(commands[curr_idx][0], "READ")) asm_read(atoi(commands[curr_idx][1]));
		else if(!strcmp(commands[curr_idx][0], "PRINT")) asm_print(atoi(commands[curr_idx][1]));
		else if(!strcmp(commands[curr_idx][0], "ASSIGN")) asm_assign(atoi(commands[curr_idx][1]), atoi(commands[curr_idx][2]));
		else if(!strcmp(commands[curr_idx][0], "DEFINE")) asm_define(atoi(commands[curr_idx][1]), atoi(commands[curr_idx][2]));
		else if(!strcmp(commands[curr_idx][0], "MOVE")) asm_move(atoi(commands[curr_idx][1]), atoi(commands[curr_idx][2]));
		else if(!strcmp(commands[curr_idx][0], "ADD")) asm_add(atoi(commands[curr_idx][1]), atoi(commands[curr_idx][2]), atoi(commands[curr_idx][3]));
		else if(!strcmp(commands[curr_idx][0], "MINUS")) asm_minus(atoi(commands[curr_idx][1]), atoi(commands[curr_idx][2]) , atoi(commands[curr_idx][3]));
		else if(!strcmp(commands[curr_idx][0], "MOD")) asm_mod(atoi(commands[curr_idx][1]), atoi(commands[curr_idx][2]), atoi(commands[curr_idx][3]));
		else if(!strcmp(commands[curr_idx][0], "EQ")) asm_eq(atoi(commands[curr_idx][1]), atoi(commands[curr_idx][2]), atoi(commands[curr_idx][3]));
		else if(!strcmp(commands[curr_idx][0], "LESS"))asm_less(atoi(commands[curr_idx][1]), atoi(commands[curr_idx][2]), atoi(commands[curr_idx][3]));	
		if(!strcmp(commands[curr_idx][0], "JUMP")) {
			curr_idx = asm_jump(atoi(commands[curr_idx][1]), atoi(commands[curr_idx][2]), curr_idx);	
		}else{
			curr_idx++;
		}
	}
	
	return 0;
}
