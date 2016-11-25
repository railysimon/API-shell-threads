#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

void kill(int status)
{
    _exit(status);
}

int main()
{
    pid_t child_pid;
    child_pid = fork();

    if(!child_pid)
    {
        cout << "Нащадок створено. PID: " << getpid() << endl; // виконання процесу нащадка
        char *argv[] = {"ls", "-l", NULL};
        execvp(argv[0], argv);
    }

    else if (child_pid > 0) // виконання процесу предка
    {
        waitpid(child_pid, 0, 0); // очікує завершення нащадка і вилучає керуючий блок
        cout << "Батьківський процес з pid: " << getpid() << endl;

        sleep(3);
        cout << "Процес і його нащадки завершено." << endl;
        kill(0); // завершає процес-предок
    }
    else if(child_pid == -1)
    {
        cout << "Нащадок не створено!" << endl;
        exit(-1);
    }

    return 0;
}

