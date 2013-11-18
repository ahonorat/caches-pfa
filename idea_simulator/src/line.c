#include "line.h"

/* Data allocations */
struct line** init_line(int nb_ways) {
  struct line **lines = malloc(nb_ways * sizeof(struct line *));
  int i;
  for (i=0; i<nb_ways; i++) {
    struct line *line = malloc(sizeof(struct line));
    line->first_case = 0;
    line->valid = 0;
    line->use = 0;
    line->written = 0;
    line->shared = 0;
    lines[i] = line;
  }
  return lines;
}

/* Data removal */
void delete_lines(struct line **lines, int nb_ways) {
  int i;
  for (i=0; i<nb_ways; i++) {
    free(lines[i]);
  }
  free(lines);
}

void update_line(struct line *line) {
  line->use ++;
}

void invalid_line(struct line *line) {
  line->valid = 0;
}

void modify_line(struct line *line) {
  line->written = 1;
}

void share_line(struct line *line) {
  line->shared = 1;
}

void exclusive_line(struct line *line) {
  line->shared = 0;
}
