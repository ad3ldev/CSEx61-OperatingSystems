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

void execute_shell_bultin(){
	printf("hey");
}

void execute_command(){
//	child_id = fork();
//	if child{
//		execvp(command_parsed);
//		print("Error");
//		exit();
//	}else if parent and foreground{
//		waitpid(child);
//	}
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
	if(strcmp(command, "cd") || strcmp(command, "export") || strcmp(command, "echo") ){
		return 1;
	}
	return 0;
}

void shell(){
	char *tok;
	char acOpen[]  = {"\""};
	char acClose[] = {"\""};
	do
	{
		fgets(cmd, sizeof(cmd), stdin);
		tok = parse_input( cmd, " ", acOpen, acClose);
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
// int on_child_exit(){
//     reap_child_zombie();
//     write_to_log_file("Child terminated");
// }
// void setup_environment(){
//     cd(Current_Working_Directory);
// }



void parent_main(){
//     register_child_signal(on_child_exit());
//     setup_environment();
    shell();
}

int main(int argc, char const *argv[]){
	parent_main();
	return 0;
}
