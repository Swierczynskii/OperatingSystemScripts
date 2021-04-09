#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_CHILD 4

void child_proccess(){
    /* Child Process algorithm */

    printf("Child[%d] of parent[%d]\n", getpid(), getppid());
    sleep(10);
    printf("Execution completed for child[%d].\n", getpid());
}

void child_killer(int i, pid_t pids[]){
    /* Child processes killing function */

    printf("Fork failed\n");

    for (int j = 0; j < i; j++){
    
        printf("Child[%d] killed", j);
        kill(pids[j], SIGTERM);
    
    }
}

int main(void)
{
    int i;
    pid_t pid;
    pid_t children_pids[NUM_CHILD];
    int child_num = 0;// in case that child_killer is used
    printf("\nINITIAL PARENT PID: %d\n\n", getpid());

    /*Main process algorithm*/
    for(i = 0; i < NUM_CHILD; i++){
        
        switch(pid = fork()){

            case 0:
                child_proccess();
                return 0;
            case -1:
                child_killer(i, children_pids);
                return 1;
            default:
                children_pids[i] = pid;
                printf("Parent[%d] with child[%d]\n", getpid(), children_pids[i]);
                //pid = wait(NULL);
                break;
        }

        child_num++;
        sleep(1);
    }

    int stat;
    for(int j = 0; j<child_num; j++){
        pid = wait(&stat);
        printf("Exit status of child[%d]: %d\n", children_pids[j], WEXITSTATUS(stat));
    }

    printf("\nNo more children for parent[%d].\n\n", getpid());

    return 0;
}