#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <regex.h>
#include <stdlib.h>

char *filename = "log.txt";
char input[1024];
char * list[256];
char acOpen[]  = {"\""};
char acClose[] = {"\""};
int running = 1;


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
void empty_list(){
	for(int i=0;i<256;i++){
		list[i]=NULL;
	}
}
int evaluate_expression(){
	int size = 0;
	regex_t regex_quote;
	regex_t regex_dollar;
	regcomp(&regex_quote, "\".*\"", 0);
	regcomp(&regex_dollar, "\\$", 0);
	for (int i=0; i<sizeof(list);i++){
		if(list[i]==NULL){
			break;
		}else{
			size++;
			printf("%d\n", size);
		}
	}
	list[size-1] = strtok(list[size-1], "\n");
	for(int i = 0; i<size;i++){
		if(regexec(&regex_quote, list[i], 0, NULL, 0)!=REG_NOMATCH){
			char b_quote[strlen(list[i])];
			strncpy(b_quote, list[i], strlen(list[i]));
			char without[sizeof(b_quote)-2];
			int j = 0;
			int k = 0;
			while(j<sizeof(b_quote)){
				if (b_quote[j]!='"') {
					without[k]=b_quote[j];
					k++;
				}
				j++;
			}
			without[k]='\0';
			strcpy(list[i], without);
		}if(regexec(&regex_dollar, list[i], 0, NULL, 0)!=REG_NOMATCH){
			char dollar[strlen(list[i])];
			strncpy(dollar, list[i], strlen(list[i]));
			char without[sizeof(dollar)];
			for(int j = 0; j<sizeof(without);j++){
				without[j]=dollar[j+1];
			}
			without[sizeof(without)-1]= '\0';
			char * value = getenv(without);
			if(value!=NULL){
				strcpy(list[i], getenv(without));
			}else{
				strcpy(list[i], " ");
			}
		}
	}
	return size;
}
void execute_shell_bultin(char * command, int size){
	char* argument_list[size+1];
	for(int i = 0;i<=size;i++){
		argument_list[i]=list[i];
		list[i]=NULL;
	}
	if(strcmp(argument_list[0], "cd")==0){
		if(argument_list[1]==NULL){
			chdir(getenv("HOME"));
		}else if(strcmp(argument_list[1], "..")==0){
			chdir("..");
		}else if(strcmp(argument_list[1], "~")==0) {
			chdir(getenv("HOME"));
		}else{
			chdir(argument_list[1]);
		}
	}else if (strcmp(argument_list[0], "export")==0){
		regex_t regex_equal;
		regcomp(&regex_equal, "=", 0);
		for(int i=1;i<size;i++){
			if(regexec(&regex_equal, argument_list[i], 0, NULL, 0)!=REG_NOMATCH){
				char equal[strlen(argument_list[i])+1];
				strncpy(equal, argument_list[i], strlen(argument_list[i]));
				equal[strlen(argument_list[i])] = '\0';
				char * token = strtok(equal, "=");
				char * equation[2];
				int j = 0;
				while(token!=NULL){
					equation[j]=token;
					token = strtok(NULL, "=");
					j++;
				}
				setenv(equation[0], equation[1],1);
			}
		}
	}else if(strcmp(argument_list[0], "echo")==0){
		for(int i=1; i<size;i++){
			printf("%s ", argument_list[i]);
		}
		printf("\n");
	}
}
void execute_command(char * command, int size){
	int status;
	int background=0;
	if(strcmp(list[size-1], "&")==0){
		background = 1;
		list[size-1]=NULL;
		size--;
	}
	regex_t regex_space;
	regcomp(&regex_space, " ", 0);
	int tempsize = size;
	for(int i=0;i<size;i++){
		if(regexec(&regex_space, list[i], 0, NULL, 0)!=REG_NOMATCH){
			char space[strlen(list[i])];
			strncpy(space, list[i], strlen(list[i]));
			char * parts[100];
			int j = 0;
			char * token = parse_input( list[i], " ", acOpen, acClose);
			parts[j]=token;
			j++;
			while(( token = parse_input ( NULL, " ", acOpen, acClose)) != NULL){
				parts[j]=token;
				j++;
			}
			for(int k = 0; k<j;k++){
				list[k+i]=parts[k];
			}
			tempsize = tempsize + j;
		}
	}
	size = tempsize;
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
	do
	{
		empty_list();
		printf("-> ");
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
	printf(">>>>>>SHELL TERMINATED\n");
}
void on_child_exit(){
	FILE * fpointer;
	fpointer =fopen(filename, "a");
	fputs("Child terminated\n", fpointer);
	fclose(fpointer);
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
	setup_environment();
	shell();
}

int main(int argc, char const *argv[]){
	printf(">>>>>>SHELL HAS STARTED\n");
	parent_main();
	return 0;
}
