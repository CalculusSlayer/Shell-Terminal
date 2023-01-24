#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "token.h"
#include <stdbool.h>
#include <dirent.h>

#define CMDLINE_MAX 512
#define TOKEN_LENGTH_MAX 32 // 32 + 1 to make space for NULL character

//Referenced slide deck 03(syscalls) for information regarding readdir();

int main(void)
{       
    char cmd[CMDLINE_MAX];
    char current_directory[100];
    DIR *dirp;
    //struct dirent *dp;

    while (1) {
        char *nl; 

        //int retval;

        /* Print prompt */
        printf("sshell@ucd$ ");
        fflush(stdout);

        /* Get command line */
        fgets(cmd, CMDLINE_MAX, stdin);

        /* Print command line if stdin is not provided by terminal */
        if (!isatty(STDIN_FILENO)) {
            printf("%s", cmd);
            fflush(stdout);
        }


        /* Remove trailing newline from command line */
        nl = strchr(cmd, '\n');
        if (nl)
            *nl = '\0';
        

        char **cmd_args = splitter(cmd);

        /* Builtin command */
        if (!strcmp(cmd_args[0], "exit")) {
            fprintf(stderr, "Bye...\n");
            fprintf(stderr, "+ completed '%s' [%d]\n", "exit", 0);
            break;
        }
        //TODO: Debug pwd. It is failing test.sh.
        else if (!strcmp(cmd_args[0], "pwd")) {
            // Maybe add error checking if getcwd() fails.
            getcwd(current_directory, sizeof(current_directory));
            printf("%s\n", current_directory);         
            fprintf(stderr, "+ completed '%s' [%d]\n", cmd, 0);
            continue;
        }
        else if (!strcmp(cmd_args[0], "cd")) {
            //TODO: CHeck to see if provided directory is valid
            
            int directory_not_found = 1;
            dirp = opendir(cmd_args[1]);
            
            /*
            dirp = opendir(".");
            while ((dp = readdir(dirp)) != NULL) 
            {
                if (!strcmp(dp->d_name, cmd_args[1])) {
                    directory_found = true;
                    chdir(cmd_args[1]);
                    break;
                }
            }
            */
            // Directory was successfully found
            if (dirp) {
                chdir(cmd_args[1]);
                directory_not_found = 0;
            }
            else
                fprintf(stderr, "Error: could not change directories.\n");


            fprintf(stderr, "+ completed '%s %s' [%d]\n", cmd_args[0], cmd_args[1], directory_not_found);
            closedir(dirp);
            continue;
        }


        //TODO:
        //PWD and CD builtin commands

        //TODO: Check if this is the correct error message.
        //Doesn't this mean that there were no cmd_args?
        /*
        if (cmd_args == NULL) {
            printf("Error: too many process arguments\n");
            continue;
        }
        */

        // Use execvp()
        /* Regular command */
        pid_t pid;
        pid = fork();


        if (pid == 0) {
            //char **cmd_args = splitter(cmd);
            //char *cmd_args[] = {cmd, "-l", NULL};
            execvp(cmd_args[0], cmd_args);
            perror("execv");
            deallocator(&cmd_args);
            exit(1);
        }
        else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
            // Printing to stderr instead of stdout now. Instructions
            // said to print to stderr.
            fprintf(stderr, "+ completed '%s' [%d]\n", cmd,  WEXITSTATUS(status));
            deallocator(&cmd_args);
        }
        else {
            perror("fork");
            exit(1);
        }

        //printf("The process ID is: %ld\n", (long) pid);
        //
        /*
        retval = system(cmd);
        fprintf(stdout, "Return status value for '%s': %d\n",
            cmd, retval);
        */
    }

    return EXIT_SUCCESS;
}
