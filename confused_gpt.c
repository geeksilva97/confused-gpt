#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define FILE_PATH "the-verdict.txt"
#define FILE_PATH "hello.txt"
#define TEMPORARY_BUFFER_SIZE 5

typedef struct {
  char **items;
  int capacity;
  int length;
} Tokens;

Tokens *create_tokens(int capacity) {
  Tokens *tokens = malloc(sizeof(Tokens));
  tokens->items = malloc(sizeof(char *) * capacity);
  tokens->capacity = capacity;
  tokens->length = 0;
  return tokens;
}

void free_tokens(Tokens *tokens) {
  for (int i = 0; i < tokens->length; i++) {
    free(tokens->items[i]);
  }
  free(tokens->items);
  free(tokens);
}

char *get_token(Tokens *tokens, int index) {
  if (index < 0 || index >= tokens->length) {
    return NULL;
  }
  return tokens->items[index];
}

void push_token(Tokens *tokens, const char *item) {
  if (tokens->length == tokens->capacity) {
    tokens->capacity *= 2;
    tokens->items = realloc(tokens->items, sizeof(char *) * tokens->capacity);
  }
  tokens->items[tokens->length] = strdup(item);
  tokens->length++;
}

int main() {
  Tokens *tokens = create_tokens(10);

  FILE *file = fopen(FILE_PATH, "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  int word_capacity = TEMPORARY_BUFFER_SIZE;
  char *word_buf = malloc(word_capacity);
  int length = 0;
  char ch;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == ' ' || ch == '\n') {
      if (length > 0) {
        word_buf[length] = '\0';
        push_token(tokens, word_buf);
        length = 0;
        continue;
      }
    }

    // ignore spaces from the beggining of the words
    if (ch == ' ') continue;

    if (ch == '.' || ch == '?' || ch == '!' || ch == '(' || ch == ')' || ch == '"' || ch == '\'' || ch == '-' || ch == ';' || ch == ':') {
      push_token(tokens, word_buf);
      push_token(tokens, &ch);
      length = 0;
      continue;
    }

    if (length >= word_capacity - 1) {
      word_capacity *= 2;
      word_buf = realloc(word_buf, word_capacity);
    }

    word_buf[length] = ch;

    length++;
  }

  printf("%d tokens\n", tokens->length);
  for (int i = 0; i < tokens->length; ++i) {
    char *token_description = tokens->items[i];
    printf("Token [%d]: %s\n", i, token_description);
  }

  free(word_buf);
  fclose(file);

  return 0;
}
