#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_api.c"

// TEST 1:
// simple run "whitout exit" test
// because exit is not still modified
// for thread compatibilty and result
// in kernel panic
// void print(void *args)
// {
//     while (1)
//     {
//         printf(1, "hello\n");
//         sleep(100);
//     }
//     return;
// }
// int main()
// {
//     thread_creator(print, (void *)NULL);
//     while (1)
//         ;
// }

// TEST 2:
// simple run "whitout exit" test by arg
// void print(void *args)
// {
//     printf(1, "hello from thread-%d\n", *(int *)args);
//     while (1)
//         ;
//     return;
// }
// int main()
// {
//     int i = 0;
//     while (1)
//     {
//         thread_creator(print, (void *)&i);
//         sleep(100);
//         i++;
//     }
// }

// TEST 3:
// simple run "whitout exit" test with 
// address space checked being shared
// in both main thread and child thread
int i = 0;
void print(void *args)
{
    while (1)
    {
        printf(1, "hello from thread, var i is now %d in main thread\n", *(int *)args);
        sleep(100);
    }
    return;
}
int main()
{
    thread_creator(print, (void *)&i);
    while (1)
    {
        sleep(100);
        i++;
    }
}