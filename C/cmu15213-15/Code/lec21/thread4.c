#include "csapp.h"

/* Global shared variable */
volatile long cnt = 0; /* Counter */
sem_t mutex; /* Semaphore that protects cnt */

void *thread(void *vargp);

int main(int argc, char **argv)
{
  long niters;
  pthread_t tid1, tid2;
  niters = atoi(argv[1]);
  Sem_init(&mutex, 0, 1); /* mutex = 1 */
  Pthread_create(&tid1, NULL, thread, &niters);
  Pthread_create(&tid2, NULL, thread, &niters);
  Pthread_join(tid1, NULL);
  Pthread_join(tid2, NULL);
  /* Check result */
  if (cnt != (2 * niters))
    printf("BOOM! cnt=%ld\n", cnt);
  else
    printf("OK cnt=%ld\n", cnt);
  exit(0);
}

/* Thread routine */
void *thread(void *vargp)
{
  long i, niters = *((long *)vargp);
  for (i = 0; i < niters; i++){
    P(&mutex);
    cnt++;
    V(&mutex);
  }
  return NULL;
} 