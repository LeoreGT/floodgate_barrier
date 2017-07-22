#include <pthread.h>  /* pthread work     */
#include <unistd.h>   /* sleep            */
#include <stdio.h>    /* printf           */
#include <stdlib.h>   /* size_t and malloc*/

#include "barrier.h"
#include "floodgate.h"

#define THREADS_NUM 10
#define CAPACITY 10

Floodgate* alligater;

void WithinBarrier(int* _threadIndex);

void* ThreadFunction (int* _threadIndex)
{
 printf("       Thread number: %d has begun                       \n", *_threadIndex);
 printf("     . Thread number: %d waiting outside the gate .      \n", *_threadIndex);

 FloodGate_Flow(alligater, (void(*)(void*))WithinBarrier, (void*)_threadIndex);

 printf("  .... Thread number: %d inside the gate now      ....   \n", *_threadIndex);
 printf("...... Thread number: %d is out                   ...... \n", *_threadIndex);
 return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void WithinBarrier(int* _threadIndex)
{
  printf("    .. Within Barrier: Thread number: %d          ..    \n", *_threadIndex);
}

/* NEED TO ADD IN SUMMERIZER TO RETURN COUNT (from first barrier?) */

// void WithinBarrier(int* _summer)
// {
//   printf("    .. SUMMERIZER: current count is %d             ..    \n", alligator->m_counter);
// }

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int main()
{
  int i;
  void* buff;
  pthread_t arrayOfThreads[THREADS_NUM];
  int threadID[THREADS_NUM];

  alligater = FloodGate_Init(CAPACITY);

  for (i = 0 ; i< THREADS_NUM; ++i){
    threadID[i] = i;
  }

  for (i = 0; i < THREADS_NUM; ++i){
    pthread_create(&(arrayOfThreads[i]), NULL, (void*(*)(void*))ThreadFunction, &threadID[i]);
  }

  for (i = 0; i < THREADS_NUM; ++i){
    pthread_join (arrayOfThreads[i], &buff);
  }

  /* still need to make summarizer */

  FloodGate_Destroy(alligater);
  return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
