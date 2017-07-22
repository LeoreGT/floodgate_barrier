#include <pthread.h>  /* pthread work     */
#include <unistd.h>   /* sleep            */
#include <stdio.h>    /* printf           */
#include <stdlib.h>   /* size_t and malloc*/

#include "barrier.h"

#define STILL_WAITING 2
#define ALL_ARRIVED   1
#define SUCCESS  0
#define FAILURE -1

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int BarrierInit(Barrier* _barrier, unsigned int _count)
{
  pthread_cond_init(&_barrier->m_cond, NULL);
  pthread_mutex_init(&_barrier->m_mutex, NULL);
  _barrier->m_condition = _count;
  _barrier->m_threadsBarrier1 = _count;
  return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int BarrierWait(Barrier* _barrier)
{
  int result = STILL_WAITING;
  pthread_mutex_lock(&_barrier->m_mutex);
  --(_barrier->m_condition);
  if(_barrier->m_condition > 0)
  {
    pthread_cond_wait(&_barrier->m_cond, &_barrier->m_mutex);
    pthread_mutex_unlock(&_barrier->m_mutex);
  }
  else
  {
    _barrier->m_condition = _barrier->m_threadsBarrier1;
    pthread_cond_broadcast(&_barrier->m_cond);
    result =  ALL_ARRIVED;
  }
  pthread_mutex_unlock(&_barrier->m_mutex);
  return result;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int BarrierDestroy(Barrier* _barrier)
{
  pthread_cond_destroy(&_barrier->m_cond);
  pthread_mutex_destroy(&_barrier->m_mutex);

  return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
