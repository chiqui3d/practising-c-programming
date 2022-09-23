#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/h_prompt.h"

void read(FILE *pFile);
void write(FILE *pFile);

int main(void) {
  FILE *pFile;

  if ((pFile = fopen("./text_example.txt", "a+")) == NULL) {
    fprintf(stderr, "Can't open %s for reading.\n", "./text_example.txt");
    exit(EXIT_FAILURE);
  }

  printf("Text file actions:\n");
  printf("1) Read\n");
  printf("2) Write\n");
  int action = getInt("Enter the action number:");

  switch (action) {
  case 1:
    read(pFile);
    break;
  case 2:
    write(pFile);
    break;
  }

  fclose(pFile);

  return EXIT_SUCCESS;
}

void read(FILE *pFile) {
  printf("Read\n");
  fseek(pFile, 0, SEEK_SET);

  char line[CHAR_MAX];

  while (fgets(line, CHAR_MAX, pFile)) {

    size_t length = strlen(line);
    printf("%zu\n", length);

    if (line[length - 1] == '\n') {
      printf("%s\n", "Yes, the line contains break line");
    }

    puts(line);
    // printf("%s", line);
  }
}
void write(FILE *pFile) {
  printf("Write\n");
  fseek(pFile, 0, SEEK_END);
  fputs("No tengo barco\n", pFile);
  fprintf(pFile, "%s\n", "fprintf mola mazo");
}