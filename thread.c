#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *id)
{
	int loc = *(int *) id;
	for(int i=0; i<5; i++)
	printf("Hello from thred %d\n", loc);
}

int main()
{
	int id = 1;

	pthread_t th1, th2;

	pthread_create(&th1, NULL, thread_function, &id);
	pthread_join(th1, NULL);

	id = 2;
	printf("\n");
	sleep(2);	
	
	pthread_create(&th2, NULL, thread_function, &id);
	pthread_join(th2, NULL);

	return 0;
}

