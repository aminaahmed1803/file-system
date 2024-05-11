#include <stdio.h>
#include <stdlib.h>
#include"common.h"

void check_zombie();
// add remove job here 
int set_status(int pid, int status){

    if (foreground_job->pid == pid){
        foreground_job->status = status;
        return SUCCESS;
    }

    JOB *job = remove_job_by_pid(&head, pid);
    if (job != NULL){
        signal(SIGTTOU, SIG_IGN);
        job->pid = status;
        signal(SIGTTOU, SIG_IGN);
    }

    return FAIL;
}

void sigchild_handler(int signal, siginfo_t *info, void *ucontext){
    
    pid_t pid = info->si_pid;

    if(info->si_code == CLD_EXITED){
        check_zombie(pid, FINISHED);
    }
    if(info->si_code == CLD_KILLED || info->si_code == CLD_DUMPED){
        check_zombie(pid, TERMINATED);
    }
    else if (info->si_code == CLD_STOPPED) { // child has stopped
        check_zombie(pid, SUSPENDED);
    }
    else if (info->si_code == CLD_CONTINUED) { // Stopped child has continued
        set_status(pid, CONTINUED);
    }

    return;
}

void sigint_handler(int signal){
    printf("\n");
}

void init() {

    total_jobs = 0;
    head = NULL;
    foreground_job = NULL;
    line = NULL;
    commands = NULL;

    struct sigaction sigint_action = {
        .sa_handler = &sigint_handler,
        .sa_flags = 0};
    sigemptyset(&sigint_action.sa_mask);
    sigaction(SIGINT, &sigint_action, NULL);

    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);

    pid_t pid = getpid();
    setpgid(pid, pid);
    tcsetpgrp(0, pid);

    /*total_jobs = 0;
    head = NULL;
    foreground_job = NULL;
    line = NULL;
    commands = NULL;

    struct sigaction sig_action = {
        .sa_flags = SA_SIGINFO,
        .sa_sigaction = &sigchild_handler
        };
    sigemptyset(&sig_action.sa_mask);
    sigaction(SIGCHLD,&sig_action,NULL);

    tcgetattr(STDIN_FILENO,&main_termios);

    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);

    pid_t shell_pid = getpid();
    setpgid(shell_pid, shell_pid);
    tcsetpgrp(STDIN_FILENO, shell_pid);*/

    
}

int wait_for_pid(int pid){
    int status = 0;

    waitpid(pid, &status, WUNTRACED);
    if (WIFEXITED(status)){
        set_status(pid, FINISHED);
    }
    else if (WIFSIGNALED(status)){
        set_status(pid, TERMINATED);
    }
    else if (WSTOPSIG(status)){
        status = -1;
        set_status(pid, SUSPENDED);
    }
    
    return status;
}

void check_zombie(){
    int status, pid;
    
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0){

        JOB *job = remove_job_by_pid(&head, pid);
            if (job == NULL){
            continue; 
        }

        if (WIFEXITED(status)){
            set_status(pid, FINISHED);
        }
        else if (WIFSTOPPED(status)){
            set_status(pid, SUSPENDED);
            insert_at_end(&head, job);
            continue;
        }
        else if (WIFCONTINUED(status)){
            set_status(pid, CONTINUED);
        }

        display_job_node(job);
        free_node(job);
        job = NULL;
    }
}

int builtin_cmd_handler(char** command){
    if (command == NULL || command[0] == NULL) {
        return FAIL;
    }

    if (strcmp(command[0], "exit") == 0) { 
        exiting();
    }

    if (strcmp(command[0], "jobs") == 0) { 
        display_jobs_list(head);
        return SUCCESS;
    }

    if (strcmp(command[0], "kill") == 0) { 
        kill_job(command);
        return SUCCESS;
    }

    /*if (strcmp(command[0], "fg") == 0) {
        fg(command);
        return SUCCESS;
    }

    if (strcmp(command[0], "bg") == 0) {
        bg(command);
        return SUCCESS;
    }*/

    /*ls lists all the files in the current or 
    specified directory. Support flags -F and -l.*/
    if (strcmp(command[0], "ls") == 0) {
        //TODO
        return SUCCESS;
    }
    /*chmod changes the permissions mode of a file. 
    Support absolute mode and symbolic mode.*/
    if (strcmp(command[0], "chmod") == 0) {
        //TODO
        return SUCCESS;
    }
    /*mkdir creates a directory*/
    if (strcmp(command[0], "mkdir") == 0) {
        //TODO
        return SUCCESS;
    }
    /*rmdir removes a directory*/
    if (strcmp(command[0], "rmdir") == 0) {
        //TODO
        return SUCCESS;
    }
    /*cd changes the current working directory 
    according to the specified path, or to the 
    home directory if no argument is given. 
    Support . and ..  */
    if (strcmp(command[0], "cd") == 0) {
        //TODO
        return SUCCESS;
    }
    //pwd prints the current working directory
    if (strcmp(command[0], "pwd") == 0) {
        //TODO
        return SUCCESS;
    }
    //cat displays the content of one or more files to the output.
    if (strcmp(command[0], "cat") == 0) {
        //TODO
        return SUCCESS;
    }
    //more lists a file a screen at a time
    if (strcmp(command[0], "more") == 0) {
        //TODO
        return SUCCESS;
    }
    //rm deletes a file
    if (strcmp(command[0], "rm") == 0) {
        //TODO
        return SUCCESS;
    }
    /*mount mounts a file system at a specified 
    location.  Note that it should be possible to 
    format multiple disks and mount them at 
    arbitrary locations in your file system */
    if (strcmp(command[0], "mount") == 0) {
        //TODO
        return SUCCESS;
    }
    // umount unmounts a file system
    if (strcmp(command[0], "umount") == 0) {
        //TODO
        return SUCCESS;
    }
    return FAIL;
}

int launch_job(char*** command, int num_commands){
    bool invalid = false;
    char invalid_command[1024] = "";

    //acknowledge_zombie();
    if (num_commands == 0){
        return SUCCESS;
    }

    for (int i = 0; i < num_commands; i++) {

        if (command[i] == NULL){
            invalid = true;
            break;
        }
        
        if ( builtin_cmd_handler(command[i]) == SUCCESS ){
            continue;
        } 
        
        int sz = 0;
        JOB *job = (JOB *)malloc(sizeof(JOB));
        job->in_background = false;
        job->job_id = -1;
        job->pid = -1;
        job->status = RUNNING;

        for (; command[i][sz]!=NULL; sz++){   
            if (strcmp(command[i][sz], "&") == 0){
                if (command[i][sz+1]==NULL){
                    job->in_background = true;
                    free(command[i][sz]);
                    command[i][sz] = NULL;
                }else{
                    return FAIL;
                } 
            }
        }
        sz += 2;
        job->command = (char **)malloc(sz *sizeof(char *));
        job->len = sz; 
        for (int k=0; k<sz; k++){
            job->command[k] = NULL;
        }

        for (int k=0; command[i][k]!=NULL; k++){
            job->command[k] = strdup(command[i][k]);
        }
        
        pid_t childpid = fork();
        if (childpid < 0){
            return FAIL;
        }
        else if (childpid == 0){
            job->pid =  getpid();
            setpgid(0, job->pid );

            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
            signal(SIGCHLD, SIG_DFL);
            signal(SIGTERM, SIG_DFL);

            if (execvp(job->command[0], job->command) < 0){
                printf("%s: command not found\n", job->command[0]);
                exit(EXIT_FAILURE);
            }

            exit(0);
        }
        else{
            if (job->pid <= 0){
                job->pid = childpid;
            }

            setpgid(0, childpid);
            int status = 0;

            if(job->in_background){
                waitpid(childpid ,&status,WNOHANG|WUNTRACED);
                printf("[%d] %d\n", job->job_id, job->pid);
                
                insert_at_end(&head, job);
                job->job_id = ++total_jobs;

            }else{
                foreground_job = job;

                tcsetpgrp(0, childpid);
                int status = wait_for_pid(childpid);
                
                signal(SIGTTOU, SIG_IGN);
                tcsetpgrp(0, getpid());
                signal(SIGTTOU, SIG_DFL);

                if (job->pid <= 0){
                    job->pid = childpid;
                }
        

                if(job->status == RUNNING){
                    printf("error\n");
                    exit(0);
                }
    
                if (job->status == FINISHED || job->status == TERMINATED ){
                    foreground_job = NULL;
                    free_node(job);
                    job = NULL;
                }
                else if (job->status == SUSPENDED){
                //print out statment
                    printf("zsh: suspended  ");
                    for (int i=0; job->command[i]!=NULL; i++){
                        printf("%s ",job->command[i]);
                    }
                    printf("\n");

                    job->job_id = ++total_jobs;
                    insert_at_end(&head, job);
                }
            
            }
        }   

        if (invalid) {
            printf("bash: syntax error near unexpected token '%s'\n", line);
        }
    }
    return SUCCESS;
}

int main(int argc, char **argv){
    init();
    
    bool run = true;
    while (run) {
        printf("[myshell] $ ");
        line = read_line(); 
        int num_commands = 0;
        
        if (line == NULL){
            printf("\n");
            exiting();
        }

        if (strlen(line) < 2 ){
            check_zombie();
            if (line!= NULL ) {
                free(line);
                line = NULL;
            }
            continue;
        }

        commands = parse_command(line, &num_commands);

        if (commands == NULL) {
            free(line); 
            line = NULL;
            continue;
        }

        if (launch_job(commands, num_commands) == FAIL){
            run = false;
        }

        free(line);
        line = NULL;
    
        if(commands != NULL){
            for (int i = 0; commands[i] != NULL ; i++ ){
                for (int j = 0; commands[i][j] != NULL; j++ ){
                    free(commands[i][j]);
                    commands[i][j] = NULL;
                }
            
                free(commands[i]);
                commands[i] = NULL;
            }

            free(commands);
            commands = NULL;
        }

        free(line);
        line = NULL;
    }

    free(line);
    line = NULL;

    free_list(&head);
    head = NULL;
    foreground_job = NULL;
    
    return EXIT_SUCCESS;
}
