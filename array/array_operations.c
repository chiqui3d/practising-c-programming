#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INDEX_NOT_FOUND -1

enum Order { ASC, DESC };

void unshift(char *newElement, char ***arrayString, size_t *size);

void push(char *newElement, char ***arrayString, size_t *size);

void insert(char *newElement, int position, char ***arrayString, size_t *size);

void delete (int index, char ***arrayString, size_t *size);

void sort(enum Order order, char ***arrayString, size_t size);

int findIndex(char *newElement, char ***arrayString, size_t size);

int main(void) {

  size_t size = 2;

  char **arrayString = malloc(size * sizeof(char *));

  if (NULL == arrayString) {
    perror("malloc() failed");
    free(arrayString);

    return EXIT_FAILURE;
  }

  arrayString[0] = "Hello, world";
  arrayString[1] = "Bye Bye, world";

  push("Hello, new world !", &arrayString, &size);
  push("Goodnight, new world", &arrayString, &size);
  delete (2, &arrayString, &size);
  arrayString[0] = "Update Hello 25";
  sort(ASC, &arrayString, size);
  insert("Hello, Position 2", 2, &arrayString, &size);

  char newElement[] = "Hello, Begin";
  unshift(newElement, &arrayString, &size);

  for (int i = 0; i < size; i++) {
    printf("%i: %s\n", i, arrayString[i]);
  }

  int index = findIndex(newElement, &arrayString, size);
  if (index != INDEX_NOT_FOUND) {
    printf("Index found in the position: %i\n", index);
  } else {
    printf("Index not found for string: %s\n", newElement);
  }

  free(arrayString);

  return EXIT_SUCCESS;
}

/**
 * Find the index of an element in an array of strings.
 *
 * @param element The element to find in the array.
 * @param arrayString a pointer to a pointer to a char.
 * @param size the size of the array
 *
 * @return The index of the element in the array or INDEX_NOT_FOUND otherwise.
 */
int findIndex(char *element, char ***arrayString, size_t size) {

  int i;

  for (i = 0; i < size; i++) {
    if (strcmp(element, (*arrayString)[i]) == 0) {
      return i;
    }
  }

  return INDEX_NOT_FOUND;
}

/**
 * It sorts an array of strings in ascending order
 *
 * Bubble sort algorithm implementation for array of strings,
 * Swap elements if  first element is bigger than second
 *
 * @param order The order of the sorting.
 * @param arrayString a pointer to a pointer to a pointer to a char.
 * @param size The number of strings in the array.
 *
 * @return void
 */
void sort(enum Order order, char ***arrayString, size_t size) {

  int i, j;

  for (i = 0; i < size; i++) {
    for (j = 0; j < size - 1; j++) {
      if (order == ASC) {
        if (strcmp((*arrayString)[j], (*arrayString)[j + 1]) > 0) {
          char *tmp = (*arrayString)[j];
          (*arrayString)[j] = (*arrayString)[j + 1];
          (*arrayString)[j + 1] = tmp;
        }
      } else {
        if (strcmp((*arrayString)[j], (*arrayString)[j + 1]) < 0) {
          char *tmp = (*arrayString)[j];
          (*arrayString)[j] = (*arrayString)[j + 1];
          (*arrayString)[j + 1] = tmp;
        }
      }
    }
  }
}

/**
 * It deletes the string at the given index from the array of strings
 *
 * @param position The index of the string to delete.
 * @param arrayString The array of strings.
 * @param size  The size of the array.
 *
 * @return void
 */
void delete (int position, char ***arrayString, size_t *size) {
  int i;

  for (i = 0; i < (*size); i++) {
    if (i < position) {
      (*arrayString)[i] = (*arrayString)[i];
    } else if (i > position) {
      (*arrayString)[i - 1] = (*arrayString)[i];
    }
  }

  (*size)--;

  *arrayString = realloc(*arrayString, (*size) * sizeof(char *));
}

/**
 * Insert new element at the given position in the array of strings
 *
 * Shifts all the elements in the array from the position where the new
 * element should be inserted to the end of the array one position to the right.
 * Finally, it inserts the new element into the array at the specified position
 *
 * @param newElement The new element to be inserted into the array.
 * @param position The position in the array where the new element will be
 * inserted.
 * @param arrayString The array of strings to insert into.
 * @param size the size of the array
 *
 * @return void
 */
void insert(char *newElement, int position, char ***arrayString, size_t *size) {
  int i;

  (*size)++;

  char **tmpArrayString = realloc(*arrayString, (*size) * sizeof(char *));

  if (NULL == tmpArrayString) {
    perror("realloc() failed");
    free(tmpArrayString);
    exit(EXIT_FAILURE);
  }

  *arrayString = tmpArrayString;

  for (i = (*size) - 1; i > position; i--) {
    (*arrayString)[i] = (*arrayString)[i - 1];
  }

  (*arrayString)[position] = newElement;
}

/**
 * It adds a new element to the end of the array
 *
 * @param newElement The new element to add.
 * @param arrayString a pointer to a pointer to a pointer to a char.
 * @param size The number of strings in the array.
 *
 * @return void
 */
void push(char *newElement, char ***arrayString, size_t *size) {
  (*size)++;

  char **tmpArrayString = realloc(*arrayString, (*size) * sizeof(char *));

  if (NULL == tmpArrayString) {
    perror("realloc() failed");
    free(arrayString);

    exit(EXIT_FAILURE);
  }

  *arrayString = tmpArrayString;
  (*arrayString)[(*size) - 1] = newElement;
}

/**
 * Adds the new string to the beginning of the array
 *
 * @param newElement The new element to be added to the beginning of the array.
 * @param arrayString a pointer to the array of strings
 * @param size the size of the array
 *
 * @return void
 */
void unshift(char *newElement, char ***arrayString, size_t *size) {
  (*size)++;

  char **tmpArrayString = realloc(*arrayString, (*size) * sizeof(char *));

  if (NULL == tmpArrayString) {
    perror("realloc() failed");
    free(arrayString);
    exit(EXIT_FAILURE);
  }

  *arrayString = tmpArrayString;

  for (int i = (*size) - 1; i > 0; i--) {
    (*arrayString)[i] = (*arrayString)[i - 1];
  }

  (*arrayString)[0] = newElement;
}