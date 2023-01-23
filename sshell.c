#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define CMDLINE_MAX 512

int main(void)
{       
    char cmd[CMDLINE_MAX];

    while (1) {
        char *nl;
        //int retval;

        /* Print prompt */
        // Added '@ucd'
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

        /* Builtin command */
        if (!strcmp(cmd, "exit")) {
            fprintf(stderr, "Bye...\n");
            break;
        }
        
        // Use execvp()
        /* Regular command */
        pid_t pid;
        pid = fork();

        if (pid == 0) {
            char *cmd_args[] = {cmd, NULL};
            execvp(cmd, cmd_args);
            perror("execv");
            exit(1);
        }
        else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
            // TODO: Remove the print statement below.
            printf("Child returned %d\n", WEXITSTATUS(status));
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
