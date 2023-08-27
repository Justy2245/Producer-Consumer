# Producer-Consumer
Implemented a version of the producer-consumer problem from a textbook in C using the PthreadsAPI.  

The program creates a number of producer threads, which inserts a random number between 1 and 3 onto a shared buffer and then has the thread sleep for the same amount of seconds. 

There are also consumer threads, which are also running that consume items off of the shared buffer and sleep for 1 to 3 seconds. 

Uses a mutex semaphore and two counting semaphores to allow only a single producer or consumer thread modify the shared buffer at a time as well as keep track if the buffer is full or empty, so as to not allow a producer thread
have access to the buffer when it is full and to now allow a consumer thread to have access if the buffer is empty.

The program takes in three arguments from the command line:
1. Length of time the program will run
2. Number of producer threads
3. Number of consumer threads

Example in linux terminal would be : ./jtprog4 25 3 3  

Which means the program will run for 25 seconds and have 3 producer and consumer threads
