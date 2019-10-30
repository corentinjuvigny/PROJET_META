#include <stdlib.h>
#include <stdio.h>

extern void* xmalloc(size_t n)
{
  void* ptr = malloc(n);
  if (ptr == NULL)
    fprintf(stderr, "Error not enough memory\n");
  return ptr;
}

extern void* xcalloc(size_t nmemb,size_t size)
{
  void* ptr = calloc(nmemb,size);
  if (ptr == NULL)
    fprintf(stderr, "Error not enough memory\n");
  return ptr;
}
