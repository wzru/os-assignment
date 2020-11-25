#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>
using namespace std;
void signal_handler(int sig)
{
    if (SIGTERM == sig)
    {
        cout << "Bye, world!" << endl;
        exit(-1);
        // printf("Received signal [%s]\n", SIGQUIT == signum ? "SIGQUIT" : "Other");
    }
}
int main()
{
    auto pid = fork();
    if (pid == 0)
    {
        signal(SIGTERM, signal_handler);
        // cout << "I'm child:" << getpid() << endl;
        int cnt = 0;
        while (1)
        {
            cout << "\nI am Child Process, alive! " << cnt++ << endl;
            sleep(2);
        }
    }
    else
    {
        cout << "I'm parent:" << getpid() << " and my child is " << pid << endl;
        while (1)
        {
            sleep(2);
            cout << "To terminate Child Process, Yes or No(Y/N):";
            char ch = getchar();
            if (ch == 'y' || ch == 'Y')
            {
                cout << "Choosed YES" << endl;
                kill(pid, SIGTERM);
                break;
            }
            else
            {
                cout << "Choosed NO" << endl;
            }
        }
        int status = 0;
        wait(&status);
        cout << "Now my child exited with " << WEXITSTATUS(status) << endl;
    }
    return 0;
}