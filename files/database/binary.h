#ifndef GET_BINARY_INCLUDE
#define GET_BINARY_INCLUDE

#include <limits.h>
#include <stdio.h>

struct Person {
  int id;
  char name[CHAR_MAX];
  char email[CHAR_MAX];
  unsigned short int age;
};


struct Database {
  FILE *file;
};

enum DatabaseActions { LIST, SEARCH, READ, WRITE, UPDATE, DELETE, FIXTURES };

void printPerson(struct Person person);
void printPersonHeader();

struct Database openDatabase(enum DatabaseActions databaseAction);

void writePerson(struct Database database);

void readPerson(struct Database database);

void updatePerson(struct Database database);

void listPerson(struct Database database);

void searchPerson(struct Database database);

void deletePerson(struct Database database);

void fixturesPerson(struct Database database);

#endif