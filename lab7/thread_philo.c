#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define phil_num	5
#define LEFT	(i + phil_num - 1) % phil_num
#define RIGHT	(i + 1) % phil_num

#define THINKING 2
#define HUNGRY 1
#define EATING 0

/* Mutexes variables */
int	state[phil_num];
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t s[phil_num] = PTHREAD_MUTEX_INITIALIZER;	
pthread_t threads[phil_num];

int j;

void test(int i)
{
	if( state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING )
	{
		state[i] = EATING;
		pthread_mutex_unlock(&s[i]);
	}
}

void grab_forks(int i)
{   
	pthread_mutex_lock(&m);
    state[i] = HUNGRY;
    printf("Philosopher[%d] is hungry\n", i);
    test(i);
	pthread_mutex_unlock(&m);
	pthread_mutex_lock(&s[i]);
}

void put_away_forks(int i)
{
	pthread_mutex_lock(&m);
    state[i] = THINKING;
    printf("Philosopher[%d] puts away fork[%d] and fork[%d]\n", i, i, i+1);
    test(LEFT);
    test(RIGHT);
	pthread_mutex_unlock(&m);
}

void eat(int i)
{/*Philosopher eats*/

    sleep(2);
    printf("Philosopher[%d] takes fork[%d] and fork[%d]\n",
    i, i, i+1);
    printf("Philosopher[%d] is eating\n", i);
}

void think(int i)
{/*Print when philosopher thinks*/

    printf("Philosopher[%d] is thinking\n", i); 
}

void* philospher(void* num)
{
    int* i = num;
    int j=0;
    while(j < 2)    // Each Philosopher eats a meal 2 times.  
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

void threads_initializer()
{
    for(j = 0; j < phil_num; j++)
    {
        pthread_mutex_lock(s+j);
    }

    int num[phil_num];

    for (j = 0; j < phil_num; j++)
    {   // Creating new threads; pthread_create returns 0 on success;
        num[j] = j;
        if(pthread_create(&threads[j], NULL, philospher, &num[j]) != 0){
            printf("\npthread_create() did not manage to create a new threat\n");
            exit(EXIT_FAILURE);
        }
    }
    sleep(25); // More or less the time of execution of phil_num threads of philosopher()
    for (j = 0; j < phil_num; j++)
    {   // Joining previously created threads;
        pthread_cancel(threads[j]);
        pthread_join(threads[j], NULL);

    }
}

int main(void)
{
    threads_initializer();
    exit(EXIT_SUCCESS);
}

/* Answering lab questions:

1. Would it be sufficient just to add to the old algorithm from task5
additional mutex variable to organize critical sections in functions
grab_forks() and put_away_forks() for making changes to values of two mutexes
indivisably?  If not, why?

2. Why m mutex is initialized with 1 and mutexes from the array s are
initialized with 0's?

1. It would not be sufficient. Mainly due to the fact, that in in 5th task we were
assigned to work on processes and algorithm was more focused whether the fork is being in use
or not. Not if the philosopher (threat/process) is eating or not. Hence this task needed more
work than just adding a new variables with new type to the grab/put fucntions. 

2. Mutex m is initialized with 1 for the simple purpose of philosophers being able to grab/put away forks.
With m initialized as 0 no philosopher could make an 'action' and the deadlock would occur. 
Array s on the other hand is initialized with 0 for the purpose of blocking philosophers after an 
attempt of grabbing forks. 

*/