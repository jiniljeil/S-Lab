#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#define false 0 
#define true 0 

void subdir_files_find_content(char * dir_name, FILE * fp){
  	
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
        		       	subdir_files_find_content(curr_path,fp);
      			}else{
     	                 	strcpy(curr_path, dir_name);
				strcat(curr_path, ep->d_name); 
       				
				// FILE에 데이터 쓰기 
                                find_file = fopen( curr_path, "rb");
                                if( find_file == NULL) {
                                        fputs("Fill error", stderr); exit(1);
                                }
                                // printf("Recursion Path: %s\n", curr_path);
                                fseek(find_file, 0, SEEK_END) ;
                                file_size = ftell(find_file);
                                rewind(find_file);

                                char * buffer = (char*)malloc(sizeof(char) * file_size);
                                if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

                                int result = fread(buffer, 1, file_size, find_file);
                                if (result != file_size) {fputs("Reading error",stderr); exit(3);}

                                // temp : file_path 
                                path_length = strlen(curr_path);

                                /*fwrite(&path_length,4,1, fp);
                                fwrite(curr_path, path_length, 1, fp);
                                fwrite(&file_size,4, 1, fp);*/
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

void tar_archive_content (char * tar_name, char * dir_name, FILE * fp) {
	// dir안에 파일 관련 내용 가져오기 
	DIR * dp;
	struct dirent *ep ;
	//FILE * fp = fopen(tar_name, "wb"); 
	FILE * find_file ; 
	
	if( dir_name[strlen(dir_name) - 1] != '/') { 
		strcat(dir_name, "/"); 
	}

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
				subdir_files_find_content(curr_path,fp); 
			}else{ 				
				strcpy(curr_path, curr_dir);  
				strcat(curr_path, ep->d_name); 
				// FILE에 데이터 쓰기 
				find_file = fopen( curr_path, "rb"); 
				if( find_file == NULL) {
					fputs("Fill error", stderr); exit(1);
				}
				//printf("Curr Path: %s\n", curr_path);
				fseek(find_file, 0, SEEK_END) ;
				file_size = ftell(find_file); 
				rewind(find_file); 
				
				char * buffer = (char*)malloc(sizeof(char) * file_size); 
				if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
				
				int result = fread(buffer, 1, file_size, find_file); 
				if (result != file_size) {fputs("Reading error",stderr); exit(3);}
				
				// temp : file_path 
				path_length = strlen(curr_path);
				
				/*fwrite(&path_length,4,1, fp);
                                fwrite(curr_path, path_length, 1, fp);
                                fwrite(&file_size,4, 1, fp);*/
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
                  //              fwrite(buffer, file_size, 1, fp);
                                printf("%s\n", ep->d_name);
                  //              free(buffer);
                                fclose(find_file) ;
                                free(curr_path);
      			}
    		}
    		(void) closedir (dp);
  	}
}


void tar_archive (char * tar_name, char * dir_name) {
	// dir안에 파일 관련 내용 가져오기 
	DIR * dp;
	struct dirent *ep ;
	FILE * fp = fopen(tar_name, "wb"); 
	FILE * find_file ; 
	
	if( dir_name[strlen(dir_name) - 1] != '/') { 
		strcat(dir_name, "/"); 
	}

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
				
				/*char * buffer = (char*)malloc(sizeof(char) * file_size); 
				if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
				
				int result = fread(buffer, 1, file_size, find_file); 
				if (result != file_size) {fputs("Reading error",stderr); exit(3);}
				*/
				// temp : file_path 
				path_length = strlen(curr_path);
				
				fwrite(&path_length,4,1, fp);
                                fwrite(curr_path, path_length, 1, fp);
                                fwrite(&file_size,4, 1, fp);
                                //fwrite(buffer, file_size, 1, fp);

				printf("%s\n", ep->d_name);
		//		free(buffer); 	
				fclose(find_file) ;		
			}

			free(curr_path);
		}
	}	
	tar_archive_content(tar_name, dir_name, fp);
	//fclose(fp); 
}

int num_of_files (char * archive_name) { 
	FILE * fp = fopen(archive_name, "rb");
	int n = 0 ; 
	int path_length = 0, file_size = 0 ; 
	
	while(fread(&path_length, 4, 1, fp)) {
                char * path = (char*)malloc(sizeof(char) * (path_length + 1) );
                fread(path, path_length, 1, fp);
                path[path_length] = '\0';
                fread(&file_size, 4, 1, fp) ;
                n++; 
                free(path);
        }
	fclose(fp);
	return n - 1; 
}


void tar_list (char * archive_name) {
	FILE * fp = fopen(archive_name, "rb"); 
	int path_length = 0 ;
	int file_size = 0 ;
	int n = 0 ;
        int *file_size_arr = (int *)malloc(sizeof(int) * 100); 
	int header_n_local;
	while(fread(&path_length, 4, 1, fp)) {	
		char * path = (char*)malloc(sizeof(char) * (path_length + 1) );
		fread(path, path_length, 1, fp); 
		path[path_length] = '\0';
		fread(&file_size, 4, 1, fp) ; 
		//char * buffer = (char*)malloc(sizeof(char) * (file_size + 1)); 
		//fread(buffer, file_size, 1, fp); 
		file_size_arr[n++] = file_size;  	
		printf("%s\n", path);  
		free(path); 
	//	free(buffer);
	}
	fclose(fp); 
	fp = fopen(archive_name, "rb"); 
	header_n_local = n - 1;
	while(header_n_local-- && fread(&path_length, 4, 1, fp)) { 
		char * path = (char*)malloc(sizeof(char) * (path_length + 1) );
                fread(path, path_length, 1, fp);
                path[path_length] = '\0';
                fread(&file_size, 4, 1, fp) ;
                //char * buffer = (char*)malloc(sizeof(char) * (file_size + 1)); 
                //fread(buffer, file_size, 1, fp); 
                file_size_arr[n] = file_size;
                free(path);
	}
	for(int i = 0 ; i < n - 1 ; i++) { 
		char * buffer = (char*)malloc(sizeof(char) *  (file_size_arr[i] + 1)); 
		fread(buffer, file_size_arr[i], 1 , fp); 
		buffer[file_size_arr[i]]  = '\0'; 
		printf("%d: BUF: %s\n", i+1, buffer); 
		free(buffer); 
	}
	free(file_size_arr); 
	fclose(fp); 
}

void make_directory(char * path) { 
	char * dir_path = (char*)malloc(sizeof(char) * (strlen(path) + 10 ));
        strcpy(dir_path, "mkdir "); 
	strcat(dir_path, path); 
	system(dir_path); 
	free(dir_path);	
}

void tar_extract (char * archive_name) {
	FILE * fp = fopen(archive_name, "rb"); 
	int path_length = 0 ; 
	int file_size = 0 ; 
	int n = 0 ;
        int start = false; 	
	int dir_c = 0 ;

	int dir_list_count = 0 ; 
	char ** dir_list = (char**)malloc(sizeof(char) * 100) ; 
	char ** path_list = (char**)malloc(sizeof(char) * 100) ; 
	int *file_size_arr = (int *)malloc(sizeof(int) * 100);

	int num_files = num_of_files(archive_name); 
	int n_files = num_files;
	
	while(num_files-- && fread(&path_length, 4, 1, fp)) { 
		char * path = (char*)malloc(sizeof(char) * (path_length + 1)) ; 
		fread(path, path_length, 1,fp); 
		path[path_length] = '\0'; 
		path_list[n] = (char*)malloc(sizeof(char) * (path_length + 1)); 
		strcpy(path_list[n], path); 
		
		fread(&file_size, 4, 1, fp); 
		file_size_arr[n++] = file_size; 
		/*char * buffer = (char*)malloc(sizeof(char) * (file_size + 1));
                fread(buffer, file_size, 1, fp);
		buffer[file_size] = '\0'; */
		char * compare; 
		for(int i = path_length-1; i >= 0 ; i--) { 
			if( path[i] == '/') {
				compare = (char*)malloc(sizeof(char) * (i+5)); 
				for(int j = 0 ; j <= i ; j++) { 
					compare[j] = path[j];		
				}
				compare[i+1] = '\0';
				break;
			}		
		}
		
		//printf("%s | %s\n", path, buffer); 
			
		// check list 
		if( dir_list_count == 0 ) {
			dir_list[dir_list_count] = (char*)malloc(sizeof(char) * (strlen(compare) + 1));
			strcpy( dir_list[dir_list_count], compare);
		        make_directory(compare);	
			dir_list_count++;
		}
		int same_check = 2; 
		for(int i = 0 ; i < dir_list_count; i++) { 
			if( !strcmp(dir_list[i], compare)) {
				same_check = 1; 			
				break;
			}
		}
		if(same_check == 2) { 
			dir_list[dir_list_count] = (char*)malloc(sizeof(char) * (strlen(compare) + 1));
			strcpy( dir_list[dir_list_count], compare); 
			dir_list_count++;
			make_directory(compare);
		}

				
		free(compare); 
			
		free(path);
                //free(buffer);
	}

	for(int i = 0 ; i < n_files; i++) { 
		char * buffer = (char*)malloc(sizeof(char) * (file_size_arr[i] + 1));
                fread(buffer, file_size_arr[i], 1, fp);
                buffer[file_size_arr[i]] = '\0'; 
		FILE * f = fopen(path_list[i], "w"); 
		
		fwrite(buffer, file_size_arr[i], 1,f);  	
                
		free(buffer); 
		fclose(f);
	}

	free(file_size_arr); 
	for(int i = 0 ; i < n_files; i++) { 
		free(path_list[i]); 
	}
	for(int i = 0 ; i < dir_list_count; i++) { 
		free(dir_list[i]); 
	}
	free(path_list); free(dir_list);
}

int main(int argc, char * argv[]){ 
    if( !strcmp(argv[1], "star") ) {
        if( !strcmp(argv[2], "archive") ){
            tar_archive(argv[3], argv[4]);
       //     tar_archive_content(argv[3], argv[4]);
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
