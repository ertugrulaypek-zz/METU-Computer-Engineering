#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
void mapper(int argc,char* argv[]){
	int N= atoi(argv[1]);
	char* mapper_proc = argv[2];
	char* cwd = malloc(sizeof(char)*1000);
	cwd =getcwd(cwd, 1000);
	int temp = strlen(cwd);
	cwd[temp]='/';
	cwd[temp+1] = '\0';
	strcat(cwd,mapper_proc);

	char* line=NULL;
	
	int current_line_len=0;
	char current_char;
	
	int fd[N][2];
	int child_status;
	pid_t pid;
	int current_line_number=0;
	int i=0;
	
	
  	while(1){
  		current_char=getchar();
		
  		if(current_char==EOF){
  			
  			for(;i<N;i++){
  				close(fd[i][1]);
  				wait(NULL);
			  }
			exit(0);
		  }
		  
  		if(current_char == '\n'){
  			current_line_number++;
  			line = realloc(line,sizeof(char)*(current_line_len+2));
  			line[current_line_len]='\n';
  			line[current_line_len +1]='\0';

  			
  			//things goes here:
  			if(current_line_number-1<N){
  				
  				pipe(fd[current_line_number-1]);
  				
  				if((pid=fork())!=0){//parent:
  					close(fd[current_line_number-1][0]);
  					write(fd[current_line_number-1][1],line,strlen(line));
  					
				  }
				else{//child
					char buf[128];
					for(int i=0;i<current_line_number;i++){
						close(fd[i][1]);
					}
					close(fd[current_line_number-1][1]);
					dup2(fd[current_line_number-1][0],0);
					
					close(fd[current_line_number-1][0]);
					
					int temp_current_mapper = (current_line_number-1)%N;
					char current_mapper[50];
					sprintf(current_mapper,"%d",temp_current_mapper);
					execl(cwd,mapper_proc,current_mapper,(char*)0);
					
					exit(0);
				}
				
  				
			  }
			else{
				
				write(fd[(current_line_number-1)%N][1],line,strlen(line));

				
			}
  			
  			
  			free(line);
  			line=NULL;
  			current_line_len=0;
		  }
		  
		else{
			line = realloc(line,sizeof(char)*(current_line_len+1));
			line[current_line_len] = current_char;
			current_line_len++;
		}
	  } 
	  
}

void mapper_reducer(int argc,char* argv[]){
	int N= atoi(argv[1]);
	char* mapper_proc = argv[2];
	char* reducer_proc = argv[3];
	char* cwd = malloc(sizeof(char)*500);
	char* cwd_reducer =malloc(sizeof(char)*500);
	cwd =getcwd(cwd, 500);
	cwd_reducer = getcwd(cwd_reducer, 500);
	int temp = strlen(cwd);
	cwd[temp]='/';
	cwd_reducer[temp]='/';
	cwd_reducer[temp+1]='\0';
	cwd[temp+1] = '\0';
	strcat(cwd,mapper_proc);
	strcat(cwd_reducer, reducer_proc);
	//char* line=NULL;
	char line[1024];
	int current_line_len=0;
	char current_char;
	int flag = 1;
	int fd[N][2];
	int mapper2reducer[N][2];
	int reducer2reducer[N-1][2];
	
	
	int current_line_number=0;
	int i=0;
	
	for(int i=0;i<N;i++){
		pipe(mapper2reducer[i]);
	}
	
	for(int i=0;i<N-1;i++){
		pipe(reducer2reducer[i]);
	}
	
  	while(1){
  		current_char=getchar();
		
  		if(current_char==EOF){
  			for(int i = 0;i<N;i++){
  				close(fd[i][1]);
  				wait(NULL);
			  }
			  
			//free(line);
			//line = NULL;
			free(cwd);
			free(cwd_reducer);
			exit(0);
		  }
		  
  		if(current_char == '\n'){
  			current_line_number++;
  			//line = realloc(line,sizeof(char)*(current_line_len+2));
  			line[current_line_len]='\n';
  			line[current_line_len +1]='\0';
  			
			int temp_current_mapper = (current_line_number-1)%N;
			char current_mapper[50];
			sprintf(current_mapper,"%d",temp_current_mapper);
			
  			if(current_line_number-1<N){
				
  				pipe(fd[current_line_number-1]);
  				
  				if((fork())!=0){//parent:
  					close(fd[current_line_number-1][0]);
  					
  					write(fd[current_line_number-1][1],line,strlen(line));
  					
				  }
				else{//child
					//free(line);
					for(int i=0;i<current_line_number-1;i++){
						close(fd[i][1]);
						//close(fd[i][0]);
					}
					for(int i=0;i<N;i++){
						if(!(i==current_line_number-1)){
							close(mapper2reducer[i][1]);
							close(mapper2reducer[i][0]);
							if(i != N - 1 && i != current_line_number - 2){
								close(reducer2reducer[i][1]);
								close(reducer2reducer[i][0]);
							}
						}
					}
					if((fork())!=0){ // reducer parent:
						//free(line);
						close(fd[current_line_number-1][1]);
						dup2(fd[current_line_number-1][0],0);
					
						close(fd[current_line_number-1][0]);
							
						if(current_line_number - 1 != N - 1){	
							close(reducer2reducer[current_line_number-1][0]);
							close(reducer2reducer[current_line_number-1][1]);
						}
						if(current_line_number - 1 != 0){
							close(reducer2reducer[current_line_number-2][0]);
							close(reducer2reducer[current_line_number-2][1]);
						}
						
						close(mapper2reducer[current_line_number-1][0]);
						dup2(mapper2reducer[current_line_number-1][1],1);
						close(mapper2reducer[current_line_number-1][1]);
					}
					else{ // reducer child:
						//free(line);
						close(fd[current_line_number-1][1]);
						close(fd[current_line_number-1][0]);
						
						
						if(current_line_number-1==0){
							close(reducer2reducer[0][0]);
							dup2(reducer2reducer[0][1], 1);
							close(reducer2reducer[0][1]);
						}
						else if(current_line_number-1==N-1){
							close(reducer2reducer[N-2][1]);
							dup2(reducer2reducer[N-2][0],2);
							close(reducer2reducer[N-2][0]);
						}
						else{
							close(reducer2reducer[current_line_number - 2][1]);
							dup2(reducer2reducer[current_line_number - 2][0], 2);
							close(reducer2reducer[current_line_number - 2][0]);
							
							close(reducer2reducer[current_line_number - 1][0]);
							dup2(reducer2reducer[current_line_number - 1][1], 1);
							close(reducer2reducer[current_line_number - 1][1]);
						}
						
						close(mapper2reducer[current_line_number-1][1]);
						dup2(mapper2reducer[current_line_number-1][0],0);
						
						close(mapper2reducer[current_line_number-1][0]);
						
						int a =execl(cwd_reducer,reducer_proc,current_mapper,(char*)0);
						printf("%d\n",a);
						exit(0);
					}
					
					execl(cwd,mapper_proc,current_mapper,(char*)0);
					
					exit(0);
				}
				
  				
			  }
			else{
				if(flag)
				{
					for(int i=0;i<N;i++){
							close(mapper2reducer[i][1]);
							close(mapper2reducer[i][0]);
							if(i != N - 1){
								close(reducer2reducer[i][1]);
								close(reducer2reducer[i][0]);
							}
					}
					flag = 0;
				}
				
				write(fd[(current_line_number-1)%N][1],line,strlen(line));
				
  					
				
			}
  			
  			
  			//free(line);
  			//line=NULL;
  			current_line_len=0;
		  }
		  
		else{
			//line = realloc(line,sizeof(char)*(current_line_len+1));
			line[current_line_len] = current_char;
			current_line_len++;
		}
	  } 
}

int main(int argc, char *argv[]) 
{ 
	if(argc == 3){
		mapper(argc, argv);
	}
	else if(argc==4){
		//reducer
		mapper_reducer(argc, argv);
	}
	
    return 0; 
} 
