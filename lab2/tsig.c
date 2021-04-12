#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

#define NUM_CHILD 11
#define INFINITE_LOOP for(;;)
#define WITH_SIGNALS

int key_pressed = 0;


/* interrupt signal: Linux default =  ctrl+c */

#ifdef WITH_SIGNALS
void sig_int_pressed(){
    /* Function printing message when interruption signal triggered */
    printf("Parent[%d]: Interrupt triggered\n", getpid());
    key_pressed = 1;
}

void sig_term_prompt(){
    /* Function printing message that child is terminated because of interrupt */
    printf("Child[%d]: Terminated\n", getpid());
}
#endif 

void child_proccess(){
    /* Child Process algorithm */
    #ifdef WITH_SIGNALS
        signal(SIGINT, SIG_IGN);
        signal(SIGTERM, sig_term_prompt);
    #endif
    printf("Child[%d]: under Parent[%d]\n", getpid(), getppid());
    sleep(10);
    printf("Child[%d]: Execution completed\n", getpid());
}

void child_terminator(int i, pid_t pids[]){     // SIGTERM - correct closing of the process
    /* Child processes killing function */      // SIGKILL - destroying process
    for (int j = 0; j < i; j++)
        kill(pids[j], SIGTERM);                 // kill signals: SIGTERM, SIGKILL, SIGSTOP, SIGCONT
}

int main(void)
{
    int i;
    pid_t pid;
    pid_t children_pids[NUM_CHILD];
    printf("\nINITIAL PARENT PID: %d\n\n", getpid());
    
    /* Ignoring all signals with the signal() */
    
    #ifdef WITH_SIGNALS
        for(int it = 0; it < _NSIG; it++)
            signal(it, SIG_IGN);

        signal(SIGCHLD, SIG_DFL);   // restoring default handler for SIGCHLD 
                                    // SIGCHLD tells parent that one of the child processes ended
        signal(SIGINT, sig_int_pressed);    //Interrupt handler
    #endif
    
    /* Main process algorithm */
    for(i = 0; i < NUM_CHILD; i++){
        
        switch(pid = fork()){

            case 0:
                child_proccess();
                return 0;
            case -1:
                printf("Fork() failed\n");
                child_terminator(i, children_pids);
                return 1;
            default:
                children_pids[i] = pid;
                if(key_pressed == 0)
                    printf("Parent[%d]: with new Child[%d]\n", getpid(), pid);
                else{
                    printf("Parent[%d]: Creation process of Child[%d] interrupted\n", getpid(), pid);
                    child_terminator(i, children_pids);
                }
                break;

        }
        sleep(1);
    }

    int status, num_zero = 0, num_one = 0;

    INFINITE_LOOP{

        pid = wait(&status);
        if(pid == -1) // Stopping loop  // wait() returns PID of the child
            break;                      // if -1 is returned error occurs -> we are in parent
        if(WIFEXITED(status) == true)
            printf("Child[%d]: Exit status: %d\n", pid, WEXITSTATUS(status));
        
        if(WEXITSTATUS(status) == 0)
            num_zero++;
        else
            num_one++;
    }

    printf("\nNo more children for Parent[%d].\n", getpid());
    printf("Number of recieved 0 exit codes: %d\n", num_zero);
    printf("Number of recieved 1 exit codes: %d\n\n", num_one);
    
    /* Setting signals to default */
    #ifdef WITH_SIGNALS
        for(int it = 0; it < _NSIG; it++)
            signal(it, SIG_DFL);
    #endif
    
    return 0;
}
