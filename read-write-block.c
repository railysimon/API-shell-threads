#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int someVal = 0;

pthread_mutex_t indexMutex = PTHREAD_MUTEX_INITIALIZER;

int getIndex()
{
	static index = 0;
	index++;
	return index;
}

void *readThread(void *p)
{
	pthread_mutex_lock(&indexMutex);
	int thindex = getIndex();
	pthread_mutex_unlock(&indexMutex);

	pthread_rwlock_rdlock(&rwlock);
	printf("Потік № %i почав зчитування значення.\n", thindex);
	usleep(500);
	printf("Потік № %i, значення: %i\n", thindex, someVal);
	pthread_rwlock_unlock(&rwlock);
}

void *writeThread(void *p)
{
	pthread_mutex_lock(&indexMutex);
	int thindex = getIndex();
	pthread_mutex_unlock(&indexMutex);

	pthread_rwlock_wrlock(&rwlock);
	printf("Потік № %i записує значення.\n", thindex);
	usleep(500);
	someVal = thindex * 10;
	usleep(500);
	printf("Потік № %i закінчив запис.\n", thindex);
	pthread_rwlock_unlock(&rwlock);
}

void *controlThread(void *p)
{
	pthread_rwlock_rdlock(&rwlock);
	printf("Значення після записування: %i\n", someVal);
	pthread_rwlock_unlock(&rwlock);
}

int main()
{
	pthread_t readth1, readth2, writeth;
	pthread_create(&readth1, NULL, readThread, NULL);
	pthread_create(&readth2, NULL, readThread, NULL);
	pthread_create(&writeth, NULL, writeThread, NULL);

	pthread_join(readth1, NULL);
	pthread_join(readth2, NULL);
	pthread_join(writeth, NULL);
	pthread_join(readth1, NULL);

	pthread_t controlth;
	pthread_create(&controlth, NULL, controlThread, NULL);
	pthread_join(controlth, NULL);

	return 0;
}
