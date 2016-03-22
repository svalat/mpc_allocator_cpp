/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef MPC_ALLOCATOR_C_API_H
#define MPC_ALLOCATOR_C_API_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Force allocation on the given NUMA node. On non NUMA architecture it will fallback onto 
 * standard allocator.
 * @param numa_id ID of the numa node to target.
 * @param size Size of the memory allocation to perform.
**/
void * mpc_numa_alloc(int numa_id, size_t size);

/**
 * To be used if you move your thread to another NUMA node after doing some memory allocations.
 * It also ensure that the future allocation will be performed with the good memory source.
**/
void mpc_alloc_numa_migrate(void);

#ifdef __cplusplus
}
#endif

#endif //MPC_ALLOCATOR_C_API_H
