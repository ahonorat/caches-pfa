#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "block.h"

#define ARCH 64

/*
  Module utilis� pour la gestion d'un cache.
  G�re lecture/�criture au sein du cache.
  G�re la m�moire et les stats relatives � ce cache.
*/

struct cache {
  int size;
  int linesize; // linesize = ARCH ?
  int nb_ways;
  int nb_blocks;
  struct block **blocks;

  int misses;
  int hits;
  int writes;  
  int writes_back;
};

/* Data allocations */
struct cache *init_cache(int, int, int, int);

/* Data removal */
void delete_cache(struct cache *);

int block_id(struct cache *, int);
int is_in_cache(struct cache *, int);
int add_line_cache(struct cache *, int, int);

void print_infos(struct cache *);

struct line *line_in_cache(struct cache *, int);

#endif
