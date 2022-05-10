#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_api.c"

// TEST 1:
// simple run "without exit" test
// because exit is not still modified
// for thread compatibility and result
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
// simple run "without exit" test by arg
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
// simple run "without exit" test with
// address space checked being shared
// in both main thread and child thread
// int i = 0;
// void print(void *args)
// {
//     while (1)
//     {
//         printf(1, "hello from thread, var i is now %d in main thread\n", *(int *)args);
//         sleep(100);
//     }
//     return;
// }
// int main()
// {
//     thread_creator(print, (void *)&i);
//     while (1)
//     {
//         sleep(100);
//         i++;
//     }
// }

// // TEST 4:
// // simple run "with exit" test with
// // with 2 hierarchy threads by sleep
// // in both main thread and child thread
// void print2(void *args)
// {
//     printf(1, "thread is created - tid %d\n", thread_id());
//     return;
// }
// void print(void *args)
// {
//     printf(1, "thread is created - tid %d\n", thread_id());
//     thread_creator(print2, (void *)NULL);
//     sleep(500);
//     return;
// }
// int main()
// {
//     printf(1, "main thread is goting to create other threads\n");
//     thread_creator(print, (void *)NULL);
//     // sleep(100);
//     thread_creator(print, (void *)NULL);
//     // sleep(700);
//     exit();
// }

// TEST 5:
// simple run "with join" test with
// with 2 hierarchy threads by sleep
// in both main thread and child thread
void print2(void *args)
{
    printf(1, "thread is created in print2 - tid %d\n", thread_id());
    return;
}
void print(void *args)
{
    printf(1, "thread is created - tid %d\n", thread_id());
    int tid = thread_creator(print2, (void *)NULL);
    printf(1, "tid exited %d\n", thread_join(tid));
    // sleep(500);
    return;
}
int main()
{
    printf(1, "main thread is goting to create other threads\n");
    int tid = thread_creator(print, (void *)NULL);
    printf(1, "tid exited %d\n", thread_join(tid));
    // sleep(500);
    int tid2 = thread_creator(print, (void *)NULL);
    printf(1, "tid exited %d\n", thread_join(tid2));
    // sleep(500);
    exit();
}