#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_BASE 5

typedef struct {
  char **data;
  size_t msg_count;
} DATABASE;

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

  void *msg_store = malloc(sizeof(char) * (msg_len + 1));

  strcpy(msg_store, message);

  *(database + *ptr_n) = msg_store;

  ++(*ptr_n);

  return database;
}

char **delete_last_message(char **database, size_t *ptr_n) {
  --(*ptr_n);

  free(*(database + *ptr_n));
  // == free(database[*ptr_n]);

  if (0 == (*ptr_n % N_BASE)) {
    // Explicitly subtracting N_BASE again could easily lead to mis-calculations
    // but *ptr_n should always reflect the (least) size necessary
    database = reallocarray(database, *ptr_n, sizeof(char *));
    // Probably not necessary here because we _shrink_ memory but still safer
    // than
    // database = realloc(database, *ptr_n * sizeof(char *));
  }
  return database;
}

// c
void print_message(char **database, size_t i, size_t n) {
  if (i > n) {
    printf("\033[31;mCannot access database at position %zu!\033[0;m (i = %zu "
           "> n = %zu)\n",
           i, i, n);
    return;
  }

  printf("Message at %zu: %s\n", i, database[i]);
}
// The function above should solve the excercise as stated but we feel like it
// makes more sense to flesh it out a bit more and take into account that in an
// array(/db) with e.g. 6 messages you can only access messages up to db[5]

void print_db_message(char **database, size_t db_index, size_t message_count) {
  if ((message_count < 1) || (db_index > (message_count - 1))) {
    printf("\033[31;mCannot access database at position %zu!\033[0;m (db_index "
           "= %zu "
           "> message_count = %zu)\n",
           db_index, db_index,
           message_count > 0 ? message_count - 1 : message_count);
    return;
  }

  printf("Message at %zu: %s\n", db_index, database[db_index]);
}
char **read_and_add_line(char **database, size_t *ptr_n) {
  char user_input[BUFSIZ];
  printf("Enter message to add to database: ");
  // %[^\n]s will take a string with whitespaces (up to the first "enter")
  // whereas %s will only take the user input up to the first whitespace
  scanf("%[^\n]s", user_input);

  database = add_message(user_input, database, ptr_n);

  return database;
}

// d
void print_all(char **database, size_t message_count) {
  printf("\033[1;mPrint all logs\033[0;m\n");
  for (size_t idx = 0; idx < message_count; idx++) {
    print_db_message(database, idx, message_count);
  }
}
void free_all(char **database, size_t message_count) {
  for (size_t idx = 0; idx < message_count; idx++) {
    delete_last_message(database, &message_count);
  }
}
void delete_db(char **database, size_t message_count) {
  free_all(database, message_count);
  free_database(database);
}

void print_db_row(char **database, size_t db_index, size_t message_count,
                  size_t longest_row) {
  if ((message_count < 1) || (db_index > (message_count - 1))) {
    printf("\033[31;mCannot access database at position %zu!\033[0;m (db_index "
           "= %zu "
           "> message_count = %zu)\n",
           db_index, db_index,
           message_count > 0 ? message_count - 1 : message_count);
    return;
  }
  char row_delimiter[BUFSIZ] = "";

  for (size_t i = 0; i < longest_row; i++) {
    strcat(row_delimiter, "─");
  }

  char *cross_character = (db_index == 0 ? "┬" : "┼");
  printf("───%s─%s─\n"
         " %zu │ %s\n",
         cross_character, row_delimiter, db_index, database[db_index]);
}

size_t longest_row(char **database, size_t message_count) {
  size_t longest_row = 0;

  for (size_t idx = 1; idx < message_count; idx++) {
    if (strlen(database[idx - 1]) < strlen(database[idx])) {
      longest_row = idx;
    }
  }

  return longest_row;
}

void print_db(char **database, size_t message_count) {
  printf("\033[1;mPrint all logs\033[0;m\n");

  char *longest_entry = database[longest_row(database, message_count)];
  size_t l_row = strlen(longest_entry);

  for (size_t idx = 0; idx < message_count; idx++) {
    print_db_row(database, idx, message_count, l_row);
  }
}

int main() {
  char **db = new_database();
  size_t msg_count = 0;

  printf("msg_count: %zu\n", msg_count);
  // db = read_and_add_line(db, &msg_count);
  printf("msg_count: %zu\n", msg_count);

  print_db_message(db, 0, msg_count);
  print_db_message(db, 1, msg_count);
  print_db_message(db, 2, msg_count);
  print_db_message(db, 3, msg_count);
  print_db_message(db, 4, msg_count);
  print_db_message(db, 5, msg_count);
  // printf("*(db + 5): %s\n", *(db + 5)); // will seg-fault at this point bc
  // the database is not big enough

  db = add_message("Hello", db, &msg_count);
  db = add_message("World", db, &msg_count);
  db = add_message("How", db, &msg_count);
  db = add_message("Are", db, &msg_count);
  db = add_message("You", db, &msg_count);
  db = add_message("On", db, &msg_count);

  print_db(db, msg_count);

  delete_db(db, msg_count);
  // Should produce seg-fault at this point
  // printf("*db: %s\n", *db);
  // printf("*(db + 1): %s\n", *(db + 0));

  return EXIT_SUCCESS;
}
