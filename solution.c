#include <stdio.h>
#include <stdlib.h>

#define N_BASE 5

// a
char **new_database() { return malloc(N_BASE * sizeof(char *)); }

void free_database(char **database) { free(database); }

// b
char **add_message(char *message, char **database, size_t *ptr_n);
char **delete_last_message(char **database, size_t *ptr_n);

// c
void print_message(char **database, size_t i, size_t n);
char **read_and_add_line(char **database, size_t *ptr_n);

// d
void print_all(char **database);

int main() {
  char **db = new_database();

  free_database(db);

  return EXIT_SUCCESS;
}
