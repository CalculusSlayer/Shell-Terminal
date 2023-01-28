// sshell.c
// The main program that runs the shell program
// i.e. the interface between the user and the terminal.

#include "sshell.h"

//Referenced slide deck 03(syscalls) for information regarding readdir();

bool check_active_jobs(List background_jobs) {
    moveFront(background_jobs);
    bool active_jobs = false;
    for (int i = 0; i < length(background_jobs); i++) {
        // add this: waitpid(child_pid, &(x->exit_status), WNOHANG);
        //printf("exit_status: %i\n", ((BackgroundJobObj*)get(background_jobs))->exit_status);
        // fflush(stdout);
        BackgroundJobObj* x = (BackgroundJobObj*)get(background_jobs);
        if (waitpid(x->pid, &(x->exit_status), WNOHANG)==0) {
            active_jobs = true;
            break;
        }
        moveNext(background_jobs);
    }
    return active_jobs;
}

int main(void)
{       
    /* Child process, run sshell until user exit */
    List background_jobs = newList();
    char cmd[CMDLINE_MAX];
    char current_directory[100];
    DIR *dirp;
    // Continutation of sshell process
    while (1) {
        char *nl; 
        bool full_spaces = true;
        bool background_job_flag = false;
        bool parsing_error_detected = false;
        //int retval;

        /* Print prompt */
        printf("sshell@ucd$ ");
        fflush(stdout);

        /* Get command line */
        fgets(cmd, CMDLINE_MAX, stdin);
        
        
        for (int i = 0; i < CMDLINE_MAX; i++) {
            if (cmd[i] == '\n') break;
            else if (cmd[i] != ' ') {
                full_spaces = false;
                if (background_job_flag) {
                    fprintf(stderr, "Error: mislocated background sign\n");
                    parsing_error_detected = true;

                }
            }
            if (cmd[i] == '&') {
                background_job_flag = true;
            }
        }

        if (full_spaces) continue;

        /* Print command line if stdin is not provided by terminal */
        if (!isatty(STDIN_FILENO)) {
            printf("%s", cmd);
            fflush(stdout);
        }


        /* Remove trailing newline from command line */
        nl = strchr(cmd, '\n');
        if (nl)
            *nl = '\0';
       

        if (background_job_flag) {
            char* ambersand_needle = strstr(cmd, "&");
            size_t ambersand_index = ambersand_needle - cmd;
            cmd[ambersand_index] = '\0';
        }
        //char **cmd_args = splitter(cmd);
        //Process testProcess = split_redirection(cmd);

        StringArray pipe_strings = split_pipes(cmd);
        int *pipe_exit_status;
        Process processes[4] = { NULL, NULL, NULL, NULL };
        for (int i = 0; i < 4; i++) {
            processes[i] = split_redirection(pipe_strings->arr[i]);
        }

        // If parsing error was detected, then command line
        // and go to next iteration.
        if (parsing_error_detected) {
            deallocator(&pipe_strings);
            for (int i=0; i < 4; i++) {
                free_process(&processes[i]);
            }
            continue;
        }
        int num_processes = 1;
        for (int i = 1; i < 4; i++) {
            if (processes[i] == NULL) {
                break;
            }
            num_processes++;
        }
        if (num_processes > 1)
        {
            pipe_exit_status = sshell_system_pipe(processes, num_processes);
            fprintf(stderr, "+ completed '%s' ", cmd);
            for (int i = 0; i < num_processes; i++) {
                printf("[%d]", pipe_exit_status[i]);
            }
            printf("\n");
            break;
        }
        


        /* Builtin command */
        if (!strcmp(processes[0]->program, "exit")) {
            bool c = check_active_jobs(background_jobs);
            if (c) {
                fprintf(stderr, "Error: active jobs still running\n");
                fprintf(stderr, "+ completed '%s' [%d]\n", "exit", 1);
                fflush(stderr);
            }
            else {
                fprintf(stderr, "Bye...\n");
                fprintf(stderr, "+ completed '%s' [%d]\n", "exit", 0);
                fflush(stderr);
            }
            deallocator(&pipe_strings);
            for (int i=0; i < 4; i++) {
                free_process(&processes[i]);
            }
            if (!c)
                break;
            else
                continue;
        }
        //TODO: Debug pwd. It is failing test.sh.
        else if (!strcmp(processes[0]->program, "pwd")) {
            // Maybe add error checking if getcwd() fails.
            getcwd(current_directory, sizeof(current_directory));
            printf("%s\n", current_directory);         
            fprintf(stderr, "+ completed '%s' [%d]\n", "pwd", 0);
            fflush(stderr);
            deallocator(&pipe_strings);
            for (int i=0; i < 4; i++) {
                free_process(&processes[i]);
            }
            continue;
        }
        else if (!strcmp(processes[0]->program, "cd")) {
            int directory_not_found = 1;
            if (getLength(processes[0]->left_args) == 1) {
                dirp = opendir(front(processes[0]->left_args));
                
                if (dirp) {
                    chdir(front(processes[0]->left_args));
                    directory_not_found = 0;
                }
                else {
                    fprintf(stderr, "Error: cannot cd into directory\n");
                    fflush(stderr);
                }
                
                //fprintf(stderr, "+ completed '%s %s' [%d]\n", processes[0]->program, processes[0]->left_args, directory_not_found);
                closedir(dirp);
                //continue;
            
                // TODO: simplify error message here if needed
                fprintf(stderr, "+ completed '%s ", processes[0]->program);
                printLinkedList(stderr, processes[0]->left_args);
                fprintf(stderr, "' [%d]\n", directory_not_found);         
                fflush(stderr);

            }
            deallocator(&pipe_strings);
            for (int i=0; i < 4; i++) {
                free_process(&processes[i]);
            }
            continue;
        }
        else {
            // TODO: HAVE A LOOP TO CALL SSHELL_SYSTEM
            // UP TO 4 TIMES
            // Never Mind - Call sshell_system_pipe() function
            // if pipe count > 0
            /* Regular commands */
            //printf("processes[0] in main file: %lu\n", (unsigned long) processes[0]);
            int ret_val = sshell_system(processes[0]
                    , background_job_flag, background_jobs, cmd);
//            fprintf(stderr, "cmd = %s\n", 
            if (!background_job_flag) {
                if (background_jobs && length(background_jobs)>0) {
                    
                    while (check_active_jobs(background_jobs));
                    moveFront(background_jobs);
                    for (int i = 0; i < length(background_jobs); i++) {
                        fprintf(stderr, "+ completed '%s&' [%d]\n", 
                            ((BackgroundJobObj*)get(background_jobs))->cmd_str,
                            ((BackgroundJobObj*)get(background_jobs))->exit_status); 
                        fflush(stderr);
                        moveNext(background_jobs);
                    }
                    clear(background_jobs);

                }
                fprintf(stderr, "+ completed '%s' [%d]\n", cmd, ret_val); 
                fflush(stderr);
            }
            /*
            if (background_job)
                fprintf(stderr, "+ completed '%s&' [%d]\n", cmd, ret_val); 
            else
                fprintf(stderr, "+ completed '%s' [%d]\n", cmd, ret_val); 
            */

            deallocator(&pipe_strings);
            for (int i=0; i < 4; i++) {
                free_process(&processes[i]);
            }
            /*
            pid_t child_pid;
            child_pid = fork();

            if (child_pid == 0) {
                //char **cmd_args = splitter(cmd);
                //char *cmd_args[] = {cmd, "-l", NULL};
                execvp(cmd_args[0], cmd_args);
                perror("execv");
                deallocator(&cmd_args);
                exit(1);
            }
            else if (child_pid > 0) {
                int child_status;
                waitpid(child_pid, &child_status, 0);
                // Printing to stderr instead of stdout now. Instructions
                // said to print to stderr.
                fprintf(stderr, "+ completed '%s' [%d]\n", cmd,  WEXITSTATUS(child_status));
                deallocator(&cmd_args);
            }
            else {
                perror("fork");
                exit(1);
            }
            */
        }
    }

    return EXIT_SUCCESS;
}
