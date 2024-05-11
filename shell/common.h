#ifndef COMMON_JOBS_H
#define COMMON_JOBS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <pwd.h>
#include <glob.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <stdbool.h>
#include <ctype.h>


#define TOKEN_DELIMITERS "&%<>"
#define COMMAND_DELIMITERS "\n\0;|"
#define MAXLEN 100
#define COMMAND_BUFSIZE 1024

#define RUN "Running"
#define SUSP "Suspended"
#define CONT "Continued"
#define FIN "Done"
#define TER "Terminated"

#define SUCCESS 0
#define FAIL -1

typedef enum status {
    RUNNING,
    SUSPENDED,
    CONTINUED,
    FINISHED,
    TERMINATED
} STATUS;

// A structure for a [job] node in the doubly linked list.
typedef struct job{
    char **command;
    pid_t pid;
    int job_id;
    bool in_background;
    STATUS status;
    int len;
    struct job *next;
    struct job* prev;
} JOB;

int total_jobs; 
pid_t shell_pid;

char * line;
char ***commands;

JOB *head ;
JOB *foreground_job;

sigset_t blocked;

char *read_line();
char ***parse_command(char *line, int *num_commands);

void exiting();
int kill_job(char** command);
int fg(char **command);
int bg(char **command);
int ls();
int chmod();
int mkdir();
int rmdir();
int cd();
int pwd();
int cat();
int more();
int rm();
int mount();
int unmount();

void display_jobs_list(JOB* head);
void display_job_node(JOB* node);
void insert_at_end(JOB** head, JOB* new_node);
JOB* remove_job_by_pid(JOB** head, pid_t pid);
JOB* remove_job_by_id(JOB** head, pid_t pid);
JOB* remove_last(JOB** head);
JOB* get_job(JOB* head, pid_t pid);
JOB* get_job_by_id(JOB* head, int job_id);
void free_list(JOB** head);
JOB* get_last_SUSPENDED(JOB* head);
void free_node(JOB* job);


#endif