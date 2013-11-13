#include <stdio.h>
#include <stdlib.h>

/*
  Module utilis� pour communiquer avec Hwloc et ainsi d�finir
  l'architecture se laquelle on se place.  Lie les diff�rents caches
  pour les communications utl�rieures.  Les allocations de
  blocs/lignes seront r�alis�es par les caches eux-m�mes.
*/

/*
  get_depth()
  add_cache(depth, links, size, line size, associativity)
*/
