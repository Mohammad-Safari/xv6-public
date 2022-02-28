#include "types.h"
#include "stat.h"
#include "user.h"
int main()
{
    printf(1, "tick %d :: parent started\n",getTicks());
    printf(1, "getProcInfo() will be tested in forked child\n");
    if (fork() == 0)
    {
        getProcInfo();
    }
    else
    {
        // some heavy task must keep parent busy while child is running
        printf(1, "parent is going to wait!\n"); // may be interrupted by child task message
        wait();
        printf(1, "tick %d :: child finished and parent is going to finish\n",getTicks());
    }
    exit();
}
