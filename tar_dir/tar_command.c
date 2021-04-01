#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#define false 0 
#define true 0 

typedef struct file_data{ 
    int path_length; 
    char * path ;
    long long content_size; 
    char * content ;
} FILE_DATA; 

typedef struct Node{ 
    FILE_DATA * file ; 
    struct Node * next; 
} NODE; 

NODE * head = NULL; 
NODE * tail = NULL; 
FILE * pFile;
FILE * out; 
long lSize;
char * buffer;
size_t result;

void add_node(FILE_DATA * f) {
    if( head == NULL) {
        head = (NODE*)malloc(sizeof(NODE));
        head->next = NULL;
        head->file = f;
        tail = head;
    }else{
        tail->next = (NODE*)malloc(sizeof(NODE));
        tail = tail->next; 
        tail->next = NULL; 
        tail->file = f;
    }
}

FILE_DATA * alloc_file (int path_length, char * path, long long content_size, char * content) { 
    FILE_DATA *f =  (FILE_DATA*)malloc(sizeof(FILE_DATA)) ; 
    f->path = (char*)malloc(sizeof(char) * path_length);
    f->content = (char*)malloc(sizeof(char) * content_size); 

    f->path_length = path_length; 
    strcpy(f->path, path);
    printf("ALLOC: _path: %s\n", path);
    f->content_size = content_size; 
    for(int i = 0 ; i < content_size; i++) {
        f->content[i] = content[i];
    }
    printf("ALLOC: _content: %s\n", content);

    return f;
}

void dir_files_show(char * dir_name){ 
  char tmp_name[100];
  strcpy(tmp_name, dir_name);
  DIR * dp = opendir(tmp_name);
  struct dirent * ep ; 

  if( dp != NULL) {
    while( ep = readdir(dp)) {
      strcpy(tmp_name, dir_name);
      int check = false; 
      if( (strlen(ep->d_name) == 1 && ep->d_name[0] == '.') || (strlen(ep->d_name) == 2 && ep->d_name[0] == '.')) continue; 
      if( ep->d_type == DT_DIR){
        strcat(tmp_name, ep->d_name);
        strcat(tmp_name,"/");
        dir_files_show(tmp_name);
      }else{
          strcat(tmp_name, ep->d_name);
          printf("TEST: %s\n", tmp_name);
          pFile = fopen ( tmp_name , "rb" );
          if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
          
          // obtain file size:
          fseek (pFile , 0 , SEEK_END);
          lSize = ftell (pFile);
          rewind (pFile);
          // allocate memory to contain the whole file:
          buffer = (char*) malloc (sizeof(char)*lSize);
          if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

          char tmp[50];
          strcpy(tmp, "./store_dir/");
          strcat(tmp, ep->d_name);
          out = fopen(tmp, "wb");
          if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
          while ( (result=fread(buffer,1,lSize,pFile)) != 0 ) {
            fwrite( buffer, 1, result, out );
          }
          printf("NAME LENGTH: %ld , NAME: %s, CONTENT LENGTH: %ld\n",strlen(ep->d_name), ep->d_name, lSize );
          puts (ep->d_name);
      }
    }
    (void) closedir (dp);
  }
}

void tar_archive (char * tar_name, char * dir_name) {
    // dir안에 파일 관련 내용 가져오기 

}

void tar_list (char * archive_name) {

}

void tar_extract (char * archive_name) {

}

int main(int argc, char * argv[]){ 
    if( !strcmp(argv[1], "star") ) {
        if( !strcmp(argv[2], "archive") ){
            tar_archive(argv[3], argv[4]);
        }else if(!strcmp(argv[2], "list")){
            tar_list(argv[3]);
        }else if(!strcmp(argv[2], "extract")){
            tar_extract(argv[3]);
        }else{
            printf("Command not found\n");
        }
    }else{
        printf("%s: command not found", argv[1]);
    }


    return 0;
}
