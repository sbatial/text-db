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

DATABASE *delete_message(DATABASE *database, size_t index) {
  if (database->msg_count == 0) {
    printf("No messages left.\n");
    return database;
  }

  if (index > (database->msg_count)) {
    printf("No field with index %zu.\n", index);
    return database;
  }

  --database->msg_count;

  if (database->msg_count == 0) {
    return database;
  }

  for (size_t idx = index; idx < database->msg_count; ++idx) {
    strcpy(database->data[idx], database->data[idx + 1]);
  }

  free(*(database->data + database->msg_count));

  // == free(database[*ptr_n]);

  if (0 == (database->msg_count % N_BASE)) {
    // Explicitly subtracting N_BASE again could easily lead to mis-calculations
    // but *ptr_n should always reflect the (least) size necessary
    database =
        reallocarray(database, database->msg_count + N_BASE, sizeof(char *));
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
  printf("\033[1;mPrint %zu logs\033[0;m\n", database->msg_count);
  for (size_t idx = 0; idx < database->msg_count; idx++) {
    print_db_message(idx, database);
  }
}

void free_all(DATABASE *database) {
  free(database->data);
  database->msg_count = 0;
}

void delete_db(DATABASE *database) {
  free_all(database);
  free_database(database);
}

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
  printf("\033[1;mPrint %zu logs\033[0;m\n", database->msg_count);

  char *longest_entry = database->data[longest_row(database)];
  size_t l_row = strlen(longest_entry);

  for (size_t idx = 0; idx < database->msg_count; idx++) {
    print_db_row(idx, l_row, database);
  }
}

void input_loop(DATABASE *database) {
  char *help_page = ":h\t\tfor help\n"
                    ":p\t\tto get current state\n"
                    ":x\t\tto delete last message\n"
                    ":x <num>\tto delete message at index <num>\n"
                    "\t\tdoes nothing if <num> > amount of logs\n"
                    ":q\t\tto quit\n";
  printf("%s", help_page);

  char user_input[BUFSIZ] = "";
  while (1) {
    // Reset input
    memset(user_input, 0, strlen(user_input));
    // memset(user_input, 0, BUFSIZ);

    printf("Enter message to add to database ");
    // %[^\n]s will take a string with whitespaces (up to the first "enter")
    // whereas %s will only take the user input up to the first whitespace
    fgets(user_input, BUFSIZ, stdin);

    // remove trailing newline character by setting the first \r or \n to the
    // terminating \0
    // see <https://stackoverflow.com/a/28462221>
    user_input[strcspn(user_input, "\r\n")] = '\0';

    if (strcmp(":q", user_input) == 0) {
      break;
    }

    if (strcmp(":p", user_input) == 0) {
      print_db(database);
      continue;
    }

    if (strcmp(":x", user_input) == 0) {
      database = delete_message(database, database->msg_count);
      continue;
    }

    char u_in[BUFSIZ];
    int idx = 0;
    sscanf(user_input, "%s %i", u_in, &idx);
    if (strcmp(":x", u_in) == 0) {
      database = delete_message(database, idx);
      continue;
    }

    if (strcmp(":h", user_input) == 0) {
      printf("%s", help_page);
      continue;
    }

    database = add_message(user_input, database);
  }
}

int main() {
  DATABASE *database = malloc(sizeof(DATABASE));
  database->data = new_database();
  database->msg_count = 0;

  input_loop(database);

  print_db(database);

  delete_db(database);

  return EXIT_SUCCESS;
}
