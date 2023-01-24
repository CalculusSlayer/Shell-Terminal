#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "token.h"
#include <stdbool.h>
#include <dirent.h>

#define CMDLINE_MAX 512
#define TOKEN_LENGTH_MAX 33

//Referenced slide deck 03(syscalls) for information regarding readdir();

int main(void)
{       
    char cmd[CMDLINE_MAX];
    char current_directory[TOKEN_LENGTH_MAX];
    DIR *dirp;
    struct dirent *dp;

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
        else if (!strcmp(cmd_args[0], "pwd")) {
            getcwd(current_directory, sizeof(current_directory));
            printf("%s\n", current_directory);         
            fprintf(stderr, "+ completed '%s' [%d]\n", cmd, 0);
            continue;
        }
        else if (!strcmp(cmd_args[0], "cd")) {
            //TODO: CHeck to see if provided directory is valid
            
            bool directory_found = false;
            dirp = opendir(".");
            while ((dp = readdir(dirp)) != NULL) 
            {
                if (!strcmp(dp->d_name, cmd_args[1])) {
                    directory_found = true;
                    chdir(cmd_args[1]);
                    break;
                }
            }
            if (!directory_found) {
                fprintf(stderr, "Error: could not change directories.\n");
            }

            fprintf(stderr, "+ completed '%s %s' [%d]\n", cmd_args[0], cmd_args[1], 0);
            continue;
        }


        //TODO:
        //PWD and CD builtin commands


        if (cmd_args == NULL) {
            printf("Error: too many process arguments\n");
            continue;
        }
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
