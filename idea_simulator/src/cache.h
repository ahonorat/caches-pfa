#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
  Module utilis� pour la gestion d'un cache.
  G�re lecture/�criture au sein du cache.
  G�re la m�moire et les stats relatives � ce cache.
*/

struct cache {
  int size;
  int linesize;
  int nb_ways;
  int nb_blocks;
  struct block **blocks;
};

struct block {
  int nb_ways;
  struct line **lines;
};

struct line {
  int label;
  int first_case;
  int valid;
};

struct cache *init_cache(int, int, int, int);
struct block **init_block(int, int);
struct line** init_line(int);

void delete_cache(struct cache *);
void delete_blocks(struct block **, int);
void delete_lines(struct line **, int);

int get_size(struct cache *);
