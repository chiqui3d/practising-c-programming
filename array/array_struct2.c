#include <stdio.h>
#include <string.h>

struct car {
  char make[20];
  char model[30];
  int year;
};

int main() {
  struct car arr_car[2] = {{"Audi", "TT", 2016}, {"Bentley", "Azure", 2002}};

  for (int i = 0; i < 2; i++) {
    printf("%s %s %i\n", arr_car[i].make, arr_car[i].model, arr_car[i].year);
  }
}
