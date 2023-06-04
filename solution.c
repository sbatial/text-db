#include <stdio.h>
#include <stdlib.h>

#define N_BASE 5

char **new_database();
void free_database(char **database);
char **add_message(char *message, char **database, size_t *ptr_n);
char **delete_last_message(char **database, size_t *ptr_n);
void print_message(char **database, size_t i, size_t n);
char **read_and_add_line(char **database, size_t *ptr_n);
void print_all(char **database);

int main(int argc, char *argv[]) { return EXIT_SUCCESS; }
