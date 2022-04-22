#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_api.c"

// TEST 1:
// simple run "whitout exit" test
// because exit is not still modified
// for thread compatibilty and result
// in kernel panic
void print(void *args)
{
    while (1)
    {
        printf(1, "hello\n");
        sleep(100);
    }
    return;
}
int main()
{
    thread_creator(print, (void *)NULL);
    while (1)
        ;
}
