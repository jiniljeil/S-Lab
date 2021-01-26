#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#define false 0 
#define true 0 

void subdir_files_find(char * dir_name, FILE * fp){
  	
  	DIR * dp = opendir(dir_name); // ex)
  	struct dirent * ep ;
	FILE * find_file ;
        int file_size = 0 ;
        int path_length = 0 ;

  	if( dp != NULL) {
    		while( ep = readdir(dp)) {
			char *curr_path = (char*)malloc(sizeof(char) * (strlen(dir_name) + strlen(ep->d_name) + 3));
		       	if( (strlen(ep->d_name) == 1 && ep->d_name[0] == '.') || (strlen(ep->d_name) == 2 && ep->d_name[1] == '.')) continue;
	     		if( ep->d_type == DT_DIR){
				strcpy(curr_path, dir_name); 
				strcat(curr_path, ep->d_name);
				strcat(curr_path, "/"); 
        		       	subdir_files_find(curr_path,fp);
      			}else{
     	                 	strcpy(curr_path, dir_name);
				strcat(curr_path, ep->d_name); 
       				
				// FILE에 데이터 쓰기 
                                find_file = fopen( curr_path, "rb");
                                if( find_file == NULL) {
                                        fputs("Fill error", stderr); exit(1);
                                }
                                printf("Recursion Path: %s\n", curr_path);
                                fseek(find_file, 0, SEEK_END) ;
                                file_size = ftell(find_file);
                                rewind(find_file);

                                char * buffer = (char*)malloc(sizeof(char) * file_size);
                                if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

                                int result = fread(buffer, 1, file_size, find_file);
                                if (result != file_size) {fputs("Reading error",stderr); exit(3);}

                                // temp : file_path 
                                path_length = strlen(curr_path);

                                fwrite(&path_length,4,1, fp);
                                fwrite(curr_path, path_length, 1, fp);
                                fwrite(&file_size,4, 1, fp);
                                fwrite(buffer, file_size, 1, fp);

                                printf("%s\n", ep->d_name);
                                free(buffer);
                                fclose(find_file) ;
                                free(curr_path);
      			}
    		}
    		(void) closedir (dp);
  	}
}

void subdir_files_extract_ (char * dir_name) {
	// Produce Directory
	char * make_dir = (char*)malloc(sizeof(char) * (strlen(dir_name) + 10)); 
	strcpy(make_dir, "mkdir "); 
	strcat(make_dir, dir_name); 
	system(make_dir); 


}

void tar_archive (char * tar_name, char * dir_name) {
	// dir안에 파일 관련 내용 가져오기 
	DIR * dp;
	struct dirent *ep ;
	FILE * fp = fopen(tar_name, "wb"); 
	FILE * find_file ; 
	int file_size = 0 ; 
	int path_length = 0 ;
	// path_length | path | file_size | file content  
	char * curr_dir = (char*)malloc(sizeof(char) * strlen(dir_name) + 1); 
	strcpy(curr_dir, dir_name);
	dp = opendir(curr_dir); 
	if( dp != NULL) { 
		while(ep = readdir (dp)){
			char *curr_path = (char*)malloc(sizeof(char) * (strlen(ep->d_name) + strlen(curr_dir) + 3 )) ;
			
			if( (strlen(ep->d_name) == 1 && ep->d_name[0] == '.') || (strlen(ep->d_name) == 2 && ep->d_name[1] == '.')) continue; 
			if( ep->d_type == DT_DIR) { 
				strcpy(curr_path, curr_dir); 
				strcat(curr_path, ep->d_name);
				strcat(curr_path, "/"); 
				subdir_files_find(curr_path,fp); 
			}else{ 				
				strcpy(curr_path, curr_dir);  
				strcat(curr_path, ep->d_name); 
				// FILE에 데이터 쓰기 
				find_file = fopen( curr_path, "rb"); 
				if( find_file == NULL) {
					fputs("Fill error", stderr); exit(1);
				}
				printf("Curr Path: %s\n", curr_path);
				fseek(find_file, 0, SEEK_END) ;
				file_size = ftell(find_file); 
				rewind(find_file); 
				
				char * buffer = (char*)malloc(sizeof(char) * file_size); 
				if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
				
				int result = fread(buffer, 1, file_size, find_file); 
				if (result != file_size) {fputs("Reading error",stderr); exit(3);}
				
				// temp : file_path 
				path_length = strlen(curr_path);
				
				fwrite(&path_length,4,1, fp);
                                fwrite(curr_path, path_length, 1, fp);
                                fwrite(&file_size,4, 1, fp);
                                fwrite(buffer, file_size, 1, fp);
				
				printf("%s\n", ep->d_name);
				free(buffer); 	
				fclose(find_file) ;		
			}

			free(curr_path);
		}
	}	
	fclose(fp); 
}

void tar_list (char * archive_name) {
	FILE * fp = fopen(archive_name, "rb"); 
	int path_length = 0 ;
	int file_size = 0 ;
	int n = 0 ; 
	while((n = fread(&path_length, 4, 1, fp) != 0)) { 
		char * path = (char*)malloc(sizeof(char) * (path_length + 1) );
		fread(path, path_length, 1, fp); 
		fread(&file_size, 4, 1, fp) ; 
		char * buffer = (char*)malloc(sizeof(char) * (file_size + 1)); 
		fread(buffer, file_size, 1, fp); 
		
		printf("%s\n", path);  
		free(path); 
		free(buffer);
	}
	fclose(fp); 
}

void make_directory(char * path) { 
	for(int i =  strlen(path) - 1; i >= 0; i--) { 
		if(path[i] == '/') {  // 끝에 존재하는 / 
			char * dir_name = (char*)malloc(sizeof(char)* (i)) ; 
			for(int j = 0; j < i ; j++) { 
				dir_name[j] = path[j]; 
			}
			dir_name[strlen(dir_name)] = '\0'; 
			char * command = (char*)malloc(sizeof(char) * (i + 10)); 
			strcpy( command, "mdkir "); 
			strcat( command, dir_name); 
			system( command ); 
			free(dir_name); 
			free(command); 
		}	
	}
}

void tar_extract (char * archive_name) {
	FILE * fp = fopen(archive_name, "rb"); 
	int path_length = 0 ; 
	int file_size = 0 ; 
	int n = 0 ;
        int start = false; 	
	while((n = fread(&path_length, 4, 1, fp) != 0)) { 
		char * path = (char*)malloc(sizeof(char) * (path_length + 1)) ; 
		fread(path, path_length, 1,fp); 
		fread(&file_size, 4, 1, fp); 
		char * buffer = (char*)malloc(sizeof(char) * (file_size + 1));
                fread(buffer, file_size, 1, fp);

                if( !start ) { 
			make_directory(path); 
			start = true; 
		}
		
		FILE * f = fopen(path, "r"); 
		
		fputs(buffer, f); 
                
		fclose(f); 	
		free(path);
                free(buffer);
	}
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
