/*
 * Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
 * Alexandre Honorat, Gilles Marait, Grégoire Pichon.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * \file test_replacement.c
 * \brief Test the replacement policies
 * \author ~gpichon
 * \version 1.0
 * \date 3rd january 2014
 * 
 * 
 *
 */

 #include "test_replacement.h"

int test_replacement(int argc, char** argv) {
  (void) argc;
  (void) argv;
  
  struct node **threads = malloc(1 * sizeof(struct node *));
  struct cache *L1;
  struct node *node = init_node();
  threads[0] = node;
  int i;

  /* Test of Least Frequently Used replacement */
  /* Pour version sans automates: MESI -> &coherence_MESI */
  L1 = init_cache(8192, 64, 4, 32, 0, &replacement_LFU, MESI, Inclusive, false, false);
  node->data = L1;

  /* There are four ways in each block. As adress 163 is more used, it is not deleted from the cache */
  for (i=1; i<3; i++) {
    load_line_hierarchy(threads[0], 163+2048);
  }

  for (i=1; i<5; i++) {
    load_line_hierarchy(threads[0], 163+2048*i);
  }

  assert(!is_in_cache(L1, 163));
  assert(is_in_cache(L1, 163+2048));
  for (i=2; i<5; i++, 0) {
    assert(is_in_cache(L1, 163+2048*i));
  }

  delete_cache(L1);
  printf("Tests LFU OK\n");

  /* Test of First In First Out replacement */
  L1 = init_cache(8192, 64, 4, 32, 0, &replacement_FIFO, MESI, Inclusive, false, false);
  node->data = L1;

  /* There are four ways in each block. The deleted value in the first one in the cache, whatever how many times it was used */

  for (i=1; i<3; i++) {
    load_line_hierarchy(threads[0], 163);
  }

  for (i=1; i<6; i++) {
    load_line_hierarchy(threads[0], 163+2048*i);
  }

  assert(!is_in_cache(L1, 163));
  assert(!is_in_cache(L1, 163+2048));
  for (i=2; i<6; i++, 0) {
    assert(is_in_cache(L1, 163+2048*i));
  }

  delete_cache(L1);
  printf("Tests FIFO OK\n");


  /* Test of Last Recently Used replacement */
  L1 = init_cache(8192, 64, 4, 32, 0, &replacement_LRU, MESI, Inclusive, false, false);
  node->data = L1;

  for (i=1; i<3; i++) {
    load_line_hierarchy(threads[0], 163);
  }

  for (i=1; i<6; i++) {
    load_line_hierarchy(threads[0], 163+2048*i);
  }

  for (i=1; i<6; i++) {
    load_line_hierarchy(threads[0], 163+2048*(5-i));
  }

  assert(is_in_cache(L1, 163));
  assert(is_in_cache(L1, 163+2048));
  for (i=2; i<4; i++, 0) {
    assert(is_in_cache(L1, 163+2048*i));
  }
  for (i=4; i<6; i++, 0) {
    assert(!is_in_cache(L1, 163+2048*i));
  }

  delete_cache(L1);
  printf("Tests LRU OK\n");

  free(node);
  free(threads);
  return EXIT_SUCCESS;
}
