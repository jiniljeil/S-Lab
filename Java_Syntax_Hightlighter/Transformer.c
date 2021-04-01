#include <stdio.h>
#include <regex.h>

int main(void) { 
    regex_t regex; 
    FILE *fp, *f; 
    char filename[100];
    printf("Enter a filename: ") ;
    scanf("%s", filename);
    fp = fopen(filename, "r");

    // .java -> .html 
    int idx = 0 ; 
    for(int i = 0 ; i < sizeof(filename)/sizeof(char); i++) {
        if( filename[i] == '.') {
            idx = i; 
        }
    }
    filename[idx+1] = 'h'; filename[idx+2] = 't'; filename[idx+3] = 'm'; filename[idx+4] = 'l';
    fp = fopen(filename, "w");

    fprintf("<html lang=\"en\"><head><meta charset=\"UTF-8\">
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
    <title>Document</title></head><body>");

    while(!feof(fp)){ 
        char buf[255]; 

        fscanf(fp, "<p>%s</p>", buf);
        // char reg_exp
        // int regi = regcomp(&regex, "public class+", 0);
        // if(regi){
        //     fprintf(stderr, "Could not compile regex\n");
        //     exit(1);
        // }
        // regi = regexec(&regex, buf, 0, NULL, 0); 
        // if( !regi ) {
        //     // match!!
        // }else{

        // }
    }
    fprintf("</body>\n</html>")
    fclose(fp) ;
    fclose(f) ;
    free(&regex);
    return 0;
}