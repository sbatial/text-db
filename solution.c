#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_BASE 5

// a
char **new_database() { return malloc(N_BASE * sizeof(char *)); }

void free_database(char **database) { free(database); }

// b
char **add_message(char *message, char **database, size_t *ptr_n) {
  unsigned long msg_len = strlen(message);
  // printf("%lu\n", msg_len);

  if (0 == (*ptr_n % N_BASE)) {
    printf("New length: %lu\n", (*ptr_n + N_BASE));
    printf("Current message: %s\n", message);
    database = realloc(database, (*ptr_n + N_BASE) * sizeof(char *));
  }

  char *msg_store = malloc(sizeof(char) * (msg_len + 1));

  ptr_n = ptr_n + 1;

  printf("%zu\n", *ptr_n);
  printf("%p\n", ptr_n);
  // database[*ptr_n] = msg_store;

  // printf("%lu\n", sizeof(msg_store));

  free(msg_store);

  return database;
}

char **delete_last_message(char **database, size_t *ptr_n);

// c
void print_message(char **database, size_t i, size_t n);
char **read_and_add_line(char **database, size_t *ptr_n);

// d
void print_all(char **database);

int main() {
  char **db = new_database();
  size_t msg_count = 0;

  printf("%zu\n", msg_count);
  printf("%p\n", &msg_count);
  db = add_message("Hello", db, &msg_count);
  printf("%zu\n", msg_count);
  printf("%p\n", &msg_count);

  free_database(db);
  // free(msg_count);

  return EXIT_SUCCESS;
}
