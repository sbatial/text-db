#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_BASE 5

// a
char **new_database() {
  return calloc(N_BASE, sizeof(char *));
  // Almost the same as:
  // malloc(N_BASE * sizeof(char *));
  // but better/safer because (according to the manual) if
  // N_BASE * sizeof(char *) would lead to an integer overflow then malloc would
  // just allocate a wrong amount of memory whereas calloc would fail
}

void free_database(char **database) { free(database); }

// b
char **add_message(char *message, char **database, size_t *ptr_n) {
  unsigned long msg_len = strlen(message);

  if (0 == (*ptr_n % N_BASE)) {
    database = reallocarray(database, (*ptr_n + N_BASE), sizeof(char *));
    // Almost the same as:
    // database = realloc(database, (*ptr_n + N_BASE) * sizeof(char *));
    // but safer because (*ptr_n + N_BASE) * sizeof(char *) could overflow and
    // lead to problems whereas (according to the manual) reallocarray() returns
    // NULL and leaves the original memory intact
  }

  char *msg_store = malloc(sizeof(char) * (msg_len + 1));

  msg_store = message;

  *(database + *ptr_n) = msg_store;

  ++(*ptr_n);

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

  printf("msg_count: %zu\n", msg_count);

  printf("*db: %s\n", *db);
  printf("*(db + 1): %s\n", *(db + 1));
  printf("*(db + 2): %s\n", *(db + 2));
  printf("*(db + 3): %s\n", *(db + 3));
  printf("*(db + 4): %s\n", *(db + 4));
  // printf("*(db + 5): %s\n", *(db + 5)); // will seg-fault at this point bc
  // the database is not big enough

  db = add_message("Hello", db, &msg_count);
  db = add_message("World", db, &msg_count);
  db = add_message("How", db, &msg_count);
  db = add_message("Are", db, &msg_count);
  db = add_message("You", db, &msg_count);
  db = add_message("On", db, &msg_count);

  printf("msg_count: %zu\n", msg_count);
  printf("*db: %s\n", *db);
  printf("*(db + 1): %s\n", *(db + 1));
  printf("*(db + 2): %s\n", *(db + 2));
  printf("*(db + 3): %s\n", *(db + 3));
  printf("*(db + 4): %s\n", *(db + 4));
  printf("*(db + 5): %s\n", *(db + 5));
  printf("*(db + 6): %s\n", *(db + 6));
  printf("*(db + 7): %s\n", *(db + 7));
  printf("*(db + 8): %s\n", *(db + 8));
  printf("*(db + 9): %s\n", *(db + 9));
  printf("*(db + 10): %s\n", *(db + 10));
  // printf("*(db + 11): %s\n", *(db + 11)); // will seg-fault at this point bc
  // the database is not big enough

  db = add_message("This", db, &msg_count);

  printf("*db: %s\n", *db);
  printf("*(db + 1): %s\n", *(db + 1));

  free_database(db);

  return EXIT_SUCCESS;
}