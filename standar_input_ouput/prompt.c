#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char *get_line();

int main(void) {

    /* Name ----------------------------- */
    char name[50];
    printf("What is your name:\n");
    scanf("%[0-9a-zA-Z ]", name);
    printf("Name choose: %s\n", name);

    
    /* Age ----------------------------- */
    int age;
    printf("Age:\n");
    scanf("%i", &age);
    printf("Age choose: %i\n", age);

    /* Year ----------------------------- */
    int year;
    printf("Year:\n");
    fscanf(stdin, "%i", &year);
    printf("Year choose: %i\n", year);

    /* Address ----------------------------- */
    char address[100];
    printf("Address:\n");
    fscanf(stdin, "%s", address);
    printf("Address choose: %s\n", address);

    /* Village ----------------------------- */
    char line[100];
    char village[100];
    printf("Village:\n");

    while (true) {
        // VS
        // https://stackoverflow.com/questions/22330969/using-fscanf-vs-fgets-and-sscanf
        // https://stackoverflow.com/questions/1247989/how-do-you-allow-spaces-to-be-entered-using-scanf
        fgets(line, sizeof(line), stdin);
        // until '\n' newline is encountered
        if (sscanf(line, "%[^\n]", village)) {
            printf("Village choose: %s\n", village);
            break;
        }
    }

    /* Country ----------------------------- */

    char *country = NULL;
    size_t n;

    printf("Country:\n");
    getline(&country, &n, stdin);
    printf("Country choose: %s\n", country);

    free(country);
}

// https://stackoverflow.com/questions/52984551/using-fgets-with-realloc
// https://gist.github.com/btmills/4201660
// https://stackoverflow.com/questions/27326839/how-to-read-input-of-unknown-length-using-fgets

char *get_line() {

    int c;
    /* what is the buffer current size? */
    size_t size = 5;
    /* How much is the buffer filled? */
    size_t read_size = 0;
    /* firs allocation, its result should be tested... */
    char *line = malloc(size);

    if (!line) {
        perror("malloc");
        return line;
    }

    while ((c = fgetc(stdin)) != EOF && c != '\n') {

        line[read_size++] = c;
        if (read_size == size) {
            size = read_size + size;
            char *test = realloc(line, size);
            if (!test) {
                perror("realloc");
                return line;
            }

            line = test;
        }
    }

    line[read_size] = '\0';

    return line;
}