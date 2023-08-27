#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#define buffer_size 5

void *runner(void *param); 
void *producer(void *param);
void *consumer(void *param); 
void insert_item(int input);
void remove_item();
void printBuff();

int buffer[buffer_size];
sem_t mutex, empty, full;
int in = 0;
int out = 0;
int timer;
time_t begin;
int empty_space = 200;	/*200 is used in this case to denote empty since it
			is out range of the generated numbers*/

int main(int argc, char *argv[])
{
	printf("Main Thread Beginning\n");
	int i;
	int PNUM_THREADS = strtol(argv[2], NULL, 10);
	int CNUM_THREADS = strtol(argv[3], NULL, 10);
	int Plocaltid[PNUM_THREADS];
	int Clocaltid[CNUM_THREADS];
	pthread_t Ptid[PNUM_THREADS]; 	/* the thread identifier for producer*/
	pthread_t Ctid[CNUM_THREADS]; 	/* the thread identifier for consumer*/
	pthread_attr_t attr; 		/* set of attributes for the thread */
	
	for(int n = 0; n < buffer_size; n++)
	{
		buffer[n] = empty_space; /*initializing buffer to 200 to represent empty*/
	}
	
	timer = strtol(argv[1], NULL, 10);
	begin = time(NULL);	/*store time when program starts*/
	
	/*initializing semaphores*/
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, buffer_size);
	sem_init(&full, 0, 1);

	/* get the default attributes */
	pthread_attr_init(&attr);

	/* create the threads */
	for (i = 0; i < PNUM_THREADS; i++) { 
		Plocaltid[i-1] = i;
		/* Note that we are passing the thread id by reference */
		pthread_create(&Ptid[i],&attr,producer,(void *) &Plocaltid[i-1]); 
		printf("Main: Creating producer thread with id = %lu\n", Ptid[i]);
        }
        for (i = 0; i < CNUM_THREADS; i++) { 
		Clocaltid[i-1] = i;
		/* Note that we are passing the thread id by reference */
		pthread_create(&Ctid[i],&attr,consumer,(void *) &Clocaltid[i-1]); 
		printf("Main: Creating consumer thread with id = %lu\n", Ctid[i]);
        }
	printf("Main thread sleeping for %d seconds\n", timer);
	sleep(timer);
	printf("Main thread exiting\n");
	exit(0);
}

void *producer(void *param)	/*producer threads*/
{
	pthread_t ID = pthread_self();
	int loop = 1;
	time_t end = time(NULL);
	int timediff;
	while(loop)
	{
		end = time(NULL);	/*store current time*/
		timediff = end - begin;	/*subtract current time from original time*/
		if(timediff < timer)
		{
			int num = (rand() % 3) + 1;	/*random number between 1 and 3*/
			printf("Producer thread %ld sleeping for %d seconds\n", ID, num);
			sleep(num);			/*sleep for 1 to 3 seconds*/
			num = (rand() % 100) + 1;	/*generate number to later insert*/
			insert_item(num);	
		}
		else
		{
			loop = 0;
		}
	}
	pthread_exit(0);
}
void *consumer(void *param)	/*consumer threads*/
{
	pthread_t ID = pthread_self();
	int loop = 1;
	time_t end = time(NULL);
	int timediff;
	while(loop)
	{
		end = time(NULL);	/*store current time*/
		timediff = end - begin; /*subtract current time from original time*/
		if(timediff < timer)
		{
			int num = (rand() % 3) + 1;	/*random number between 1 and 3*/
			printf("Consumer thread %ld sleeping for %d seconds\n", ID, num);
			sleep(num);			/*sleep for 1 to 3 seconds*/
			num = (rand() % 100) + 1;	/*generate number to later insert*/
			remove_item();
		}
		else
		{
			loop = 0;
		}	
	}
	pthread_exit(0);

}
void insert_item(int input)
{
	pthread_t ID = pthread_self();
	sem_wait(&empty);
	sem_wait(&mutex);
	if(((in+1)%buffer_size) != out)		/*make sure in and out aren't the same*/
	{
		buffer[in] = input;	/*insert the item*/
		printf("Producer thread %ld inserted %d at position %d\n", ID, input, in);
		printf("Insert_item inserted item %d at position %d\n", input, in);
		in = (in + 1) % buffer_size;	/*increment in*/
		printBuff();
	}
	sem_post(&mutex);
	sem_post(&full);
}
void remove_item()
{
	pthread_t ID = pthread_self();
	sem_wait(&full);
	sem_wait(&mutex);
	if(in != out)		/*make sure in and out aren't the same*/
	{
		int consumed = buffer[out];
		buffer[out] = empty_space;	/*consume the item*/
		printf("Consumer thread %ld consumed %d at position %d\n", ID, consumed, out);
		printf("Remove_item removed item %d at position %d\n", consumed, out);
		out = (out + 1) % buffer_size;	/*increment out*/
		printBuff();
	}
	sem_post(&mutex);
	sem_post(&empty);
}
void printBuff()	/*function to print out the buffer*/
{
	for(int n = 0; n < buffer_size; n++)
	{
		if(buffer[n] == empty_space)
		{
			printf("[empty]");
		}
		else
		{
			printf("[%d]", buffer[n]);
		}
	}
	printf(" in = %d, out = %d", in, out);
	printf("\n");
}

