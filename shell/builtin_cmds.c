#include <stdio.h>
#include <stdlib.h>
#include"common.h"

void exiting() {
    if (head != NULL) {
        free_list(&head);
        head = NULL;
    }
    if (line != NULL){
        free(line);
        line = NULL;
    } 
    
    line = NULL;
    head = NULL;
    foreground_job = NULL;

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

    kill(0, SIGTERM);
    printf("exit...\n");
    printf("Connection to myshell closed.\n");
    exit(0);
}

int kill_job(char** command) {
    
    int start = 1;
    pid_t pid = -1;
    int job_id = -1;
    JOB *job = NULL; 

    if (command[start] == NULL) {
        printf("kill: usage: kill [-s sigspec | -n signum | -sigspec] pid | jobspec ... or kill -l [sigspec]\n");
        return FAIL;
    }

    bool is_sigkill = false;
    if (strcmp(command[start], "-9") == 0) {
        is_sigkill = true;
        start++;
    }

    for (int i = start; command[i] != NULL; i++) {
        
        pid_t pid = atoi(command[i]);
        job = get_job(head, pid);

        if (job == NULL) {
            printf("-bash: kill: (%s) - No such process\n", command[i]);
            continue;
        }
            // Determines which signal to send SIGKILL or SIGTERM
            // If is_sigkill is true, sends SIGKILL
            // If is_sigkill is false, sends SIGTERM
        pid = job->pid;
        int signal;
        if (is_sigkill){
            signal = kill(pid, SIGKILL);
        }else { 
            signal = kill(pid, SIGTERM);
        }
        if (signal < 0) {
               // Error occurred
            printf("Failed to send signal to job (%s)\n", command[i]);
        } else {
               // Success, signal sent
            printf("Sent signal to job (%s)\n", command[i]);
        }
        //wait_for_pid(pid);
    }  
    return SUCCESS;
}

int fg(char **command){
    if (command == NULL){
        return FAIL;
    }
    if (head == NULL){
        printf("bash: fg: current: no such job\n");
        return SUCCESS;
    }

    JOB* job = NULL;
    pid_t pid = -1;
    int job_id = -1;

    if (command[1] == NULL){
        job = get_last_SUSPENDED(head);
        if (job == NULL){
            printf("bash: fg: current: no such job\n");
            return SUCCESS;
        }

        pid = job->pid;
        job_id = job->job_id;
        remove_job_by_pid(&head, pid);
        
    } else if (command[1][0] == '%'){
     
        if (command[2] == NULL){
            printf("mysh: fg %s: no such job\n", command[2]);
            return FAIL;
        }
        job_id = atoi(command[2]);
        JOB* job = remove_job_by_id(&head, job_id);
        if (job == NULL){
            printf("mysh: fg %s: no such job\n", command[2]);
            return FAIL;
        }else {
            pid = job->pid;
        }

    } else {

        pid = atoi(command[1]);
        job = remove_job_by_pid(&head, pid);
       
        if (job == NULL){
            printf("mysh: fg %s: no such job\n", command[1]);
            return FAIL;
        }
        job_id = job->job_id;
        pid = job->pid;
    }
    
    if (kill(-pid, SIGCONT) < 0)
    {
        printf("mysh: fg %d: job not found\n", pid);
        return -1;
    }

    tcsetpgrp(0, pid);

    foreground_job = job;
    job->status = CONTINUED;
    job->in_background=false;

    wait_for_pid(pid);

    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(0, getpid());
    signal(SIGTTOU, SIG_DFL);
    
    foreground_job = NULL;
    
    if (job->status == FINISHED || job->status == TERMINATED ){
        free_node(job);
        job = NULL;
    } else if (job->status == SUSPENDED){
            //print out statment
        printf("zsh: suspended  ");
        for (int i=0; job->command[i]!=NULL; i++){
            printf("%s ",job->command[i]);
        }
        printf("\n");
        insert_at_end(&head, job);
    }

    return 0;
}

int bg(char **command){

    if (command == NULL){
        return FAIL;
    }
    if (head == NULL){
        printf("bash: bg: current: no such job\n");
        return SUCCESS;
    }

    JOB* job = NULL;
    pid_t pid = -1;
    int job_id = -1;

    if (command[1] == NULL){
        job = get_last_SUSPENDED(head);
        if (job == NULL){
            printf("bash: bg: current: no such job\n");
            return SUCCESS;
        }

        pid = job->pid;
        job_id = job->job_id;
        remove_job_by_pid(&head, pid);
        
    } else if (command[1][0] == '%'){
        
        if (command[2] == NULL){
            printf("mysh: bg %s: no such job\n", command[2]);
            return FAIL;
        }
        job_id = atoi(command[2]);
        JOB* job = get_job_by_id(head, job_id);
        if (job == NULL){
            printf("mysh: bg %s: no such job\n", command[1]);
            return FAIL;
        }else {
            pid = job->pid;
        }

    }else {
        pid = atoi(command[1]);
        job = get_job(head, pid);
        if (job == NULL){
            printf("mysh: bg %s: no such job\n", command[1]);
            return FAIL;
        }
        job_id = job->job_id;
    } 
    
    
    if (kill(-pid, SIGCONT) < 0)
    {
        printf("mysh: bg %d: job not found\n", pid);
        return -1;
    }

    job->in_background=true;
    if (job_id > 0){
        set_status(pid, CONTINUED);
        display_job_node(job);
    }

    return SUCCESS;
}


int ls(){
    //TODO
    return FAIL;

}
int chmod(){
    //TODO
    return FAIL;

}
int mkdir(){
    //TODO
    return FAIL;

}
int rmdir(){
    //TODO
    return FAIL;

}
int cd(){
    //TODO
    return FAIL;

}
int pwd(){
    //TODO
    return FAIL;

}
int cat(){
    //TODO
    return FAIL;

}
int more(){
    //TODO
    return FAIL;

}
int rm(){
    //TODO
    return FAIL;

}
int mount(){
    //TODO
    return FAIL;

}
int unmount(){
    //TODO
    return FAIL;
}

