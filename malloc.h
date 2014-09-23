/*
** malloc.h for  in /home/tolsa_c//projets/malloc/malloc_simple
** 
** Made by camille tolsa
** Login   <tolsa_c@epitech.net>
** 
** Started on  Wed Jan 25 14:09:33 2012 camille tolsa
** Last update Sun Feb  5 19:27:02 2012 camille tolsa
*/
#ifndef __MALLOC__
#define __MALLOC__

#include <unistd.h>
#include <pthread.h>

#define error (void*)-1

typedef short map_type;
typedef size_t uint;

typedef struct s_data
{
  uint			size;
  struct s_data		*next;
}t_data;

typedef struct s_malloc
{
  t_data		*mallocMap[32];
  t_data		*freeMap[32];
  t_data		*list;
  uint			j;
}t_malloc;

void decreaseBreak(uint);
void addMalloc(uint, t_data*);
void addFree(uint, t_data*);
void removeMalloc(uint, t_data*);
void removeFree(uint);
void *formatHeader(uint, uint, t_data *, uint);
int  printf(const char *, ...);
void *memcpy(void*, void*, uint);
void *getMem(uint);
uint getSize(uint);
uint getPow(uint);
void *realloc(void *, uint);
void *malloc(uint);
void free(void *);

#endif
