#include <stdio.h> 
typedef struct { int left , top , right , bottom; } RECT; 
int main() { 
	RECT rect; FILE *file = fopen("rect.dat" , "rb"); 
	if (file == NULL) { 
		printf("파일을 읽기 모드로 열 수 없습니다.\n"); 
		return 0; 
	} 
	fread(&rect , sizeof(RECT) , 1 , file); 
	fclose(file); 
	printf("left=%d, top=%d, right=%d, bottom = %d\n" , rect.left , rect.top , rect.right , rect.bottom); 
	return 0; }

