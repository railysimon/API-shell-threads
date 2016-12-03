#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

void *server(void *per)
{
	int fd1;
	int *map;

	fd1 = open("tmp", O_RDWR | O_CREAT, 0644);
	lseek(fd1, sizeof(int), SEEK_SET);
	write(fd1, "", 1);
	map = (int*)mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, fd1, 0);
	close(fd1);

	map[0] = 5;
	while(1)
	{
		printf("Server temperature is: %i\n", map[0]);
		if(!map[0]) break;
		usleep(200);
		map[0]--;
	}
}

int index = 0;
pthread_mutex_t i = PTHREAD_MUTEX_INITIALIZER;

int getIndex()
{
	index++;
	return index;
}

void *client(void *par)
{
	usleep(200);
	int fd1;
	int *map;

	pthread_mutex_lock(&i);
	int index = getIndex();
	pthread_mutex_unlock(&i);

	fd1 = open("./tmp", O_RDONLY);
	map = (int*)mmap(0,sizeof(int), PROT_READ, MAP_SHARED, fd1, 0);
	close(fd1);

	while(1)
	{
		printf("client %i temperature is: %i\n", index, map[0]);
		if (!(map[0])) break;
		usleep(200);
	}
}

int main()
{
	int clientCount = 3;
	pthread_t serverThread;
	pthread_t clientThreads[clientCount];

	pthread_create(&serverThread, NULL, server, NULL);
	for(int i=0; i<clientCount; i++)
		pthread_create(&clientThreads[i], NULL, client, NULL);

	for(int i=0; i<clientCount; i++)
		pthread_join(clientThreads[i], NULL);
	pthread_join(serverThread, NULL);

	return 0;
}
