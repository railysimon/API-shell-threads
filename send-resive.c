#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = 	PTHREAD_COND_INITIALIZER;

int condition = 0;

void send_msg()
{
	condition = 1;
	pthread_cond_broadcast(&cond);
}

void resive_msg()
{
	pthread_mutex_lock(&mutex);
	while(!condition)
		pthread_cond_wait(&cond, &mutex);
	pthread_mutex_unlock(&mutex);
}

void *someThread(void *p)
{
	int threadID = *(int*) p;
	printf("Thread %i is waiting for message...\n", threadID);
	resive_msg();
	printf("Thread %i reseive the message...\n", threadID);
}

int main()
{
	const int thread_count = 5;
	pthread_t threads[thread_count];

	send_msg();

	for(int i=0; i<thread_count; i++)
	{
		printf("Thread %i is starting\n", i);
		pthread_create(&threads[i], NULL, someThread, &i);
	}

	printf("Message sent for all threads\n");
	return 0;
}
