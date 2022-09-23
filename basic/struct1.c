#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

// install ES locale: sudo locale-gen es_ES.UTF-8

struct Person {
  char *name;
  unsigned short int age;
  float height;
  float weight;
};

struct Person createPerson(char *name, unsigned short int age, float height,
                           float weight);

struct Person *createPersonPointer(char *name, unsigned short int age,
                                   float height, float weight);

void printPerson(struct Person person);

int main(void) {
  struct Person antonio = createPerson("Antonio Rodriguez", 39, 1.72, 73.50);
  printPerson(antonio);
  printf("\n");

  setlocale(LC_ALL, "es_ES.UTF-8");
  struct Person *manolo = createPersonPointer("Manolo Lopez", 39, 1.73, 74);
  printPerson(*manolo);
  free(manolo);

  return EXIT_SUCCESS;
}

struct Person createPerson(char *name, unsigned short int age, float height,
                           float weight) {

  struct Person Person = {name, age, height, weight};

  return Person;
}

struct Person *createPersonPointer(char *name, unsigned short int age,
                                   float height, float weight) {

  struct Person *Person = malloc(sizeof(struct Person));
  assert(Person != NULL);

  Person->name = name;
  Person->age = age;
  Person->height = height;
  Person->weight = weight;

  Person->height += 0.02f;
  Person->weight += 0.50f;

  return Person;
}

void printPerson(struct Person person) {
  printf("Name: %s\n", person.name);
  printf("Age: %hu\n", person.age);
  printf("Height: %.2f\n", person.height);
  printf("Weight: %.2f\n", person.weight);
}
