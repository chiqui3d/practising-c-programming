#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// install ES locale: sudo locale-gen es_ES.UTF-8

struct Person {
  char name[CHAR_MAX];
  char surname[CHAR_MAX];
};

int main(void) {

  struct Person carlos = {"Carlos","Campos"};

  printf("%s %s\n", carlos.name, carlos.surname);

  struct Person pedro;

  strcpy(pedro.name, "Pedro");
  strcpy(pedro.surname, "Ruiz");

  printf("%s %s\n", pedro.name, pedro.surname);

  struct Person alberto;

  char *name = "Alberto";
  char *surname = "Gomez";

  strcpy(alberto.name, name);
  strcpy(alberto.surname, surname);

  printf("%s %s\n", alberto.name, alberto.surname);
  
  return EXIT_SUCCESS;
}

