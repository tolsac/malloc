/*
** malloc.c for  in /home/tolsa_c//projets/malloc/malloc_simple
** 
** Made by camille tolsa
** Login   <tolsa_c@epitech.net>
** 
** Started on  Wed Jan 25 12:02:37 2012 camille tolsa
** Last update Tue Sep 23 02:41:54 2014 root
*/
#include "malloc.h"

static t_malloc s = {NULL};
static uint	cursor = {-1};
static pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;

void		decreaseBreak(uint pow)
{
  void		*Bpos;

  pthread_mutex_lock(&mutex);
  Bpos = sbrk(0);
  s.list = s.freeMap[pow];
  if (s.freeMap[pow] != NULL)
    {
      while (s.list->next != NULL)
	{
	  if ((s.list + s.list->size) == Bpos)
	    {
	      sbrk(s.list->size * -1);
	      cursor -= s.list->size;
	    }
	  s.list = s.list->next;
	}
    }
  pthread_mutex_unlock(&mutex);
}

void		show_alloc_mem()
{
  uint		i;

  i = 0;
  printf("break : %p\n", sbrk(0));
  while (i < 32)
    {
      s.list = s.mallocMap[i];
      if (s.mallocMap[i] != NULL)
	{
	  while (s.list != NULL)
	    {
	      printf("%p - %p", s.list, (s.list + s.list->size));
	      printf(" : %lu octets\n", s.list->size);
	      s.list = s.list->next;
	    }
	}
      i++;
    }
}

void		addMalloc(uint pow, t_data* ptr)
{
  pthread_mutex_lock(&mutex);
  ptr->next = s.mallocMap[pow];
  s.mallocMap[pow] = ptr;
  pthread_mutex_unlock(&mutex);
}

void		addFree(uint pow, t_data* ptr)
{
  pthread_mutex_lock(&mutex);
  ptr->next = s.freeMap[pow];
  s.freeMap[pow] = ptr;
  pthread_mutex_unlock(&mutex);
}

void		removeMalloc(uint pow, t_data* ptr)
{
  pthread_mutex_unlock(&mutex);
  s.list = s.mallocMap[pow];
  if (s.list != NULL)
    {
      while (s.list->next != NULL && s.list->next != ptr)
	s.list = s.list->next;
      if (s.list->next == NULL)
	s.list = NULL;
      else
	s.list->next = s.list->next->next;
    }
  pthread_mutex_unlock(&mutex);
}

void		removeFree(uint pow)
{
  pthread_mutex_lock(&mutex);
  if (s.freeMap[pow] != NULL)
    s.freeMap[pow] = s.freeMap[pow]->next;
  pthread_mutex_unlock(&mutex);
}

uint		getPow(uint x)
{
  uint i;
  uint p;

  i = 0;
  p = 1;
  while (p < x)
    {
      p *= 2;
      i++;
    }
  return (i);
}

uint		getSize(uint x)
{
  uint		nb;

  nb = 2;
  while (x != 1)
    {
      nb *= 2;
      x--;
    }
  return (nb);
}

void		free(void *ptr)
{
  pthread_mutex_lock(&mutex);
  if (ptr != NULL)
    {
      ((t_data*)ptr - 1)->next = NULL;
      removeMalloc(getPow(((t_data*)ptr - 1)->size + 16), (t_data*)ptr - 1);
      addFree(getPow(((t_data*)ptr - 1)->size + 16), (t_data*)ptr - 1);
      decreaseBreak(getPow(((t_data*)ptr - 1)->size + 16));
    }
  pthread_mutex_unlock(&mutex);
}

void		*getMem(uint size)
{
  s.list = s.freeMap[getPow(size)];
  if (s.list == NULL)
    {
      return (NULL);
    }
  else
    removeFree(getPow(size));
  return (s.list);
}

void		*formatHeader(uint si, uint size, t_data *data, uint mode)
{
  void		*ptr;

  if (mode == 0)
    data = sbrk(0) - cursor;
  else
    {
      s.j = (si / getpagesize()) + 1;
      cursor = s.j * getpagesize();
      data = sbrk(cursor);
    }
  cursor -= si;
  data->size = size;
  ptr = (void*)data;
  addMalloc(getPow(si), data);
  ptr += sizeof(t_data);
  return (ptr);
}

void		*malloc(uint size)
{
  void		*ptr;
  t_data	*data;
  uint		si;
  int tot;

  pthread_mutex_lock(&mutex);
  ptr = NULL;
  si = getSize(getPow(size + sizeof(t_data)));
  if ((data = getMem(si)) != NULL)
    return (data + 1);
  else
    tot = cursor - si;
  if (cursor != 0)
    if (tot > 0)
      {
	pthread_mutex_unlock(&mutex);
	return (formatHeader(si, size, data, 0));
      }
    else
      {
	pthread_mutex_unlock(&mutex);
	return (formatHeader(si, size, data, 1));
      }
  return (NULL);
}

void		*realloc(void *src, uint size)
{
  void		*d;

  pthread_mutex_lock(&mutex);
  if (src == NULL)
    {
      if (size > 0)
	return (malloc(size));
      pthread_mutex_unlock(&mutex);
      return (NULL);
    }
  if (size < ((t_data*)src - 1)->size)
    {
      pthread_mutex_unlock(&mutex);
      return (src);
    }
  d = malloc(size);
  if (size >= ((t_data*)src - 1)->size)
    d = memcpy(d, src, ((t_data*)src - 1)->size);
  free(src);
  pthread_mutex_unlock(&mutex);
  return (d);
}
