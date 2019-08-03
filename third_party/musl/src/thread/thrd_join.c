#include <stdint.h>
#include <threads.h>

int __pthread_join(thrd_t, void**);

int thrd_join(thrd_t t, int *res)
{
        void *pthread_res;
        __pthread_join(t, &pthread_res);
        if (res) *res = (int)(intptr_t)pthread_res;
        return thrd_success;
}
