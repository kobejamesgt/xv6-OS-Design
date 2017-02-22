#include "types.h"
#include "user.h"
#include "syscall.h"

#define NUM_ELEMENTS 100

int mutex;
int *main_buffer;

unsigned short lfsr = 0xACE1u;
unsigned bit;

unsigned rand() {
  bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
  return lfsr =  (lfsr >> 1) | (bit << 15);
}

void producer(void *arg)
{
  int i;

  for(i = 0; i < NUM_ELEMENTS; i++)
  {
    mtx_lock(mutex);
    main_buffer[i] = rand();
    printf(1,"Producer put %d\n", main_buffer[i]);
    mtx_unlock(mutex);

    if(i == (NUM_ELEMENTS / 2))
    {
      sleep(100);
    }
  }

  exit();
}

void consumer(void *arg)
{
  int i;

  mtx_lock(mutex);

  printf(1,"\nConsumer has [");
  for(i = 0; i < NUM_ELEMENTS; i++)
  {
    if(main_buffer[i] != -1)
    {
      printf(1, "%d, ", main_buffer[i]);
      main_buffer[i] = -1;
    }
  }
  printf(1, "]\n");

  mtx_unlock(mutex);

  exit();
}

int main(int argc, char *argv[])
{
  mutex = mtx_create(1);
  main_buffer = (int *)malloc(NUM_ELEMENTS * sizeof(int));

  memset(main_buffer, -1, NUM_ELEMENTS * sizeof(int *));

  void *stack = (void *)malloc(4096);
  void *return_stack;
  void *args[2];

  thread_create((void *)producer, stack, args[0]);
  thread_join((void **)&return_stack);

  thread_create((void *)consumer, stack, args[1]);
  thread_join((void **)&return_stack);

  free(return_stack);
  exit();
}