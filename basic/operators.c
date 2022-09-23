#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

  printf("Operator update\n");

  short int number;
  char operation;

  printf("Enter a number:\n");
  scanf("%hu", &number);
  printf("Enter an operator (+, -, *, /):\n");
  // https://stackoverflow.com/questions/9562218/c-multiple-scanfs-when-i-enter-in-a-value-for-one-scanf-it-skips-the-second-s
  scanf(" %c", &operation);

  switch (number) {
  case 1:
    printf("%hu\n", number);
    break;
  case 2:
    printf("%hu\n", number);
    break;
  default:
    printf("Mayor a 2\n");
  }

  register double n1 = 10.20, n2 = 25.25;
  short int i = 0;
  while (i < 10) {
    i++;
    printf("While i: %hu\n", i);
  }

  switch (operation) {
  case '+':
    printf("%.2f + %.2f = %.2f", n1, n2, n1 + n2);
    break;

  case '-':
    printf("%.2f - %.2f = %.2f", n1, n2, n1 - n2);
    break;

  case '*':
    printf("%.2f * %.2f = %.2f", n1, n2, n1 * n2);
    break;

  case '/':
    printf("%.2f / %.2f = %.2f", n1, n2, n1 / n2);
    break;

  // operator doesn't match any case constant +, -, *, /
  default:
    printf("Error! operator is not exists");
  }

  printf("\n");

  char *word = "for_loop";
  size_t count = 0;

  for (i = 0; word[i] != '\0'; i++) {
    printf("char for: %c\n", word[i]);
    count++;
  }

  while (word[i] != '\0') {
    printf("char while: %c\n", word[i]);
  }

  printf("Count: %zu, strlen: %zu\n", count, strlen(word));

  printf("\n");

  int numbers[4] = { 1 };
  
  printf("numbers: %d %d %d %d\n",numbers[0], numbers[1], numbers[2], numbers[3]);

  return EXIT_SUCCESS;
}