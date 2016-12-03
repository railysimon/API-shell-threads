#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <string.h>

void *server(void *par)
{
	mkfifo("server", 0644);

	int file;
	char fileName[100], fileEntry[10000];
	int length;

	while(1)
	{
		file = open("server", O_RDONLY);
		length = read(file, fileName, sizeof(fileName));
		close(file);

		if(!strcmp(fileName, "exit")) break;
	
		file = open(fileName, O_RDONLY);
		length = read(file, fileEntry, sizeof(fileEntry));
		close(file);

		if(length > 0)
		{
			file = open("client", O_WRONLY);
			write(file, fileEntry, sizeof(fileEntry));
			close(file);
		}
		else
		{
			file = open("client", O_WRONLY);
		char errorStr[] = "Error while try to open file\nMaybe file doens't exist!";
			write(file, errorStr, sizeof(errorStr));
			close(file);
		}
	}

}

void *client(void *per)
{

	mkfifo("client", 0644);

	int file;
	char input[100], fileEntry[10000];
	int length;

	while(1)
	{
		printf("Enter file name: ");
		scanf("%s", input);
		printf("\n");

		file = open("server", O_WRONLY);
		length = write(file, input, sizeof(input));
		close(file);

		if(!strcmp(input, "exit")) break;

		file = open("client", O_RDONLY);
		length = read(file, fileEntry, sizeof(fileEntry));
		close(file);

		if(length > 0)
		{
			fileEntry[length] = '\0';
			printf("File entry: %s\n", fileEntry);
		}
	}

	unlink("client");
}

int main()
{
	pthread_t ser, cl;
	pthread_create(&ser, NULL, server, NULL);
	pthread_create(&cl, NULL, client, NULL);

	pthread_join(ser, NULL);
	pthread_join(cl, NULL);

	return 0;
}
