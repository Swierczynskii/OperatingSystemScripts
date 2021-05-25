#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define INF_LOOP for(;;)
#define phil_num 5
#define right (left + 1) % phil_num
int philosophers[phil_num];
static int semid;


void grab_forks(int left)
{/* current philosopher id == Left fork id
    next philosopher id == right fork id
    Function responsible for grabbing forks*/

    printf("Philosopher[%d] is hungry\n", philosophers[left]);

    struct sembuf sops[] = {
    /*Operate on semaphore left and right; Wait for semval to become
    greater or equal to the abs value of sem_op;*/
        {.sem_num = left, .sem_op = -1, .sem_flg = 0},
        {.sem_num = right, .sem_op = -1, .sem_flg = 0},
    };
    if(semop(semid, sops, 2) == -1) // returns -1 on failure 
    {/*int semop(int semid, struct sembuf *sops, size_t nsops)*/
        printf("\nsemop() did not manage to do an operation\n");
        exit(EXIT_FAILURE);
    }
}

void put_away_forks(int left)
{/*Function responsible for putting forks away*/

    struct sembuf sops[] = {
    /*Operate on semaphore left and right; Increment value by 1;*/
        {.sem_num = left, .sem_op = 1, .sem_flg = 0},
        {.sem_num = right, .sem_op = 1, .sem_flg = 0},
    };

    if(semop(semid, sops, 2) == -1)
    {
        printf("\nsemop() did not manage to do an operation\n");
        exit(EXIT_FAILURE);
    }    

    printf("Philosopher[%d] puts away fork[%d] and fork[%d]\n", philosophers[left], left, right);

}

void eat(int left)
{/*Philosopher eats*/

    sleep(2);
    printf("Philosopher[%d] takes fork[%d] and fork[%d]\n",
    philosophers[left], left, right);
    printf("Philosopher[%d] is eating\n", philosophers[left]);
}

void think(int left)
{/*Print when philosopher thinks*/

    printf("Philosopher[%d] is thinking\n", left); 
}

void* philospher(void* num)
{/*Function responsible for philosopher behavior*/

    int* i = num;
    int j=0;
    while(j < 2)    // Each philosopher eats his meal 2 times.
    {
        think(*i);
        sleep(1);
        grab_forks(*i);
        eat(*i);
        sleep(1);   
        put_away_forks(*i);
        j++;
    }
    return NULL;
}

void philosophers_terminator(int i, pid_t pids[])
{/*If fork() fails terminate already forked philosophers*/
    for (int j = 0; j < i; j++)
        kill(pids[j], SIGTERM);                
}

int main(void)
{
    int i;
    semid = semget(IPC_PRIVATE, phil_num, 0666); // getting semaphore set id;

    if (semid < 0)
    {/*semget() returns nonnegative integer on success, -1 on failure;*/
        printf("\nsemget() did not manage to create semaphores\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < phil_num; i++)  
    {/*Assigning numbers and semaphore values to philosophers*/                             
        philosophers[i]=i;
        if(semctl(semid, i, SETVAL, 1) < 0){    // semctl() returns -1 on failure;
            printf("\nsemctl() did not manage to set value for %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid;
    pid_t philosophers_pids[phil_num];
    
    for (i = 0; i < phil_num; i++) 
    {
        switch(pid = fork())
        {/*process divider*/
            case 0:
                philospher(&philosophers[i]);
                exit(EXIT_SUCCESS);
            case -1:
                printf("Fork() failed\n");
                philosophers_terminator(i, philosophers_pids);
                exit(EXIT_FAILURE);
            default:
                philosophers_pids[i] = pid;
                break;
        }
    }

    int status;

    INF_LOOP
    {/*Infinite loop*/ 
        pid = wait(&status);    // parents process waits for children processes
        if(pid == -1)           // if parent -> break
            break;
    }

    exit(EXIT_SUCCESS);
}



