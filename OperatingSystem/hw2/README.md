PCTest: Multi-processed Programming Assignment Testers   
###### Write-up: (Operating System Section 1) 21900195 Jinil Kim  
====================   
   
## Introduction   
This program named PCTest that verifies the target source file outputs the correct results.  
It shows a variety of functions as a result, such as whether a particular code outputs the correct results, or if the program runs in time.  
This was implemented in C language, and the target source file and solution source file can be processed simultaneously when each test case is executed.     
     
<strong>(Then, there is no subdirectory inside the test directory containing test input files, and only input files exist.)</strong> 

In addition, this program also checks for errors.     
For example, if a program result is not executed within a specified time frame, a it checks for errors based on the results, for example, such as the timeout, runtime error etc..   

#### Program operating principles  
First of all, when you receive the target source file and the solution source file, you create each child process and execute the gcc command to create the executable. 
Then, use the child process to enter the standard input from the test input file in the executable file.   
Afterwards, the standard output is delivered to the parent process through the pipe and the results are compared.   
Then, if the running time of the target program exceeds appointed time, the program prints "TIME OVER".  
Also, if there is an error in the target program, the program prints "ERROR!", and if the target source code results and the solution's source code results are different, the program prints that "the text printed to the standard output is not identical to that of solution.".   
    
#### Command-line interface

```   
$ pctest -i <testdir> -t <timeout> <solution> <target> 
```   
          
<img src="overall.png"/>  
    
## Checking Test Results 

Each test input file has a child process for target and a child process for solution.    
Therefore, the result value obtained from each process is compared.    
Additionally, the failure detector is used to check if there is an error in the program.   
   
## Executing Method     
      
There is a Makefile for executing the program. Thus, if you try to execute the program, you should enter the "make" command in the command line.  
Then, you can confirm the executable file named "pctest".  
    
```    
$ ./pctest -i <testdir> -t <timeout> <solution> <target>    
```     
    
If you enter like above command, the program is executed. 
After executing the program, you can see the test result as standard output.  


