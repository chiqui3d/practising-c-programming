#include <stdio.h>
#include <stdlib.h>

#include "../include/h_prompt.h"

void read(FILE *pFile);
void write(FILE *pFile);

int main(void) {
  FILE *pSrcFile;
  FILE *pDestFile;

  if ((pSrcFile = fopen("./text_example.txt", "r")) == NULL) {
    fprintf(stderr, "Can't open %s for reading.\n", "./text_example.txt");
    exit(EXIT_FAILURE);
  }

  if ((pDestFile = fopen("./copy_text_example.txt", "w")) == NULL) {
    fprintf(stderr, "Can't open %s for writing.\n", "./copy_text_example.txt");
    exit(EXIT_FAILURE);
  }

  int c;
  while ((c = fgetc(pSrcFile)) != EOF) {
    fputc(c, pDestFile);
    // fprintf(pDestFile, "%c", c);
  }

  fclose(pSrcFile);
  fclose(pDestFile);

  printf("File has been copied.\n");

  return EXIT_SUCCESS;
}