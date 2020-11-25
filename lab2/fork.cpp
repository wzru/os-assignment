#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
using namespace std;
int main()
{
    auto pid = fork();
    if (pid == 0)
    {
        sleep(5);
        cout << "I'm child:" << getpid() << endl;
        exit(-1);
    }
    else
    {
        cout << "I'm parent:" << getpid() << " and my child is " << pid << endl;
        int status = 0;
        wait(&status);
        cout << "Now my child exited with " << WEXITSTATUS(status) << endl;
    }
    return 0;
}