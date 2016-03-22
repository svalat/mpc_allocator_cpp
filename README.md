# MPC Allocator C++

This is a reimplemenation of the C version of MPC\_Allocator embeded into the MPC framework (http://mpc.hpcframework.paratools.com/).
This memory allocator provided :

 * Support of NUMA architecture. For binded threads it automatically isolate the data transfers between each NUMA nodes.
 * Large buffer caching and reuse. For HPC application it is an issue to return too much the memory to the OS due to performance issue,
 MPC allocator reuse them as much as possible with some caching technics and mremap usage to avoid large fragmentation.
 * Isolation of thread sub-allocators. Each thread run his own lock free allocator.
 * Manage remote free (returning a segment to another thread) with a dedicated lock-free list.
 * Management of small chunk uses the same approach than Jemalloc (http://www.canonware.com/jemalloc/) with bitfield headers and size segregation.
 * Medium chunks are handled by segregeted double linked list for fast merging.
 
Due to it's deseign this allocator might by default consume more memory to prevend to much exchange with the OS. This can be controled by.... [[FIXME]]...

**WARNING**: This version is not yet fully stable, you can get the C stable version from MPC (http://mpc.hpcframework.paratools.com/) 
looking into mpcframework/MPC_Allocator. It can be built outside of MPC.

## Detailed research documentation

If you want more details about the research work behind this allocator you can read my PhD. :

 * PhD. manuscript (in french) : https://tel.archives-ouvertes.fr/tel-01253537
 * PhD. defense slides (in english) : http://fr.slideshare.net/SbastienValat1/2014-valatphddefenseslides
 * A reserch paper about page zeroing performance issue : http://dl.acm.org/citation.cfm?id=2492414&CFID=764041395&CFTOKEN=78061783

## Other parallel allocators

If you search other good parallel memory allocator I studied for this work :

 * Jemalloc (http://www.canonware.com/jemalloc/)
 * TCMalloc (https://github.com/gperftools/gperftools)
 * Lokless allocator (http://locklessinc.com/downloads/)
 * Hoard (http://www.hoard.org/)
