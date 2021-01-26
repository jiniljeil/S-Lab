#include <stdio.h> 
typedef struct { int left , top , right , bottom; } RECT; 
int main() { 
	FILE *file; RECT rect; printf("사각형의 네 모서리의 좌표를 나타내는 숫자를 입력하십시오. >"); 
	scanf("%d %d %d %d" , &rect.left , &rect.top , &rect.right , &rect.bottom); 
	printf("left=%d, top=%d, right=%d, bottom=%d\n", rect.left , rect.top , rect.right , rect.bottom); 
	file = fopen("rect.txt" , "wb"); 
	if (file == NULL) { 
		printf("파일을 쓰기 모드로 열 수 없습니다."); return 0; 
	} 
	fwrite(&rect , sizeof(RECT) , 1 , file); fclose(file); 
	return 0; 
}
