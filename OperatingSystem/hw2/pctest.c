#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h> 
#include <dirent.h> 
#include <string.h> 
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h> 
#include <signal.h> 
#define  BUFF_SIZE 1024
int pipes[2];
// Time 
struct timespec begin, end;  
double max_time = 0 , min_time = 1e8 ; 
double avg_time = 0; int test_count = 0 ;  
int correct = 1; 
char * make_a_execute_file_name(char * src_file) { 
	char * executeFile = NULL; 
	
	for(int i = strlen(src_file) - 1 ; i >= 0; i--) { 
		if( src_file[i] == '.') {
			executeFile = (char*)malloc(sizeof(char) * (i)); 
		        strncpy(executeFile, src_file, i); 
			executeFile[i] = '\0';
			return executeFile; 
		}
	}
}

void failure_detector(int sig){
	
	switch(sig) {
	case SIGALRM:
                printf("TIME OVER!\n");
                exit(1);
        case SIGSEGV:
                printf("RUNTIME ERROR\n"); // Invalid Memory reference 
                exit(1);
        case SIGFPE:
                printf("RUNTIME ERROR\n");
                exit(1);
        case SIGABRT:
                printf("ERROR!\n");
                exit(1);
	}
}

char * exe_file_stdin(char *buffer, char * path_file, char * execute_file, int timer) { 
	pid_t child_pid; 
	int exit_code ;

	if( pipe(pipes) ){ 
		perror("Error!"); 
		exit(1); 
	}
	
	// SIGNAL
	struct itimerval t ; 
	signal(SIGALRM, failure_detector); 
	t.it_value.tv_sec = timer;
        t.it_value.tv_usec = 0 ; 	
	t.it_interval = t.it_value; 
	setitimer(ITIMER_REAL, &t, 0x0); 
	
	// Runtime Error 
	if(signal(SIGSEGV, failure_detector) == SIG_ERR) {
                perror("CRASH!\n");
                exit(1);
        }
        signal(SIGFPE, failure_detector);
        signal(SIGABRT, failure_detector);	
	
	child_pid = fork(); 
	if(child_pid == 0) { 
		char * tmp = make_a_execute_file_name(execute_file);
		char *stdin_exe = (char*)malloc(sizeof(char) * (strlen(tmp) + 3)); 	
		stdin_exe[0] = '.'; stdin_exe[1] = '/'; stdin_exe[2] = '\0'; 
		strcat(stdin_exe, tmp); 	
				
		pipes[0] = open(path_file, O_RDONLY); 
		dup2(pipes[0], 0) ; 
		close(pipes[0]) ; 
		dup2(pipes[1],1) ; 
		close(pipes[1]); 

		execl(stdin_exe, stdin_exe, NULL); 
	}else{  
		wait(&exit_code);
	       	
		// ERROR CHECK 
		if (exit_code != 0 ){ 
			// 8: When a specific value is divided by zero.
			// 11: Invalid index access or Segmentation fault 
			if (exit_code == 8 || exit_code == 11) {
				printf("RUNTIME ERROR!\n") ;
				exit(1);  
			}else if(exit_code == 256){ 
				printf("COMPILE ERROR!\n"); 
				exit(1); 
			}else{ 
				printf("ERROR!\n"); 
				exit(1); 
			}
		}	
		// printf("EXIT_CODE: %d\n", exit_code);
		char * buf = 0x0; 
		ssize_t s; 
		size_t len = 0 ; 
		
		buffer = (char*)malloc(sizeof(char) * BUFF_SIZE); 
		
		if((s = read(pipes[0], buffer, BUFF_SIZE)) != -1) {
		 	buffer[s + 1] = 0x0; 		
			//printf("> %s \n", buffer); 
			return buffer; 
		}
		
	}
	wait(&exit_code);	
}

int result_detector(char * test_file, char * target_result, char *  solution_result){
	int result_checker = 0 ;

	if ( strcmp(target_result, solution_result) == 0 ) {  
		printf("TEST[%s]: PASS\n", test_file);   	
	}else{
		correct = 0 ;
		printf("TEST[%s]: FAIL\n", test_file); 
	}	
	
}

void make_a_execute_files(char * target, char * solution) {
	pid_t target_child; 
	int exit_code; 

	target_child = fork(); 
	if (target_child == 0 ) { 
		execl("/usr/bin/gcc", "gcc", "-o", make_a_execute_file_name(target), target, (char*)NULL); 
	}else if(target_child > 0) { 
		wait(0); 
	}
	wait(&exit_code);
	if (target_child == 0 ) kill(getpid(), SIGKILL); 

	pid_t solution_child; 
	solution_child = fork() ;
	if( solution_child == 0 ){ 
		execl("/usr/bin/gcc", "gcc", "-o", make_a_execute_file_name(solution), solution, (char*)NULL); 
	}else if(target_child > 0) {
		wait(0) ;	 
	}
	wait(&exit_code); 
	if (solution_child == 0 ) kill(getpid(), SIGKILL); 	
}

int main(int argc, char* argv[]) {
	// Directory  
	DIR *dp; 	
	struct dirent *ep ; 
	char * testdir = NULL ; 
	int dir_access = 0 ;
	
	// Option 
	int c, para = 0 ;
	char * solution = NULL;
        char * target = NULL;

	// FILE 
	int fd, length = 0; 
	int time = 0 ;

	// Result 
	char * target_buffer = NULL;
	char * solution_buffer = NULL;

	// Option parser 
	while((c = getopt(argc, argv, "i:t:")) != -1) {
		switch( c ) { 
			case 'i':
			        testdir = optarg; 	
				break; 
			case 't': 
			        time = atoi(optarg);	
				if( time < 1 || time > 10) { 
					printf("You should enter the time between 1 and 10.\n");
				       	exit(1); 	
				}	
				break ;
			case '?': 
				if (optopt == 'i' ) {
					fprintf(stderr, "Option -%c requires an argument.\n",optopt);  
				}else if( optopt == 't') { 
					fprintf(stderr, "Option -%c requires an argument.\n",optopt);
				}else if (isprint (optopt)) {
          				fprintf (stderr, "Unknown option `-%c'.\n", optopt);  
				}else{	
					fprintf (stderr,"Unknown option character `\\x%x'.\n", optopt); 
				}
				exit(1);
				break;
			default:
				abort(); 
		}
	}

	// Remainder parameters (No-Option parameters)   
	if (argc <= 7) { 	
		solution = (char*)malloc(sizeof(char) * strlen(argv[optind])); 
	       	strcpy(solution, argv[optind]); 
		target = (char*)malloc(sizeof(char) * strlen(argv[optind + 1])); 	
		strcpy(target, argv[optind + 1]); 
	}else{ 
		fprintf(stderr, "FORM: pctest -i <testdir> -t <timeout> <solution> <target>\n"); 
		exit(1);
	}

	// Make a execute_files (target, solution)
	make_a_execute_files(target, solution); 

	if (dir_access == 0) { 
		char * path_file = NULL; 
		dp = opendir(testdir); 
		if (dp != NULL ){ 
			while (ep = readdir(dp)) {
			        if (ep->d_name[0] == '.') continue;	
				path_file = (char*)malloc(sizeof(char) * 100) ; 
				strcpy(path_file, testdir) ; 
				strcat(path_file, "/"); 
				strcat(path_file, ep->d_name); 
				// ep->d_name: directory name 
				if ((fd = open(path_file, O_RDONLY)) < 0) {
					perror("open failed\n");
					exit(1);
				}	
				// TIME START
				clock_gettime(CLOCK_MONOTONIC, &begin);

				// Get a Target Result 
				target_buffer = exe_file_stdin(target_buffer, path_file, target, time );
				
				if( target_buffer != NULL) { 
					clock_gettime(CLOCK_MONOTONIC, &end);
					// TIME END 
					unsigned long long time = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec);
					double TIME = (double)time/1000000000; 
							
					if( max_time < TIME ) { 
						max_time = TIME; 
					}		
					if( min_time > TIME ) { 
						min_time = TIME; 
					}	
					// AVERAGE TIME 	
					avg_time += TIME;
	       				test_count++; ;
				}	
							
				solution_buffer = exe_file_stdin(solution_buffer, path_file, solution, time);	
				free(path_file); 
				
				// Get a Solution Result
				// Result Comparsion (Failure detector)  
				result_detector(ep->d_name, target_buffer, solution_buffer);	
			}
			(void) closedir(dp); 

			dir_access = 1; 
		}else{ 
			printf("Couldn't open the directory[%s]", testdir);
			exit(1); 
		}
	}

	// TEST RESULT SUMMARY 
	if( dir_access == 1 ) {
		printf("(TEST) Program Maximum Run Time: %lf(ms)\n", max_time); 
		printf("(TEST) Program Minimum Run Time: %lf(ms)\n", min_time); 
		printf("(TEST) Program Average Run Time: %lf(ms)\n", (double)avg_time/test_count); 
	}
	if ( correct == 0 ) {
	        printf("Incorrect...\n");	
                printf("The text printed to the standard output is not identical to that of solution.\n"); 
        }else{
		printf("Correct!!!\n"); 
		printf("The target source file is the same as the result of the solution source file.\n");
	}
	free(target); 
	free(solution);  
	return 0; 
}
