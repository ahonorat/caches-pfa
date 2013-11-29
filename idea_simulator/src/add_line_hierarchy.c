#include "add_line_hierarchy.h"

/* Return 1 if there is entry is somewhere else in level
   Used for a miss when load or a hit when store */
int share_level(struct list *level, struct cache *cache, int entry, void (*action)(struct line *)) {
  struct list *current = level;
  struct line *line;
  int res = 0;
  while (current != NULL){
    if (current->cache != cache){
      if (is_in_cache(current->cache, entry)) {
	line = line_in_cache(current->cache, entry);
	
	if (is_modified(line)) {
	  action(line);
	  current->cache->writes_back++;
	  return 1;
	}
	if (current->cache->flags(line, action)) {
	  return 1;
	}

	if (is_shared(line)) {
	  action(line);
	  res = 1;
	}
      }
    }
    current = current->next;
  }
  return res;
}

void load_line_hierarchy(struct list **levels, struct list *cache, int entry) {

  /* Hit: Ok! */
  if (is_in_cache(cache->cache, entry)) {
    update_lines(cache->cache, entry);
    cache->cache->hits++;
  }

  /* Miss:
     if data is not in any cache: Exclusive
     else Shared, as other values in caches
  */
  else {
    int res = 0;
    int v;
    struct list *current_list = cache;
    struct list *current_level = levels[0];
    struct cache *current_cache;
    struct line *line = NULL;
    
    while (res == 0 && current_list != NULL) {
      current_cache = current_list->cache;

      res = add_line_cache(current_cache, entry, 0);
      update_lines(current_cache, entry);
      
      v = share_level(current_level, current_cache, entry, &share_line);
      current_cache->broadcasts++;

      /* If line was previously in the cache, keep it as it was! */
      if (res == 0) {
	line = line_in_cache(current_list->cache, entry);
	current_list->cache->flags_new_line(v, line);
      }
            
      current_level = current_level->next;
      current_list = current_list->next;
    }    
  }
}

/* Warning: caches are supposed to be inclusive -> if store L1 then store L2, L3... */
void store_line_hierarchy(struct list **levels, struct list *cache, int entry) {
  struct line *line = NULL;
  struct list *current_list = cache;
  struct list *current_level;
  struct cache *current_cache;
  int i;

  /* Hit: Ok! */
  if (is_in_cache(cache->cache, entry)) {
    line = line_in_cache(current_list->cache, entry);
    modify_line(line);
    cache->cache->hits++;
    
    i = 0;
    while (current_list != NULL) {
      current_level = levels[i++];
      current_cache = current_list->cache;
      line = line_in_cache(current_cache, entry);
      modify_line(line);
      share_level(current_level, current_cache, entry, &invalid_line);
      current_cache->broadcasts++;
      current_list = current_list->next;
    }    
  }
  
  /* Miss:
     if data is not in any cache: Exclusive
     else Shared, as other values in caches
  */
  else {
    int res = 0;
    i = 0;
    current_list = cache;
    
    while (res == 0 && current_list != NULL) {
      current_level = levels[i++];
      current_cache = current_list->cache;
      res = add_line_cache(current_cache, entry, 1);

      share_level(current_level, current_cache, entry, &invalid_line);
      current_cache->broadcasts++;
      line = line_in_cache(current_list->cache, entry);
      modify_line(line);
            
      current_list = current_list->next;
    }    
  }
  update_lines(cache->cache, entry);
}
