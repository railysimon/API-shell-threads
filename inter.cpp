#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstdlib>

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

pid_t pid;

void Command(char *command)
{
	char *argv[20];
	int background;
	char *token;
	token = strtok(command, " ");
	int i = 0;
	
	for(; token != NULL; i++)
	{
		argv[i] = token;
		token = strtok(NULL, " ");
	}

	if (!strcmp(argv[i-1], "&"))
	{
		background = 1;
		argv[i-1] = NULL;
	} 
	else
	{
		background = 0;
		argv[i] = NULL;
	}	

	if ((pid = fork()) == -1) exit(-1);

	if(!pid)
	{
		execvp(argv[0], argv);
		exit(1);		
	}

	if(!background) wait(NULL);	
}

int main()
{
	system("clear");

	cout << "Shell is running!" << endl;

	char command[200];

	while(1)
	{
		cout << "> ";
		gets(command);

		if(command[0] == '\0')
		continue;

		if(!strcmp(command, "exit")) break;

		Command(command);
	}
	
	cout << "Shell is closed!" << endl; 

	return 0;
}

