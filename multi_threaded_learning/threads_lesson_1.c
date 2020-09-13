#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* myTurn(void *arg)
{
  for (size_t i = 0; i < 9; i++) {
    printf("myTurn i=%ld\n", i);
    sleep(1);
  }
  return NULL;
}
void yourTurn()
{
  for (size_t i = 0; i < 3; i++) {
    printf("yourTurn i=%ld\n", i);
    sleep(2);
  }
}

int main(int argc, char **agv)
{
  pthread_t myTurnThread;
  int thread_create_status = pthread_create(&myTurnThread, NULL, myTurn, NULL);
  if (thread_create_status > 0) {
    perror("Failed to create thread");
    exit(1);
  }

  yourTurn();
  int thread_join_status = pthread_join(myTurnThread, NULL);
  if (thread_join_status > 0) {
    perror("Failed to join thread");
    exit(1);
  }

  return 0;
}
