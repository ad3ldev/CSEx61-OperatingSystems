//
//  main.c
//  simple_shell
//
//  Created by AbdelRahman AbdelFattah on 21/03/2022.
//

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char input[1024];
char * list[256];
int running = 1;

int evaluate_expression(){
	int size = 0;
	for (int i=0; i<sizeof(list);i++){
		if(list[i]==NULL){
			break;
		}else{
			size++;
		}
	}
	char * arguments [size];
	list[size-1] = strtok(list[size-1], "\n");
	return size;
}

void execute_shell_bultin(char * command, int size){
	char* argument_list[size+1];
	for(int i = 0;i<=size;i++){
		argument_list[i]=list[i];
		list[i]=NULL;
	}
	if(strcmp(argument_list[0], "cd")==0){
	}else if (strcmp(argument_list[0], "export")==0){
	}else if(strcmp(argument_list[0], "echo")==0){
	}
}

void execute_command(char * command, int size){
	int status;
	int background=0;
	if(strcmp(list[size-1], "&")==0){
		background = 1;
		list[size-1]=NULL;
	}
	pid_t child_id = fork();
	if (child_id == 0){
		char* argument_list[size+1];
		for(int i = 0;i<=size;i++){
			argument_list[i]=list[i];
			list[i]=NULL;
		}
		execvp(argument_list[0], argument_list);
		printf("Invalid Command\n");
	}
	else{
		if(!background){
			waitpid(child_id, &status, 0);
		}
	}
}

char *parse_input ( char *input, char *delimit, char *openblock, char *closeblock) {
	static char *token = NULL;
	char *lead = NULL;
	char *block = NULL;
	int iBlock = 0;
	int iBlockIndex = 0;
	if ( input != NULL) {
		token = input;
		lead = input;
	}
	else {
		lead = token;
		if ( *token == '\0') {
			lead = NULL;
		}
	}
	while ( *token != '\0') {
		if ( iBlock) {
			if ( closeblock[iBlockIndex] == *token) {
				iBlock = 0;
			}
			token++;
			continue;
		}
		if ( ( block = strchr ( openblock, *token)) != NULL) {
			iBlock = 1;
			iBlockIndex = block - openblock;
			token++;
			continue;
		}
		if ( strchr ( delimit, *token) != NULL) {
			*token = '\0';
			token++;
			break;
		}
		token++;
	}
	return lead;
}

int checkcommand(char * command){
	if(command==NULL){
		return -1;
	}
	if(strcmp(command,"exit")==0){
		running = 0;
		return 2;
	}
	if(strcmp(command, "cd")==0 || strcmp(command, "echo")==0 || strcmp(command, "export")==0 ){
		return 1;
	}else{
		return 0;
	}
}

void shell(){
	char *tok;
	char acOpen[]  = {"\""};
	char acClose[] = {"\""};
	do
	{
		fgets(input, sizeof(input), stdin);
		tok = parse_input( input, " ", acOpen, acClose);
		int i = 0;
		list[i] = tok;
		i++;
		while ( ( tok = parse_input ( NULL, " ", acOpen, acClose)) != NULL) {
			list[i] = tok;
			i++;
		}
		int command_size = evaluate_expression();
		switch (checkcommand(list[0])) {
			case 1:
				execute_shell_bultin(list[0], command_size);
				break;
			case 0:
				execute_command(list[0], command_size);
				break;
			default:
				break;
		}
	} while (running);
}
void on_child_exit(){
//     reap_child_zombie();
//     write_to_log_file("Child terminated");
}
void setup_environment(){
	long size;
	char *buf;
	char *ptr;
	size = pathconf(".", _PC_PATH_MAX);
	if ((buf = (char *)malloc((size_t)size)) != NULL){
		ptr = getcwd(buf, (size_t)size);
		chdir(ptr);
	}
}



void parent_main(){
	signal (SIGCHLD, on_child_exit);
//     register_child_signal(on_child_exit());
	setup_environment();
	shell();
}

int main(int argc, char const *argv[]){
	parent_main();
	return 0;
}
