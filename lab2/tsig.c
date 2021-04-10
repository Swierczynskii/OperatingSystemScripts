#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_CHILD 7
#define INFINITE for(;;)
#define WITH_SIGNALS

#ifdef WITH_SIGNALS
    int key_pressed = 0;
#endif 

/* interrupt signal: default =  ctrl+c */

void child_proccess(){
    /* Child Process algorithm */

    printf("Child[%d]: under Parent[%d]\n", getpid(), getppid());
    sleep(10);
    printf("Child[%d]: Execution completed\n", getpid());
}

void child_killer(int i, pid_t pids[]){
    /* Child processes killing function */
    for (int j = 0; j < i; j++){
    
        kill(pids[j], SIGTERM);
    
    }
}

#ifdef WITH_SIGNALS
void sig_int_pressed(int signal){

    //printf("Interrupt triggered for Parent[%d]\n", getpid());
    key_pressed = 1;

}
#endif 

int main(void)
{
    int i;
    pid_t pid;
    pid_t children_pids[NUM_CHILD];
    printf("\nINITIAL PARENT PID: %d\n\n", getpid());
    
    /* Ignoring all signals with the signal() */
    
    #ifdef WITH_SIGNALS
        for(int it = 0; it < _NSIG; it++){
            signal(it, SIG_IGN);
        }

        signal(SIGCHLD, SIG_DFL);
        signal(SIGINT, sig_int_pressed);
    #endif
    
    /* Main process algorithm */
    for(i = 0; i < NUM_CHILD; i++){
        
        switch(pid = fork()){

            case 0:
                child_proccess();
                return 0;
            case -1:
                printf("Fork failed\n");
                child_killer(i, children_pids);
                return 1;
            default:
                children_pids[i] = pid;
                printf("Parent[%d]: with new Child[%d]\n", getpid(), children_pids[i]);
                break;
        }
        #ifdef WITH_SIGNALS
            if(key_pressed == 1){
                printf("Parent[%d]: Creation process interrupted\n", getpid());
                child_killer(i, children_pids);
            }
        #endif
        sleep(1);
    }

    int stat, num_zero = 0, num_one = 0;
    int j = 0;

    INFINITE{

        pid = wait(&stat);
        if(pid == -1)
            //printf("Stopping loop\n");
            break;

        printf("Child[%d]: Exit status: %d\n", children_pids[j], WEXITSTATUS(stat));
        
        if(WEXITSTATUS(stat) == 0)
            num_zero++;
        else
            num_one++;
        j++;
    }

    printf("\nNo more children for Parent[%d].\n", getpid());
    printf("Number of recieved 0 exit codes: %d\n", num_zero);
    printf("Number of recieved 1 exit codes: %d\n\n", num_one);
    
    #ifdef WITH_SIGNALS
        for(int it = 0; it < _NSIG; it++){
            signal(it, SIG_DFL);
        }
    #endif
    
    return 0;
}
