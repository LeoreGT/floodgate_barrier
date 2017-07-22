#ifndef __FLOODGATE_H__
#define __FLOODGATE_H__

typedef struct Floodgate Floodgate;

/**
 * @brief : initializes a floodgate using the given capacity (of threads) and creating two barriers
 * @params _capacity : the given number of threads
 * @return : the newly initialized floodgate structure
 */
Floodgate* FloodGate_Init(size_t _capacity);

/**
 * @brief : destroys the given floodgate struct, including the two barriers using BarrierDestroy()
 * @params _fgate: the floodgate struct to be destroyed
 */
void FloodGate_Destroy(Floodgate* _fgate);

/**
 * @brief : holds two barriers and manages them to open when the appropriate number of threads pass through
 * @params _fgate : the floodgate structure holding the two barriers
 * @params *_ThreadFunc : function giving threads the desired action
 * @params _context : context to be passed to the given thread function
 * @return : 0 upon success, -1 upon failure
 */
int FloodGate_Flow(Floodgate* _fgate, void(*_ThreadFunc)(void*), void* _context);


#endif /*__FLOODGATE_H__ */


