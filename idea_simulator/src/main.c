#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "add_line_hierarchy.h"

int main(int argc, char *argv[]) {

  struct list **caches = NULL;
  int nb_threads = 4;

  caches = malloc(nb_threads * sizeof(struct list *));

  int i;
  for (i=0; i<4; i++) {
    struct cache *cache;
    cache = init_cache(8192, 64, 4, 32);
    caches[i] = init_list(cache);
  }

  struct cache *cache_L2_0, *cache_L2_1, *cache_L3;
  cache_L2_0 = init_cache(32768, 64, 8, 64);	  
  cache_L2_1 = init_cache(32768, 64, 8, 64);	  
  cache_L3 = init_cache(130172, 64, 16, 128);
  
  for (i=0; i<2; i++) {
    add_list(caches[i], cache_L2_0);
    add_list(caches[i+2], cache_L2_1);
  }
  for (i=0; i<4; i++) {
    add_list(caches[i], cache_L3);
  }

  /* Classics loads */
  load_line_hierarchy(caches, nb_threads, caches[0], 163+2048, 0); /* Miss L1_0, L2_0, L3_0 */
  load_line_hierarchy(caches, nb_threads, caches[1], 163+2048, 0); /* Miss L1_1 Hit L2_0 */
  load_line_hierarchy(caches, nb_threads, caches[2], 163+2048, 0); /* Miss L1_2, L2_1 Hit L3_0 */

  /* Store value in cache -> Hit */
  /* Others caches are invalidated */
  store_line_hierarchy(caches, nb_threads, caches[0], 163+2048);   /* Hit L1_0 */

  /* Invalid line in cache -> miss. Modified copy in cache -> WB + update data*/
  load_line_hierarchy(caches, nb_threads, caches[1], 163+2048, 0); /* WB L1_0 Miss L1_1, L2_0, L3_0 */

  /* Data was updated with last load */
  load_line_hierarchy(caches, nb_threads, caches[0], 163+2048, 0); /* Hit L1_0 */

  /* Invalidated caches */
  store_line_hierarchy(caches, nb_threads, caches[2], 163+2048);   /* Miss L1_2, L2_1 Hit L3_0 */


  /* Informations about caches */
  for (i=0; i<4; i++) {
    fprintf(stdout, "L1 %d:\n", i);
    print_infos(caches[i]->cache);
  }
  
  for (i=0; i<2; i++) {
    fprintf(stdout, "L2 %d:\n", i);
    print_infos(caches[2*i]->next->cache);
  }

  fprintf(stdout, "L3:\n");
  print_infos(caches[0]->next->next->cache);

  delete_cache(cache_L2_0);
  delete_cache(cache_L2_1);
  delete_cache(cache_L3);

  for (i=0; i<4; i++) {
    delete_cache(caches[i]->cache);
    free(caches[i]->next->next);
    free(caches[i]->next);
    free(caches[i]);
  }

  free(caches);
  return EXIT_SUCCESS;
}
