#ifndef ADD_LINE_HIERARCHY_H
#define ADD_LINE_HIERARCHY_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

/*
  Module utilis� pour la gestion d'une ligne m�moire au sein de la hi�rarchie de caches
  G�re lecture/�criture de cette ligne dans l'ensemble des caches.
*/

void add_line(struct list *, int, int);

#endif
