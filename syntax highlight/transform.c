#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#define MAX 300
#define FILE_SIZE 100
#define MAX_MATCH 20 
#define BUFF_SIZE 255
#define WORD_SIZE 128

int status;  
regex_t regex_expr; 

int main(void) { 
    FILE *fp, *f; 
    // Regular_Expression();
    char filename[FILE_SIZE];
    printf("Enter a filename: "); scanf("%s", filename);
    fp = fopen(filename, "r");

    // .java -> .html 
    int idx = 0 ; 
    for(int i = 0 ; i < sizeof(filename)/sizeof(char); i++) {
        if( filename[i] == '.') {
            idx = i; 
        }
    }
    // .html file
    char htmlfile[FILE_SIZE];
    for(int i = 0 ; i < idx; i++) {
        htmlfile[i] = filename[i];
    }
    htmlfile[idx] = '\0';
    strcat(htmlfile, ".html");

    f = fopen(htmlfile, "w");

    fprintf(f, "<html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Document</title></head><body>");
    int k = 0 ; 
    char before_word[WORD_SIZE];
    int range_on = 0;
    while(!feof(fp)){ 
        char buf[255]; 

        fgets(buf, sizeof(buf), fp);
        int start = 0, end = 0 ; 
        int p = 0 ;
        char * html_code = (char *) malloc (sizeof(char) * 2000); 
        for(int i = 0 ; i < 2000; i++) html_code[i] = '\0';

        strcpy( html_code, "<pre>"); 

        int comment_on = 0, line_on = 0;
        int space = 0, name_color = 0, point_method = 0 ;
        int small_quotes = 0, large_quotes = 0 ; 
        int import_finish = 0; 
        

        while(end < strlen(buf)) { 
            char * word = (char *) malloc(sizeof(char) * WORD_SIZE) ;
            for(int i = 0; i < WORD_SIZE; i++) word[i] = '\0';
            int word_idx = 0 ;
            for(int i = start; (buf[i] >= 'a' && buf[i] <= 'z')||(buf[i] >= 'A' && buf[i] <= 'Z'); i++) {
                word[word_idx++] = buf[i];
                end = i; 
            }

            // import, package (CLEAR)
            
            if( !strcmp(word, "import") || !strcmp(word,"package")) {
                if(!strcmp(word, "import")) {
                    status = regcomp(&regex_expr, "(\\s)*(import)( )+.+;", REG_EXTENDED);
                }else if(!strcmp(word, "package")){
                    status = regcomp(&regex_expr, "(\\s)*(package)( )+.+;", REG_EXTENDED);
                }

                if(status){
                    fprintf(stderr, "Could not compile regex\n");
                    exit(1);    
                }
                status = regexec(&regex_expr, buf, 0, NULL, 0); 
                if( !status ) {
                    int idx = 0; int whitespace_check = 0 ; 
                    for(int i = 0 ; ; i++) {
                        if( buf[i] == ';') {
                            idx = i ;
                            break;
                        }
                    }
                    for(int i = idx; i >= 0; i--) {
                        if((buf[i] >= 'a' && buf[i] <='z') || (buf[i] >= 'A' && buf[i] <= 'Z')) {
                            whitespace_check = 1; 
                            continue;
                        }
                        if(whitespace_check && buf[i] == ' ') { 
                            idx = i + 1;
                            break;
                        }
                    }
                    char * remainder_str = (char *)malloc(sizeof(char) * 200); 
                    int c = 0 ;
                    for(int i = idx; (buf[i] != '\n'); i++) {
                        remainder_str[c++] = buf[i];
                    }
                    remainder_str[strlen(buf) - idx - 1] = '\0';

                    char * color_word = (char*)malloc(sizeof(char) * 200); 
                    if( !strcmp(word, "import")) {
                        strcpy(color_word, "<span style=\"color: green\">import </span>"); 
                    }else if( !strcmp(word,"package")) {
                        strcpy(color_word, "<span style=\"color: green\">package </span>"); 
                    }
                    strcat(color_word, remainder_str); 
                    strcat(color_word, "</pre>");
                    strcat(html_code, color_word);
                    fprintf(f, "%s", html_code);
                    free(remainder_str); 
                    free(color_word);
                    import_finish = 1; 
                }
            }
            if( import_finish ) {
                break; 
            }
            // 기호 
            if( strlen(word) == 0 ) {
                
                if(!comment_on && large_quotes || small_quotes ) {
                    if( buf[end] == ';' || buf[end] == ')'){
                        large_quotes = 0 ; 
                        small_quotes = 0 ;
                        int len = strlen(html_code); 
                        html_code[len] = buf[end]; 
                        html_code[len+1] = '\0';
                    }else if(buf[end] == '\"'){
                        large_quotes = 0 ; 
                        char * color_word = (char *) malloc(sizeof(char) * 200); 
                        strcpy(color_word, "<span style=\"color: #FA6109\">&#34;</span>");
                        strcat(html_code, color_word);
                        free(color_word);
                    }else if(buf[end] == '\''){
                        small_quotes = 0 ;
                        char * color_word = (char *) malloc(sizeof(char) * 200); 
                        strcpy(color_word, "<span style=\"color: #FA6109\">&#39;</span>");
                        strcat(html_code, color_word);
                        free(color_word);
                    }else{
                        char * color_word = (char *) malloc(sizeof(char) * 200); 
                        strcpy(color_word, "<span style=\"color: #FA6109\">");
                        int len = strlen(color_word);
                        color_word[len] = buf[end]; color_word[len + 1] = '\0';
                        strcat(color_word, "</span>") ;
                        strcat(html_code, color_word);
                        free(color_word);
                    }
                }else if(!comment_on && buf[end] == '\"') {
                    large_quotes = 1;
                    char * color_word = (char *) malloc(sizeof(char) * 200); 
                    strcpy(color_word, "<span style=\"color: #FA6109\">&#34;</span>");
                    strcat(html_code, color_word);
                    free(color_word);
                }else if( !comment_on && buf[end] == '\'') {
                    small_quotes = 1; 
                    char * color_word = (char *) malloc(sizeof(char) * 200); 
                    strcpy(color_word, "<span style=\"color: #FA6109\">&#39;</span>");
                    strcat(html_code, color_word);
                    free(color_word);
                }else{
                    if( (buf[end] == '/' || buf[end] == '*')) {
                        if(end-1 >= 0 && buf[end-1] == '*' && buf[end] == '/' && range_on) {
                            range_on = 0 ;
                            comment_on = 0 ;
                        }
                        if( comment_on ) {
                            if(buf[end] == '/') {
                                line_on = 1; 
                            }else if(buf[end] == '*') {
                                range_on = 1; 
                            }
                        }
                        comment_on = 1; 
                        if( buf[end] == '/') {
                            char * color_word = (char *) malloc(sizeof(char) * 200); 
                            strcpy(color_word, "<span style=\"color: #B0AD9C\">&#47;</span>");
                            strcat(html_code, color_word);
                            free(color_word);
                        }
                        if( buf[end] == '*') {
                            char * color_word = (char *) malloc(sizeof(char) * 200); 
                            strcpy(color_word, "<span style=\"color: #B0AD9C\">&#42;</span>");
                            strcat(html_code, color_word);
                            free(color_word);
                        }
                    }else{
                        if( line_on || range_on) {
                            char * color_word = (char *) malloc(sizeof(char) * 200); 
                            strcpy(color_word, "<span style=\"color: #B0AD9C\">");
                            int len = strlen(color_word); 
                            color_word[len] = buf[end]; 
                            color_word[len + 1] = '\0';
                            strcat(color_word, "</span>");
                            strcat(html_code, color_word);
                            free(color_word);
                        }else if(buf[end] == '@'){
                            char * color_word = (char *)malloc(sizeof(char) * 200) ; 
                            strcpy(color_word, "<span style=\"color: #C81DF1\">");
                            strcat(color_word, buf); 
                            strcat(color_word, "</span>"); 
                            strcat(html_code, color_word); 
                            free(color_word);
                            break;
                        }else{
                            /* \n 걸러 낼 필요 있음 */
                            if( (end - 1 >= 0 && !((buf[end-1] >= 'a' && buf[end-1] <= 'z') || (buf[end-1] >= 'A' && buf[end-1] <= 'Z')) && 
                                buf[end+1] != '\n' && !((buf[end+1] >= 'a' && buf[end+1] <= 'z') || (buf[end+1] >= 'A' && buf[end+1] <= 'Z')) 
                                && buf[end] >= '0' && buf[end] <= '9')) {
                                char * color_word = (char *)malloc(sizeof(char) * 100) ; 
                                strcpy(color_word, "<span style=\"color: #3B8CFA\">");
                                int len = strlen(color_word); 
                                color_word[len] = buf[end]; 
                                color_word[len+1] = '\0';  
                                strcat(color_word, "</span>"); 
                                strcat(html_code, color_word);
                                free(color_word);
                            }else{
                                int len = strlen(html_code); 
                                html_code[len] = buf[end]; 
                                html_code[len+1] = '\0';
                            }
                        }
                        
                        if(buf[end] == ' '){
                            space = 1; 
                        }
                        if(buf[end] == '('){
                            if( point_method ) {
                                point_method = 0;
                            }
                        }
                        if(buf[end] == '.'){
                            point_method = 1 ;
                        }
                    }
                }
            }else{
                /* 단어인 경우*/ 
                word[strlen(word)] = '\0';
                if( space == 1) {
                    int check = 1 ;
                    for(int i = end + 1; i < strlen(buf); i++) {
                        if( check && buf[i] == '(') {
                            name_color = 1; 
                            break; 
                        }
                        if( (buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= 'A' && buf[i] <= 'Z')) {
                            check = 0;
                            space = 0 ;
                            name_color = 0;
                            break;
                        }
                    }
                } 
                int match_check = 0; 
                if(line_on == 1) { // 한 줄 주석 
                    match_check = 1; 
                    char * color_word = (char *) malloc(sizeof(char) * 200); 
                    strcpy(color_word, "<span style=\"color:"); 
                    strcat(color_word, "#B0AD9C"); 
                    strcat(color_word, "\">");
                    strcat(color_word, word); 
                    strcat(color_word, "</span>"); 
                    strcat(html_code, color_word);
                    free(color_word);
                }else if( range_on == 1){ // 여러 줄 주석 
                    match_check = 1;
                    char * color_word = (char *) malloc(sizeof(char) * 200); 
                    strcpy(color_word, "<span style=\"color:#B0AD9C\">"); 
                    strcat(color_word, word); 
                    strcat(color_word, "</span>"); 
                    strcat(html_code, color_word);
                    free(color_word);
                }else if(large_quotes || small_quotes){ // 큰 따옴표, 작은 따옴표 
                    match_check = 1;
                    char * color_word = (char *) malloc(sizeof(char) * 200); 
                    strcpy(color_word, "<span style=\"color:#FA6109\">"); 
                    strcat(color_word, word); 
                    strcat(color_word, "</span>"); 
                    strcat(html_code, color_word);
                    free(color_word);
                }
                else if( point_method == 1) { // method 
                    match_check = 1;
                    char * color_word = (char *) malloc(sizeof(char) * 200); 
                    strcpy(color_word, "<span style=\"color:orange\">"); 
                    strcat(color_word, word); 
                    strcat(color_word, "</span>"); 
                    strcat(html_code, color_word);
                    free(color_word);
                    point_method = 0;
                }else{ // Keyword 
                    if( !strcmp( before_word, "class") || !strcmp(before_word, "interface") ){
                        match_check = 1;
                        char * color_word = (char *) malloc(sizeof(char) * 200); 
                        strcpy(color_word, "<span style=\"color:blue\">"); 
                        strcat(color_word, word); 
                        strcat(color_word, "</span>"); 
                        strcat(html_code, color_word);
                        free(color_word); 
                        strcpy(before_word, "");
                    }else{
                        int check = 0 ; 
                        int match_c = 0 ;
                        status = regcomp(&regex_expr, "(public$|private$|protected$|default$|class$|interface$|extends$|implements$|static$|package$|import$super$|this$|enum$|if$|else$|switch$|case$|break$|default$|catch$|throw$|new$|while$|do$|for$|true$|continue$|return$|finally$|synchronized$|assert$|final$)+", REG_EXTENDED);

                        if(status){
                            fprintf(stderr, "Could not compile regex\n");
                            exit(1);
                        }
                        
                        status = regexec(&regex_expr, word, 0, NULL, 0); 
                        
                        if( !status ) {
                            match_check = 1;
                            char * color_word = (char *) malloc(sizeof(char) * 200); 
                            strcpy(color_word, "<span style=\"color: green\">"); 
                            strcat(color_word, word); 
                            strcat(color_word, "</span>"); 
                            strcat(html_code, color_word);
                            free(color_word);
                            printf("MATCH\n");// match!!
                            match_c = 1; 
                        }
                        
                        if( !match_c ) {
                            status = regcomp(&regex_expr, "(byte$|short$|int$|long$|float$|double$|boolean$|char$|void$)+", REG_EXTENDED);

                            if(status){
                                fprintf(stderr, "Could not compile regex\n");
                                exit(1);
                            }
                            status = regexec(&regex_expr, word, 0, NULL, 0); 
                            if( !status ) {
                                match_check = 1;
                                char * color_word = (char *) malloc(sizeof(char) * 200); 
                                strcpy(color_word, "<span style=\"color: red\">"); 
                                strcat(color_word, word); 
                                strcat(color_word, "</span>"); 
                                strcat(html_code, color_word);
                                free(color_word);
                                printf("MATCH\n");// match!!
                                
                            }
                        }
                        strcpy(before_word, word);
                    }
                }
                if( !match_check) {
                    if(space && name_color) {
                        match_check = 1;

                        status = regcomp(&regex_expr, "(^new)*( )+[a-zA-Z]+( )+(.)+( )*\\(.*\\)( )*{?[?!;]$", REG_EXTENDED);

                        if(status){
                            fprintf(stderr, "Could not compile regex\n");
                            exit(1);
                        }
                        status = regexec(&regex_expr, buf, 0, NULL, 0); 
                        if( !status ) {
                            char * color_word = (char *) malloc(sizeof(char) * 200); 
                            strcpy(color_word, "<span style=\"color:blue\">"); 
                            strcat(color_word, word); 
                            strcat(color_word, "</span>"); 
                            strcat(html_code, color_word);
                            free(color_word);
                            space = 0 ;
                            name_color = 0 ;
                        }else{
                            status = regcomp(&regex_expr, ".+\\(.*\\)( )*{+", REG_EXTENDED);
                            if(status){
                                fprintf(stderr, "Could not compile regex\n");
                                exit(1);
                            }
                            status = regexec(&regex_expr, buf, 0, NULL, 0); 
                            if( !status ) {
                                char * color_word = (char *) malloc(sizeof(char) * 200); 
                                strcpy(color_word, "<span style=\"color:blue\">"); 
                                strcat(color_word, word); 
                                strcat(color_word, "</span>"); 
                                strcat(html_code, color_word);
                                free(color_word);
                                space = 0 ;
                                name_color = 0 ;
                            }else{
                                strcat(html_code, word);
                            }
                            
                        }
                    }else{
                        strcat(html_code, word);
                    }
                }
            }  
            end++; 
            start = end ;
            free(word);
        }
        if( import_finish ) continue; 

        strcat(html_code, "</pre>"); 
        printf("%d: %s\n",k, html_code);
        k++;
        fprintf(f, "%s", html_code);
        free(html_code);
    }
    fprintf(fp, "</body>\n</html>");
    fclose(fp) ;
    fclose(f) ;
    return 0;
}