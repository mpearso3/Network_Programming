#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* myTurn(void *arg)
{
  int *result = (int *)malloc(sizeof(int));
  *result = 0;

  int *arg_ptr = (int *)arg;
  for (size_t i = 0; i < 8; ++i) {
    printf("myTurn i=%ld arg_ptr=%d\n", i, *arg_ptr);
    sleep(1);
    (*arg_ptr)++;
    (*result)++;
  }
  return result;
}
void yourTurn()
{
  for (size_t i = 0; i < 3; ++i) {
    printf("yourTurn i=%ld\n", i);
    sleep(2);
  }
}

int main(int argc, char **argv)
{
  pthread_t my_turn_thread;
  int v = 5;

  int thread_create_status = pthread_create(&my_turn_thread, NULL, myTurn, &v);
  if (thread_create_status > 0) {
    perror("Failed to create thread");
    exit(1);
  }

  yourTurn();

  int *result = NULL;
  int thread_join_status = pthread_join(my_turn_thread, (void*)&result);
  if (thread_join_status > 0) {
    perror("Failed to join thread");
    exit(1);
  }
  printf("final value v=%d\n", v);
  printf("*result=%d\n", *result);
  free(result);

  return 0;
}
