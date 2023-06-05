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

void free_database(DATABASE *database) { free(database); }

// b
DATABASE *add_message(char *message, DATABASE *database) {
  unsigned long msg_len = strlen(message);

  if (0 == (database->msg_count % N_BASE)) {
    database->data = reallocarray(
        database->data, (database->msg_count + N_BASE), sizeof(char *));
    // Almost the same as:
    // database = realloc(database, (*ptr_n + N_BASE) * sizeof(char *));
    // but safer because (*ptr_n + N_BASE) * sizeof(char *) could overflow and
    // lead to problems whereas (according to the manual) reallocarray() returns
    // NULL and leaves the original memory intact
  }

  void *msg_store = malloc(sizeof(char) * (msg_len + 1));

  strcpy(msg_store, message);

  *(database->data + database->msg_count) = msg_store;

  ++database->msg_count;

  return database;
}

DATABASE *delete_last_message(DATABASE *database) {
  --(database->msg_count);

  free(*(database->data + database->msg_count));
  // == free(database[*ptr_n]);

  if (0 == (database->msg_count % N_BASE)) {
    // Explicitly subtracting N_BASE again could easily lead to mis-calculations
    // but *ptr_n should always reflect the (least) size necessary
    database = reallocarray(database, database->msg_count, sizeof(char *));
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

void print_db_message(size_t db_index, DATABASE *database) {
  if ((database->msg_count < 1) || (db_index > (database->msg_count - 1))) {
    printf("\033[31;mCannot access database at position %zu!\033[0;m (db_index "
           "= %zu "
           "> message_count = %zu)\n",
           db_index, db_index,
           database->msg_count > 0 ? database->msg_count - 1
                                   : database->msg_count);
    return;
  }

  printf("Message at %zu: %s\n", db_index, database->data[db_index]);
}
DATABASE *read_and_add_line(DATABASE *database) {
  char user_input[BUFSIZ];
  printf("Enter message to add to database: ");
  // %[^\n]s will take a string with whitespaces (up to the first "enter")
  // whereas %s will only take the user input up to the first whitespace
  scanf("%[^\n]s", user_input);

  database = add_message(user_input, database);

  return database;
}

// d
void print_all(DATABASE *database) {
  printf("\033[1;mPrint all logs\033[0;m\n");
  for (size_t idx = 0; idx < database->msg_count; idx++) {
    print_db_message(idx, database);
  }
}
void free_all(DATABASE *database) {
  for (size_t idx = 0; idx < database->msg_count; idx++) {
    database = delete_last_message(database);
  }
}
void delete_db(DATABASE *database) { free_all(database); }

void print_db_row(size_t db_index, size_t longest_row, DATABASE *database) {
  if ((database->msg_count < 1) || (db_index > (database->msg_count - 1))) {
    printf("\033[31;mCannot access database at position %zu!\033[0;m (db_index "
           "= %zu "
           "> message_count = %zu)\n",
           db_index, db_index,
           database->msg_count > 0 ? database->msg_count - 1
                                   : database->msg_count);
    return;
  }
  char row_delimiter[BUFSIZ] = "";

  for (size_t i = 0; i < longest_row; i++) {
    strcat(row_delimiter, "─");
  }

  char *cross_character = (db_index == 0 ? "┬" : "┼");
  printf("───%s─%s─\n"
         " %zu │ %s\n",
         cross_character, row_delimiter, db_index, database->data[db_index]);
}

size_t longest_row(DATABASE *database) {
  size_t longest_row = 0;

  for (size_t idx = 1; idx < database->msg_count; idx++) {
    if (strlen(database->data[idx - 1]) < strlen(database->data[idx])) {
      longest_row = idx;
    }
  }

  return longest_row;
}

void print_db(DATABASE *database) {
  if (database->msg_count == 0) {
    printf("\033[1;mEmpty database!\033[0;m\n");
    return;
  }
  printf("\033[1;mPrint all logs\033[0;m\n");

  char *longest_entry = database->data[longest_row(database)];
  size_t l_row = strlen(longest_entry);

  for (size_t idx = 0; idx < database->msg_count; idx++) {
    print_db_row(idx, l_row, database);
  }
}

int main() {
  DATABASE *database = malloc(sizeof(DATABASE));
  database->data = new_database();
  database->msg_count = 0;

  // char **db = new_database();
  size_t msg_count = 0;

  printf("msg_count: %zu\n", msg_count);
  // db = read_and_add_line(db, &msg_count);
  printf("msg_count: %zu\n", msg_count);

  // print_db_message(db, 0, msg_count);
  // print_db_message(db, 1, msg_count);
  // print_db_message(db, 2, msg_count);
  // print_db_message(db, 3, msg_count);
  // print_db_message(db, 4, msg_count);
  // print_db_message(db, 5, msg_count);
  // printf("*(db + 5): %s\n", *(db + 5)); // will seg-fault at this point bc
  // the database is not big enough

  // db = add_message("Hello", db, &msg_count);
  // db = add_message("World", db, &msg_count);
  // db = add_message("How", db, &msg_count);
  // db = add_message("Are", db, &msg_count);
  // db = add_message("You", db, &msg_count);
  // db = add_message("On", db, &msg_count);

  database = add_message("Hello", database);
  database = add_message("World", database);
  database = add_message("How", database);
  database = add_message("Are", database);
  database = add_message("You", database);
  database = add_message("On", database);
  print_db(database);

  free_all(database);
  // Should produce seg-fault at this point
  // printf("*db: %s\n", *db);
  // printf("*(db + 1): %s\n", *(db + 0));

  return EXIT_SUCCESS;
}
