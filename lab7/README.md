Implement in the C language again the dining philosophers program. Now,
instead of usage of processes and semaphores, use threads and mutexes from the
pthreads library.

For synchronization again implement and use the two following functions: 

void grab_forks( int philo_id );

void put_away_forks( int philo_id );

where parameters are integers identifying semaphores associated with each
fork.  grab_forks() and put_away_forks() should use pthread mutexes.

Because with mutexes we are not able to change more than one mutex value in
one call, the abovementioned functions have to be implemented differently. 

Remarks:

- use pthreads_XXX and pthread_mutex_XXX interfaces, 
- do not use old thread_XXX and mutex_XXX interfaces,
- manual pages to read: 
	pthreads
	pthread_create (see EXAMPLES section as well)
	pthread_mutex_init, pthread_mutex_lock, pthread_mutex_unlock,


Additional questions:

1. Would it be sufficient just to add to the old algorithm from task5
additional mutex variable to organize critical sections in functions
grab_forks() and put_away_forks() for making changes to values of two mutexes
indivisably?  If not, why?

2. Why m mutex is initialized with 1 and mutexes from the array s are
initialized with 0's?
