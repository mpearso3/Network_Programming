#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#define BIG 1000000000UL
uint32_t count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* count_to_big(void *arg)
{
  for (uint32_t i = 0; i < BIG; i++) {
    pthread_mutex_lock(&lock);
    count++;
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}

int main(int argc, char **argv)
{
  pthread_t t;
  pthread_create(&t, NULL, count_to_big, NULL);
  count_to_big(NULL);
  pthread_join(t, NULL);

  printf("Done, count = %u\n", count);
  return 0;
}
