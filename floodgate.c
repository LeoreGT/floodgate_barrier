#include <pthread.h>  /* pthread work     */
#include <unistd.h>   /* sleep            */
#include <stdio.h>    /* printf           */
#include <stdlib.h>   /* size_t and malloc*/

#include "barrier.h"
#include "floodgate.h"

#define CAPACITY 10
#define BAD_INPUT -1

#define WEASLEY 0xDEADBEEF

struct Floodgate
{
  Barrier m_firstBarrier;
  Barrier m_secondBarrier;

  size_t m_threadCapacity;
  size_t m_threadsWaitingEntry;
  size_t m_magicNum;
  size_t m_counter; /* or just barrier count? */

  pthread_cond_t m_waitingCondition;
  pthread_mutex_t m_mutex;
  size_t m_numOfItemsInside;
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

Floodgate* FloodGate_Init(size_t _capacity)
{
  int res;
  Floodgate* fgate;

  fgate = (Floodgate*)malloc(sizeof(Floodgate));
  if (NULL == fgate)
  {
    return NULL;
  }
  BarrierInit(&fgate->m_firstBarrier, _capacity);   /* using the barrier function to create the barriers */
  BarrierInit(&fgate->m_secondBarrier, _capacity);

  res = pthread_cond_init(&fgate->m_waitingCondition, NULL);
  res |= pthread_mutex_init(&fgate->m_mutex, NULL);
  if (res != 0)
  {
    return NULL;
  }

  fgate->m_threadCapacity = _capacity;
  fgate->m_threadsWaitingEntry = 0;
  fgate->m_magicNum = WEASLEY;
  fgate-> m_numOfItemsInside = 0;
  return fgate;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void FloodGate_Destroy(Floodgate* _fgate)
{
  int res;
  if ((NULL == _fgate) && (WEASLEY != _fgate->m_magicNum))
  {
    return;
  }

  BarrierDestroy(&_fgate->m_firstBarrier); /* using the barrier function to destroy the barriers */
  BarrierDestroy(&_fgate->m_secondBarrier);

  res = pthread_cond_destroy(&_fgate->m_waitingCondition);
  res |= pthread_mutex_destroy(&_fgate->m_mutex);
  if (res != 0)
  {
    return;
  }

  free (_fgate);
return;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

int FloodGate_Flow(Floodgate* _fgate, void(*_ThreadFunc)(void*),void* _context)
{
  int res;
  if ((NULL == _fgate) || (NULL == _ThreadFunc))
  {
    return BAD_INPUT;
  }
  res = pthread_mutex_lock (&_fgate->m_mutex);

  while (( _fgate->m_threadsWaitingEntry > _fgate->m_threadCapacity) || (_fgate->m_numOfItemsInside > 0))
  {
    res |= pthread_cond_wait(&_fgate->m_waitingCondition, &_fgate->m_mutex);
  }

  ++(_fgate->m_threadsWaitingEntry);
  res |= pthread_mutex_unlock (&_fgate->m_mutex);

  /* -------------FIRST BARRIER-------------*/


  BarrierWait(&_fgate->m_firstBarrier);                 /* here w're at the first barrier */
  res |= pthread_mutex_lock (&_fgate->m_mutex);

  --(_fgate->m_threadsWaitingEntry);                    /* update number in and out of barrier */
  ++(_fgate->m_numOfItemsInside);

  res |= pthread_mutex_unlock (&_fgate->m_mutex);
  _ThreadFunc(_context);                                /* run the thread's function between the barriers */


  /* NEED TO ADD IN SUMMARIZER THAT RETURNS COUNT (from first barrier?) */

  /* -------------SECOND BARRIER-------------*/

  BarrierWait(&_fgate->m_secondBarrier);                /* here w're at the second barrier */
  res |= pthread_mutex_lock (&_fgate->m_mutex);

  --(_fgate->m_numOfItemsInside);                       /* update num threads inside barrier */

  if (_fgate->m_numOfItemsInside == 0)
  {
    res |= pthread_cond_broadcast(&_fgate->m_waitingCondition);
  }

  res |= pthread_mutex_unlock (&_fgate->m_mutex);
  if (res != 0)
  {
    return -1;
  }

  return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
