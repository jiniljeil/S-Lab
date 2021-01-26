#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#define false 0 
#define true 0 


void dir_files_show(char * dir_name){ 
  char tmp_name[100];
  strcpy(tmp_name, dir_name);
  printf("FF: %s\n",tmp_name);
  DIR * dp = opendir(tmp_name);
  struct dirent * ep ; 
  if( dp != NULL) {
    while( ep = readdir(dp)) {
      int check = false; 
      if( (strlen(ep->d_name) == 1 && ep->d_name[0] == '.') || (strlen(ep->d_name) == 2 && ep->d_name[1] == '.')) continue; 
      if( ep->d_type == DT_DIR){
        strcat(tmp_name,"/");
        strcat(tmp_name, ep->d_name);
        dir_files_show(tmp_name);
      }else{
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
        puts (ep->d_name);
      }
    }
    (void) closedir (dp);
  }else{
    perror ("Couldn't open the directory");
  }

  return 0;
}
