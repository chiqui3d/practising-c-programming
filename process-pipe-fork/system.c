#include <stdlib.h>
int main() {
    int return_value;
    return_value = system("ls -l /");
    printf("return_value: %d\n", return_value);
    
    return return_value;
}