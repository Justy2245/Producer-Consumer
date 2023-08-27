# Producer-Consumer
Implemented a version of the producer-consumer problem from a textbook using the PthreadsAPI.
The program creates a number of producer threads, which inserts a random number between 1 and 3 onto a shared buffer and then has the thread sleep for the same amount of seconds. 
There are also consumer threads, which are also running the will consume items off of the shared buffer and sleep for 1 to 3 seconds. 
The program takes in three arguments from the command line:
1. Length of time the program will run
2. Number of producer threads
3. Number of consumer threads

Example in linux terminal would be : ./jtprog4 25 3 3
Which means the program will run for 25 seconds and have 3 producer and consumer threads
