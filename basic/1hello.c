#include <stdint.h> // size_t, unsigned integer
#include <stdio.h>
#include <string.h>
#include <unistd.h>
// #include <glib.h>

#define PHP "PHP is the best"
#define Pi 3.14

int main(void) {

    // sizeof in bytes, 1 char = 1 byte = 8 bits
    printf("Constant: \n");
    printf("Pi is %.2f\n", Pi);
    printf("%s\n", PHP);

    // https://stackoverflow.com/questions/9460260/what-is-the-difference-between-char-a-string-and-char-p-string/9631091#9631091
    // could be modified
    char hello[] = "Hello, world";
    char helloFixed[13] = "Hello, world";
    // not could be modified. sizeof(helloPointer) size the pointer not the content
    const char *helloPointer = "Hello, world";

    printf("Strings: \n");
    printf("%s size: %li bytes and length: %lu \n", hello, sizeof(hello), strlen(hello));
    printf("%s size: %li bytes and length: %lu\n", helloFixed, sizeof(helloFixed), strlen(helloFixed));
    printf("%s size: %li bytes and length: %lu\n", helloPointer, sizeof(helloPointer), strlen(helloPointer));

    int integer = 120;

    printf("Integer: \n");
    printf("%i size: %li bytes\n", integer, sizeof(integer));

    int numbers[] = {4, 6, 8, 2, 7, 5, 0};
    size_t sizeNumbers = sizeof numbers / sizeof numbers[0];

    printf("Array Integer: \n");
    printf("%i size: %li bytes and length: %zu\n", numbers[0], sizeof(numbers), sizeNumbers);

    execl("/bin/ls", "ls", "-la", (char *)NULL);

    return 0;
}
