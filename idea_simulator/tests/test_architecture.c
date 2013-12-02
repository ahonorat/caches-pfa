#include <stdio.h>
#include "../src/architecture.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <assert.h>

#define FILENAME "data/architest.xml"
#define OUTFILE "data/architest_parsed.xml"

int main(int argc, char** argv){
  struct architecture A;
  if(argc == 2){
    A = parse_archi_file(argv[1], 1);
  }
  else{
    A = parse_archi_file(FILENAME, 1);
  }

  print_archi(&A);

  delete_archi(&A);
  return EXIT_SUCCESS;
}
