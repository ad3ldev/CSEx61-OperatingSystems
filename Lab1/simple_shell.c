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

void evaluate_expression(){
	
}

void execute_shell_bultin(){
	printf("hey\n");
}

void execute_command(){
	int status;
	pid_t child_id = fork();
	if (child_id == 0){
		printf("Hello from Child!\n");
		//		execvp(command_parsed);
		//		print("Error");
		exit(child_id);
	}
	//	}else if parent and foreground{
	//		waitpid(child);
	//	}
	else{
		waitpid(child_id, &status, 0);
		printf("Hello from Parent!\n");
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
	if(strcmp(command, "cd")==0 || strcmp(command, "cd\n")==0){
		return 1;
	}else if(strcmp(command, "export")==0 || strcmp(command, "export\n")==0){
		return 1;
	}else if(strcmp(command, "echo")==0 || strcmp(command, "echo\n")==0){
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
		evaluate_expression();
		if (checkcommand(list[0])){
			execute_shell_bultin();
		}else{
			execute_command();
		}
	} while (strcmp(input, "exit"));
}
 void on_child_exit(){
//     reap_child_zombie();
//     write_to_log_file("Child terminated");
 }
// void setup_environment(){
//     cd(Current_Working_Directory);
// }



void parent_main(){
	signal (SIGCHLD, on_child_exit);
//     register_child_signal(on_child_exit());
//     setup_environment();
	shell();
}

int main(int argc, char const *argv[]){
	parent_main();
	return 0;
}
