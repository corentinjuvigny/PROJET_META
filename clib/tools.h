#ifndef __TOOLS__
#define __TOOLS__

#include <stdlib.h>

/*======================================================================*/
/*= memory allocation and freeing                                      =*/
/*=                                                                    =*/
/**
 * The xmalloc function is similar to malloc but prints a fatal
 * message using the tool_spf_printf function when the memory
 * allocation failed.
 *
 * The xcalloc function is similar to tool_malloc but fills
 * up all the bytes of the memory block with low value ('\0').
 *
 * The xfree macro frees the memory location pointed to
 * by the p pointer if the p pointer is not NULL. Otherwise
 * it does nothing.
**/


extern void* xmalloc(size_t n);
extern void* xcalloc(size_t nmemb,size_t size);
#define xfree(p) do { if ( (p)!= 0 ) free(p); } while(0)

#endif
