#include "architecture.h"
#include "list.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <string.h>

#define GET_NUMBER(inte,name) 	getAttribute(n, name, a_value);	\
  inte = atoi(a_value)

#define CHECK_XPATH(result) do { if (result == NULL) { fprintf(stderr, "Error XPath request\n"); \
				   return EXIT_FAILURE; } } while(0)
/*
TODO :
options : 
- parse HWLOC -> archi file and execute
- parse HWLOC -> archi file only
- use archi file and execute
*/


//Get the value of the attribute "name" in node n
void getAttribute(xmlNode * n, char * name, char * dest){
  xmlChar * a = xmlGetNoNsProp(n, (xmlChar *) name);
  dest[0] = 0;
  if(a == NULL){
    dest = NULL;
  }
  else{
    sprintf(dest, "%s", a);
    xmlFree(a);
  }
}

void prefix_search(xmlNodePtr node, struct architecture * archi, struct cache *** const cstackptr, int stack_head) {
  xmlNodePtr n = node;
  xmlAttr * attr;
  int i;
  int depth = 1;
  struct cache * c;
  int size, linesize, nb_ways, nb_blocks;
  static char a_name[50], a_value[50];

  while(n != NULL){
    attr = n->properties;
    while(attr != NULL){
      //Some architecture information
      getAttribute(n, (char *) attr->name, a_name);
      if(strcmp((char *) attr->name, "name") == 0){
	getAttribute(n, "value", a_value);
	if(strcmp(a_name, "Architecture") == 0){
	  sprintf(archi->name, "%s", a_value);
	}
	else if(strcmp(a_name, "CPUModel") == 0){
	  sprintf(archi->CPU_name, "%s", a_value);
	}
      }
      //Add a cache
      else if(strcmp((char *) attr->name, "type") == 0 && strcmp((char *) a_name, "Cache") == 0){
	getAttribute(n, "cache_type", a_value);
	if(a_value != NULL && atoi(a_value) <= 1){ //verify that the cache is not an L1i
	  GET_NUMBER(depth,"depth");
	  if(depth > archi->number_levels){
	    archi->number_levels = depth;
	    archi->levels = calloc(archi->number_levels, sizeof(struct list *));
	    *cstackptr = calloc(archi->number_levels, sizeof(struct cache *));
	  }
	  GET_NUMBER(size, "cache_size");
	  GET_NUMBER(linesize, "cache_linesize");
	  GET_NUMBER(nb_ways, "cache_associativity");
	  nb_blocks = size / (linesize * nb_ways);
	  /* Default policy: LFU and MESI*/
	  c = init_cache(size, linesize, nb_ways, nb_blocks, depth, &replacement_LFU, &coherence_MESI);
	  //Add the cache to the levels table
	  if(archi->levels[depth-1] == 0){
	    archi->levels[depth-1] = init_list(c);
	  }
	  else{
	    add_list(archi->levels[depth-1], c);
	  }
	  //Add to the threads table
	  //First pop levels below in the stack
	  while(stack_head > 0 && (*cstackptr)[stack_head]->depth <= depth){
	    stack_head--;
	  }
	  //Then we push
	  stack_head++;
	  (*cstackptr)[stack_head] = c;

	  if(depth == 1){
	    archi->threads = realloc(archi->threads, (archi->number_threads + 1) * sizeof(struct list *));
	    archi->threads[archi->number_threads] = init_list(c);
	    for(i = stack_head-1; i >= 0 ; i--){
	      add_list(archi->threads[archi->number_threads], (*cstackptr)[i]);
	    }
	    archi->number_threads++;
	  }
	}
      }
      attr = attr->next;
    }
    prefix_search(n->children, archi, cstackptr, stack_head);
    n=n->next;
  }
}

struct architecture parse_archi_file(const char * filename){
  
  struct architecture archi;
  //Default values
  archi.nb_bits = 64;
  strcpy(archi.name, "unknown");
  strcpy(archi.CPU_name, "unknown");
  archi.number_threads = 0;
  archi.number_levels = 0;
  archi.threads = NULL;
  archi.levels = NULL;

  xmlKeepBlanksDefault(0);
  xmlDocPtr xmlfile = xmlParseFile(filename);
  if(xmlfile == NULL){
    fprintf(stderr, "Could not load %s\n", filename);
  }

  xmlNode * root = xmlDocGetRootElement(xmlfile);
  if (root == NULL) {
    fprintf(stderr, "Empty XML file\n");
  }

  struct cache ** cstack = NULL;
  struct cache *** cstackptr = &cstack;
  int stack_head = -1;

  prefix_search(root, &archi, &cstack, stack_head);
  
  cstack = *cstackptr;
  xmlFreeDoc(xmlfile);
  xmlCleanupParser();

  free(cstack);

  return archi;
}

void print_archi(struct architecture * archi){
  int i;
  struct list * l;
  printf("Architecture %d bits : %s\n", archi->nb_bits, archi->name);
  printf("CPU model : %s\n", archi->CPU_name);
  printf("Liste des threads : \n");
  for(i=0;i<archi->number_threads;i++){
    l = archi->threads[i];
    printf("Pour thread %d\n", i+1);
    while(l != NULL){
      printf("\tL%d (taille : %d, ligne : %d, associativité : %d, nb_blocks : %d)\n", l->cache->depth, l->cache->size, l->cache->linesize, l->cache->nb_ways,  l->cache->nb_blocks);
      l = l->next;
    }
  }
  printf("\n\nListe des niveaux\n");
  for(i=0;i<archi->number_levels;i++){
    l = archi->levels[i];
    printf("Pour niveau %d\n", i+1);
    while(l != NULL){
      printf("\tL%d (taille : %d, ligne : %d, associativité : %d, nb_blocks : %d)\n", l->cache->depth, l->cache->size, l->cache->linesize, l->cache->nb_ways,  l->cache->nb_blocks);
      l = l->next;
    }
  }
}

int print_archi_xml(struct architecture * archi, char * file_in, char * file_out){
  int i = 0;
  FILE * out = fopen(file_out,"w");
  if(out == NULL){
    fprintf(stderr, "Cannot open %s\n", file_out);
    return EXIT_FAILURE;
  }

  xmlXPathInit();
  xmlNodePtr root_cache;
  xmlNodePtr cur;
  xmlDocPtr doc = xmlParseFile(file_in);
  xmlDocPtr fin_doc = xmlNewDoc(BAD_CAST "1.0");
  xmlXPathContextPtr context = xmlXPathNewContext(doc);
  if (context == NULL) {
    fprintf(stderr, "Error XPath context\n");
    return EXIT_FAILURE;
  }
  xmlNodePtr root = xmlNewNode(NULL, BAD_CAST "Architecture");
  xmlSetProp(root, BAD_CAST "name", BAD_CAST archi->name);
  xmlSetProp(root, BAD_CAST "CPU_name", BAD_CAST archi->CPU_name);

  xmlXPathObjectPtr res = xmlXPathEvalExpression(BAD_CAST "//object[@type=\"Cache\"]", context);
  CHECK_XPATH(res);
  if (res->type == XPATH_NODESET) {
    for(i=0; i<res->nodesetval->nodeNr; i++){ //Modifiying names and attributes of the interesting nodes
      cur = res->nodesetval->nodeTab[i];
      //Remove useless attributes
      xmlUnsetProp(cur, BAD_CAST "type");
      xmlUnsetProp(cur, BAD_CAST "cpuset");
      xmlUnsetProp(cur, BAD_CAST "complete_cpuset");
      xmlUnsetProp(cur, BAD_CAST "online_cpuset");
      xmlUnsetProp(cur, BAD_CAST "allowed_cpuset");
      xmlUnsetProp(cur, BAD_CAST "cache_type");
      //Changing node name
      xmlNodeSetName(cur, BAD_CAST "Cache");
      //Adding attributes
      //TODO use real values^^
      xmlSetProp(cur, BAD_CAST "replacement_protocol", BAD_CAST "MESI");
      xmlSetProp(cur, BAD_CAST "coherence_protocol", BAD_CAST "LFU");
    }
    root_cache = res->nodesetval->nodeTab[0]; //Setting the right root
  }
  xmlXPathFreeObject(res);

  res = xmlXPathEvalExpression(BAD_CAST "//object[@type=\"PU\"]", context); //Removing useless node
  CHECK_XPATH(res);
  for(i=0; i<res->nodesetval->nodeNr; i++){
    cur = res->nodesetval->nodeTab[i];
    xmlUnlinkNode(cur);
    xmlFreeNode(cur);
  }
  xmlXPathFreeObject(res);
  res = xmlXPathEvalExpression(BAD_CAST "//object[@type=\"Core\"]", context);
  CHECK_XPATH(res);
  for(i=0; i<res->nodesetval->nodeNr; i++){
    cur = res->nodesetval->nodeTab[i];
    xmlUnlinkNode(cur);
    xmlFreeNode(cur);
  }
  xmlXPathFreeObject(res);


  //Change root
  xmlAddChild(root, xmlDocCopyNodeList(doc, root_cache));
  xmlDocSetRootElement(fin_doc, root);
  xmlDocFormatDump(out, fin_doc, 1);

  xmlCleanupParser();
  xmlXPathFreeContext(context);
  xmlFreeDoc(fin_doc);
  xmlFreeDoc(doc);
  fclose(out);
  
  printf("Architecture xml written in file : %s\n", file_out);

  return EXIT_SUCCESS;
}

void print_caches(struct architecture * archi){
  int i;
  struct list * l;
  printf("Liste des caches\n");
  for(i=0;i<archi->number_levels;i++){
    l = archi->levels[archi->number_levels-i-1];
    while(l != NULL){      
      printf("\tL%d (misses: %d, hits: %d, writes_back: %d, broadcasts: %d)\n", l->cache->depth, l->cache->misses, l->cache->hits, l->cache->writes_back, l->cache->broadcasts);
      l = l->next;
    }
  }
}

void delete_archi(struct architecture * archi){
  int i;
  for(i=0;i<archi->number_threads;i++){
    delete_list(archi->threads[i]);
  }
  for(i=0;i<archi->number_levels;i++){
    delete_list_def(archi->levels[i]);
  }
  free(archi->threads);
  free(archi->levels);
}
