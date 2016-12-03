#include <iostream>
#include <fstream>
#include <iterator>

using namespace std;

void Getinfo(ifstream &file, string title)
{
    string info;

    if(file.is_open())
    {
        cout << title << endl;
        info.assign((istreambuf_iterator<char>(file.rdbuf())), istreambuf_iterator<char>());
        cout << info << endl;
        file.close();
    }
    else cerr << "Unable to open file" << endl;
}

int main ()
{
    ifstream cpu("/proc/cpuinfo"), mem("/proc/meminfo");

    Getinfo(cpu, "CPU Information: ");
    Getinfo(mem, "Memory Information: ");


   return 0;
}

