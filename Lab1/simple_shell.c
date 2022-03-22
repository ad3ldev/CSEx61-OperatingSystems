#include <signal.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

// void parent_main(){
//     register_child_signal(on_child_exit());
//     setup_environment();
//     shell();
// }

// int on_child_exit(){
//     reap_child_zombie();
//     write_to_log_file("Child terminated");
// }

// void setup_environment(){
//     cd(Current_Working_Directory);
// }

// void shell(){
//     do
//     {
//         parse_input(read_input());
//         evaluate_expression();
//         switch (input_type)
//         {
//         case shell_builtin:
//             execute_shell_bultin();
//             break;
//         case executable_or_error:
//             execute_command();
//             break;
//         default:
//             break;
//         }
//     } while (command_is_not_exit));
    
// }

// void execute_shell_bultin(){
//     switch (command_type)
//     {
//     case cd:
//         /* code */
//         break;
//     case echo:
//         /* code */
//         break;
//     case export:
//         /* code */
//         break;
//     default:
//         break;
//     }
// }

// void execute_command(){
//     child_id = fork();
//     if child{
//         execvp(command_parsed);
//         print("Error");
//         exit();
//     }else if parent and foreground{
//         waitpid(child);
//     }
// }

int main(int argc, char const *argv[])
{
    pid_t pid;
	pid = fork();
	printf("%d\n", pid);
	pid = fork();
	printf("%d\n", pid);
	return 0;
}
