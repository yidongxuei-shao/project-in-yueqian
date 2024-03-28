#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
/*************
 *  THREADS
 * *********/
pthread_t self;
void *task_thread(void *arg) 
{
    // pthread_detach(pthread_self());
    int ret = 6;
    self = pthread_self();
    pthread_exit((void*)&ret);

    return NULL;
}

int main(void)
{

    pthread_t thread;
    pthread_create(&thread, NULL, task_thread, NULL);

    sleep(1);
    int *ret = NULL;
    pthread_join(thread, (void**)&ret);
    printf("returned %d\n", *(int *)ret);
    
    return 0;
}