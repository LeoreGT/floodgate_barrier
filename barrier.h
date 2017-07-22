#include <stdlib.h>  /* size_t       */
#include <pthread.h> /* pthread work */

/**
 * @brief : functions dealing with the global barrier struct in charge of
 *          receiving threads and either putting them into a wait state or
 *          waking up everyone if all threads have arrived at the wait state
 *
 * @author: Leore Golan
 * @date  : 06/05/2017
 */

typedef struct Barrier Barrier;

struct Barrier
{
  pthread_mutex_t m_mutex;
  pthread_cond_t m_cond;
  int m_condition;
  int m_threadsBarrier1;
  int m_counter;
  int m_numLoops;
};

/**
 * @brief : initializes elements of global barrier struct, sets total threads to given count
 * @params _barrier : the global barrier struct holding the mutex, thread count, etc.
 * @return : 0 upon SUCCESS, 1 upon FAILURE if pthread initialization didn't work
 */
int BarrierInit(Barrier* _barrier, unsigned int _totalThreads);

/**
 * @brief : increments the number of waiting threads, if last thread arrived, broadcasts and opens barrier
 * @params _barrier : the global barrier struct holding the mutex, thread count, etc.
 * @return : STILL_WAITING if not all threads have arrived, ALL_DONE if they have, FAILURE if pthread funcs fail
 */
int BarrierWait(Barrier* _barrier);

/**
 * @brief : destroys the mutex and conditional variable of the global barrier struct
 * @params _barrier : the global barrier struct holding the mutex, thread count, etc.
 * @return : 0 upon SUCCESS, 1 upon FAILURE if pthread destruction didn't work
 */
int BarrierDestroy(Barrier* _barrier);
