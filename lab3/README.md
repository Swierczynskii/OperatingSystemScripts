

The scheduling simulator illustrates the behavior of scheduling algorithms against a simulated mix of process loads. 
The user can specify the number of processes, the mean and standard deviation for compute time and I/O blocking time for each process, and the duration of the simulation.
At the end of the simulation a statistical summary is presented.
Students may also be asked to write their own scheduling algorithms to be used with process loads defined by the instructor.

The program reads a configuration file (scheduling.conf) and writes two output files (Summary-Results and Summary-Processes).
To run the program, enter the following command line.

$ java Scheduling scheduling.conf

The program will display "Working..." while the simulation is working, and "Completed." when the simulation is complete.

Working...

Completed.

Create a configuration file in which all processes run an average of 2000 milliseconds with a standard deviation of zero, and which are blocked for input or output every 500 milliseconds.
Run the simulation for 10000 milliseconds with 2 processes.
Examine the two output files.
Try again for 5 processes.
Try again for 10 processes.
Explain what's happening.
