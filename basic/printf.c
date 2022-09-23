#include <errno.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *str_repeat(const char *str, size_t count);

int main(void) {

  // https://www.it.uc3m.es/pbasanta/asng/course_notes/input_output_printf_es.html
  // https://www.tutorialspoint.com/cprogramming/c_data_types.htm
  printf("\n%%[parameter][flags][width][.precision][length]type\n\n");
  char helloFixed[13] = "Hello, world";
  printf("%s\n", helloFixed);

  char *separator = str_repeat("-", 13);
  printf("%s\n", separator);
  free(separator);

  printf("Fill:\n");

  printf("\t%*c", -12, '>');
  printf("\t%*c\n", 0, '<');
  printf("\t%*d\n", 5, 10);
  printf("\t%07i\n", 7);

  printf("Types:\n\n");

  double myDouble1 = 0.00000000000001;
  float power = 2.345f;
  double doubleFloat = 56789.4532;
  printf("Float point: %f\n", 0.9375);
  printf("Float point power: %f\n", power);
  printf("Float point, default 6 precision: %f\n", myDouble1);
  printf("Double float power: %f\n", doubleFloat);

  printf("Long Float or double: %lf\n", myDouble1);
  printf("Float point notation: %g\n", myDouble1);
  printf("Hexadecimal float point: %a\n", myDouble1);

  printf("Float point, 15 precision: %.15f\n", myDouble1 + myDouble1);
  printf("Decimal float point: %.2f\n", 0.9375);
  printf("Decimal float point: %f\n", 25.4);
  printf("Decimal float point: %.2f\n", 25.4);

  short int shortInt = 10;

  printf("Short Integer unsigned: %u\n", shortInt);
  printf("Short Integer unsigned: %hi\n", shortInt);

  printf("Integer: %i\n", 254);
  printf("Integer unsigned: %u\n", 254);
  printf("Integer signed: %i\n", -257);
  printf("Integer Long unsigned: %lu\n", 356699999999999);
  printf("Integer Long signed: %li\n", -356699999999999);

  printf("Hexadecimal: %x\n", -257);
  printf("Hexadecimal capital: %X\n", -257);

  double myDouble = 1.0 / 7.0;
  printf("%f\n", myDouble);
  printf("%f\n", myDouble);
  printf("%.*e\n", DECIMAL_DIG, myDouble);
  float f = 3.14159265358979323846;
  printf("%.*f\n", FLT_DIG, f);

  char nul_byte = '\0';
  int care_percentage = 100 * nul_byte;
  printf("Which means you should care %d%%.\n", care_percentage);

  printf("\n\n");

  printf("Size of short is %ld bytes\n", sizeof(short));
  printf("Size of int is %ld bytes\n", sizeof(int));
  printf("Size of long is %ld bytes\n", sizeof(long));

  printf("Size of float is %ld bytes\n", sizeof(float));
  printf("Size of double is %ld bytes\n", sizeof(double));
  printf("Size of long double is %ld bytes\n", sizeof(long double));

  printf("Size of char is %ld bytes\n", sizeof(char));
  printf("Size of void is %ld bytes\n", sizeof(void));

  return EXIT_SUCCESS;
}

char *str_repeat(const char *str, size_t count) {
  char *result = malloc(strlen(str) * count + 1);

  if (NULL == result) {
    perror("malloc() failed");
    free(result);

    exit(EXIT_FAILURE);
  }

  strcpy(result, str);
  for (size_t i = 1; i < count; i++) {
    // option 1 putchar('-');
    // option 2 result[i] = '-';
    // option 3
    strcat(result, str);
  }

  result[count] = '\0'; // setting the null-byte

  return result;
}