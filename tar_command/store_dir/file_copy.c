#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#define false 0 
#define true 0 

FILE * pFile;
FILE * out; 
long lSize;
char * buffer;
size_t result;

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

int
main (void)
{
  DIR *dp;
  struct dirent *ep;

  dp = opendir ("./");   
    if (dp != NULL){
        while (ep = readdir (dp)){
            int check = false; 
            if( ep->d_name[0] == '.' || ep->d_name[1] == '.') continue; 
            if( ep->d_type == DT_DIR){
              char dir_n[100]; // path 작업하면 가능
                strcpy(dir_n, ep->d_name); 
                strcat(dir_n, "/");
                dir_files_show(dir_n);
            }else{
                pFile = fopen ( ep->d_name , "rb" );
                if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

                // obtain file size:
                fseek (pFile , 0 , SEEK_END);
                lSize = ftell (pFile);
                rewind (pFile);

                // allocate memory to contain the whole file:
                buffer = (char*) malloc (sizeof(char)*lSize);
                if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

                // copy the file into the buffer:
                // result = fread (buffer,1,lSize,pFile);
                // if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
                char tmp[50];
                strcpy(tmp, "./store_dir/");
                strcat(tmp, ep->d_name);
                out = fopen(tmp, "wb");
                if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
                while ( (result=fread(buffer,1,lSize,pFile)) != 0 ) {
                  fwrite( buffer, 1, result, out );
                }
                printf("NAME LENGTH: %ld , NAME: %s, CONTENT LENGTH: %ld\n",strlen(ep->d_name), ep->d_name, lSize );
                fclose (pFile);
                fclose (out);
           }

        }
        (void) closedir (dp);
    }else{
    perror ("Couldn't open the directory");
}

   return 0;
}
