#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define FILE_PATH "the-verdict.txt"
#define FILE_PATH "hello.txt"

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

  int char_count = 0;
  char ch;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == ' ') {
      int pos = ftell(file);
      char token_value[char_count + 1];
      token_value[char_count] = '\0';
      fseek(file, pos - char_count - 1, SEEK_SET);
      fgets(token_value, char_count + 1, file);
      fseek(file, pos, SEEK_SET);

      push_token(tokens, token_value);

      char_count = 0;
      continue;
    }

    char_count++;
  }

  if (char_count > 0) {
    int pos = ftell(file);
    char token_value[char_count + 1];
    token_value[char_count] = '\0';
    fseek(file, pos - char_count, SEEK_SET);
    fgets(token_value, char_count + 1, file);
    fseek(file, pos, SEEK_SET);

    push_token(tokens, token_value);
  }

  printf("Total tokens: %d\n", tokens->length);
  for (int i = 0; i < tokens->length; i++) {
    printf("Token %d: %s\n", i, get_token(tokens, i));
  }

  fclose(file);

  return 0;
}
