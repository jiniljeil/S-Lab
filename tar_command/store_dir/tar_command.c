#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h>
#include <dirent.h>

typedef struct { 
    struct stat file; 
    
} FILE_BLOCK; 

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
gv[1]);
    }


    return 0;
}
