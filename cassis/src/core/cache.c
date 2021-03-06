/*
 * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
 * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * \file cache.c
 * \brief Manage a block : initiate, removal, print data, coherence and replacement policy.
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 *
 * 
 *
 */

#include "option.h"
#include "cache.h"
#include "trace.h"

void up_stat(struct cache *cache, unsigned long entry, int stats_type) {
  unsigned int i;

  /* printf("%d ", instruction_number); */	
  for (i = 0; i < tracking_count; i++) {
    if (i == 0 || ((tracking_type == TRACKING_BOUND && tracking_lower_bound[i] <= entry && entry <= tracking_upper_bound[i]) || 
		   (tracking_type == TRACKING_INSTRUCTION && is_instr_tracked(instruction_number)) ||
		   (tracking_type == TRACKING_BOUND + TRACKING_INSTRUCTION && tracking_lower_bound[i] <= entry && entry <= tracking_upper_bound[i] 
		    && is_instr_tracked(instruction_number)) || tracking_type == TRACKING_DISABLED)) {
      switch (stats_type) {
      case MISS:
	cache->misses[i]++;
	break;
      case HIT:
	cache->hits[i]++;
	break;
      case WRITE_BACK:
	cache->writes_back[i]++;
	break;
      case COHERENCE_EVINCTION:
	cache->evincted_coherence[i]++;
	break;
      case CAPACITY_EVINCTION:
	cache->evincted_capacity[i]++;
	break;
      case TYPES_EVINCTION:
	cache->evincted_caches_types[i]++;
	break;
      case VALUE_BY_SNOOPING:
	cache->misses_snooping[i]++;
	break;
      case VALUE_ABOVE:
	cache->misses_above[i]++;
	break;
      case VALUE_BELOW:
	cache->misses_below[i]++;
	break;
      case COHERENCE_BROADCAST:
	cache->broadcast_coherence[i]++;
	break;
      case SNOOPING_BROADCAST:
	cache->broadcast_snooping[i]++;
    	break;
      }
    }
  }
}

/* Data allocations */
struct cache* init_cache(int size, int linesize, int nb_ways, int nb_blocks, int depth, void (*replace)(struct cache *), enum cache_coherence policy, int type, bool snooping, bool directory) {
  struct cache *cache   = malloc(sizeof(struct cache));
  assert(cache!=NULL);
  cache->size           = size;
  cache->linesize       = linesize;
  cache->nb_ways        = nb_ways;
  cache->nb_blocks      = nb_blocks;
  struct block **blocks = init_block(nb_blocks, nb_ways, linesize, policy);
  cache->blocks         = blocks;
  unsigned int i;
  for (i = 0; i < tracking_count; i++) {
    cache->misses[i]                = 0;
    cache->hits[i]                  = 0;
    cache->writes_back[i]           = 0;
    cache->evincted_coherence[i]    = 0;
    cache->evincted_capacity[i]     = 0;
    cache->evincted_caches_types[i] = 0;
    cache->misses_snooping[i]       = 0;
    cache->misses_above[i]          = 0;
    cache->misses_below[i]          = 0;
    cache->broadcast_coherence[i]   = 0;
    cache->broadcast_snooping[i]    = 0;
  }
  cache->depth          = depth;
  cache->type           = type;
  cache->snooping       = snooping;
  cache->directory      = directory;
  cache->policy         = policy;
  replace(cache);
  return cache;
}

/* Data removal */
void delete_cache(struct cache *cache) {
  delete_blocks(cache->blocks, cache->nb_blocks);
  free(cache);
}

int block_id(struct cache *cache, unsigned long entry) {
  entry = entry / cache->linesize; //Line in principal memory
  int id_block = entry % cache->nb_blocks;
  return id_block;
}

int is_in_cache(struct cache *cache, unsigned long entry) {
  int id_block = block_id(cache, entry);
  struct block *block = cache->blocks[id_block];
  int nb_ways = cache->nb_ways;

  struct line *line;
  int i;
  for (i=0; i<nb_ways; i++) {
    line = block->lines[i];
    if (is_valid(line) && (line->first_case == entry / cache->linesize * cache->linesize)) {
      return 1;
    }
  }    
  return 0;
}

struct line *line_in_cache(struct cache *cache, unsigned long entry) {
  int id_block = block_id(cache, entry);
  struct block *block = cache->blocks[id_block];
  int nb_ways = cache->nb_ways;

  struct line *line;
  int i;
  for (i=0; i<nb_ways; i++) {
    line = block->lines[i];
    if (line->first_case == entry / cache->linesize * cache->linesize) {
      return line;
    }
  }
  fprintf(stderr, "Error: data not present is an inclusive cache\n");
  exit(1);
}

void update_lines(struct cache *cache, unsigned long entry) {
  int id_block = block_id(cache, entry);
  struct block *block = cache->blocks[id_block];
  int nb_ways = cache->nb_ways;
  cache->update_line(block, nb_ways, entry);
}

void replacement_LFU(struct cache *cache) {
  cache->replacement = id_line_to_replace_LFU;
  cache->update_line = update_LFU;
}

void replacement_FIFO(struct cache *cache) {
  cache->replacement = id_line_to_replace_FIFO;
  cache->update_line = update_FIFO;
}

void replacement_LRU(struct cache *cache) {
  cache->replacement = id_line_to_replace_LRU;
  cache->update_line = update_LRU;
}

bool is_cache_inclusive(struct cache *cache){
  return (cache->type == Inclusive);
}

bool is_cache_exclusive(struct cache *cache){
  return (cache->type == Exclusive);
}

bool is_inclusive_like(struct cache *cache){
  return (cache->type == Inclusive || cache->type == NIIO);
}

bool is_snooping(struct cache *cache){
  return cache->snooping;
}

