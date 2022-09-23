#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
// #include <sys/types.h>

#include "../../include/h_error.h"
#include "../../include/h_prompt.h"
#include "../../include/h_string.h"
#include "./binary.h"

char *databaseActionName(enum DatabaseActions databaseAction) {
    char *strings[] = {"List",   "Search", "Read",    "Write",
                       "Update", "Delete", "Fixtures"};

    return strings[databaseAction];
}

int main(int argc, char *argv[]) {

    printf("Database binary actions:\n");
    printf("1) List\n");
    printf("2) Seach\n");
    printf("3) Read\n");
    printf("4) Write\n");
    printf("5) Update\n");
    printf("6) Delete\n");
    printf("7) Fixtures\n");
    int action = getInt("Enter the action number:");
    // scanf("%u", &action);

    enum DatabaseActions enumAction = action - 1;

    if (action < 1 || action > 7) {
        die("Error: %i is not a valid action number", action);
    }

    struct Database database = openDatabase(enumAction);

    printf("Selected action: %s\n", databaseActionName(enumAction));

    switch (enumAction) {
        case LIST:
            listPerson(database);
            break;
        case SEARCH:
            searchPerson(database);
            break;
        case READ:
            readPerson(database);
            break;
        case WRITE:
            writePerson(database);
            break;
        case UPDATE:
            updatePerson(database);
            break;
        case DELETE:
            deletePerson(database);
            break;
        case FIXTURES:
            fixturesPerson(database);
            break;
    }

    return EXIT_SUCCESS;
}

struct Database openDatabase(enum DatabaseActions databaseAction) {

    char path[CHAR_MAX];
    char dbDirectory[] = "/_data";
    char dbFileName[] = "database.dat";

    if (NULL == getcwd(path, sizeof(path))) {
        die("getcwd");
    }

    strcat(path, dbDirectory);

    if (mkdir(path, 755) == -1 && errno != EEXIST) {
        die("mkdir");
    }

    sprintf(path, "%s/%s", path, dbFileName);

    // append +b mode for binary file
    // hexdump -v -C _data/database.dat
    FILE *pr_file = fopen(path, databaseAction == FIXTURES ? "w+b" : "r+b");
    if (NULL == pr_file) {
        die("fopen");
    }

    struct Database database = {pr_file};

    return database;
}

void fixturesPerson(struct Database database) {

    struct Person persons[5] = {
        {1, "Jorge Lopez", "jorge@example.com", 89},
        {2, "Antonio Jaime", "antonio@example.com", 12},
        {3, "Carlos Campos", "carlos@example.com", 43},
        {4, "Alberto Gomez", "alberto@example.com", 56},
        {5, "Mario Javier", "mario@example.com", 35},
    };

    size_t i, write;

    for (i = 0; i < 5; i++) {

        write = fwrite(&persons[i], sizeof(struct Person), 1, database.file);
        if (write != 1) {
            die("fwrite");
        }
    }

    listPerson(database);
};

void searchPerson(struct Database database) {

    char *search = getString("Enter the name or email to search for:");

    fseek(database.file, 0, SEEK_SET);

    long int count = 0;
    struct Person *results = NULL;
    struct Person person;

    while (fread(&person, sizeof(person), 1, database.file) == 1) {
        regex_t regexName, regexEmail;
        int regexNameResult, regexEmailResult;

        regexNameResult = regcomp(&regexName, search, REG_ICASE | REG_EXTENDED);
        regexEmailResult =
            regcomp(&regexEmail, search, REG_ICASE | REG_EXTENDED);

        if (regexNameResult || regexEmailResult) {
            die("Could not compile regex");
        }

        regexNameResult = regexec(&regexName, person.name, 0, NULL, 0);
        regexEmailResult = regexec(&regexEmail, person.email, 0, NULL, 0);

        if (regexNameResult == 0 || regexEmailResult == 0) {
            count++;
            struct Person *tmp =
                realloc(results, count * sizeof(struct Person));
            if (tmp == NULL) {
                if (results) {
                    free(results);
                }
                die("Realloc");
            }

            results = tmp;
            results[count - 1] = person;
        }
        regfree(&regexName);
        regfree(&regexEmail);
    }

    if (count > 0) {
        printPersonHeader();
    }
    int i;
    for (i = 0; i < count; i++) {
        printPerson(results[i]);
    }
    // This does not work because it gives the size of the pointer, not the
    // total size of the elements. long int count2 = sizeof results / sizeof
    // *results;
    printf("Results found: %li\n", count);
    free(search);
    if (results) {
        free(results);
    }
    fclose(database.file);
};

void writePerson(struct Database database) {

    /* Seek to the end of the file */
    fseek(database.file, 0, SEEK_END);

    int id = getInt("Enter the Id:");
    char *name = getString("Enter Name:");
    char *email = getString("Enter Email:");
    int age = getInt("Enter Age:");

    struct Person person;
    person.id = id;
    // Use strncpy instead of strcpy to avoid the error of uninitialized bytes.
    strncpy(person.name, name, sizeof(person.name));
    strncpy(person.email, email, sizeof(person.email));
    person.age = age;
    free(name);
    free(email);

    size_t write = fwrite(&person, sizeof(person), 1, database.file);
    if (write != 1) {
        die("fwrite");
    }

    printPersonHeader();
    printPerson(person);

    fclose(database.file);
}

void readPerson(struct Database database) {

    int line = getInt("Enter the line number:");

    struct Person person;
    fseek(database.file, sizeof(person) * (line - 1), SEEK_SET);
    fread(&person, sizeof(person), 1, database.file);

    printPersonHeader();
    printPerson(person);
};

void updatePerson(struct Database database) {

    int line = getInt("Enter line number:");

    struct Person person;
    fseek(database.file, sizeof(person) * (line - 1), SEEK_SET);
    fread(&person, sizeof(person), 1, database.file);

    int id = getInt("Enter Id (%i):", person.id);
    char *name = getString("Enter Name (%s):", person.name);
    char *email = getString("Enter Email (%s):", person.email);
    int age = getInt("Enter Age (%i):", person.age);

    person.id = id;
    // Use strncpy instead of strcpy to avoid the error of uninitialized bytes.
    strncpy(person.name, name, sizeof(person.name));
    strncpy(person.email, email, sizeof(person.email));
    person.age = age;

    free(name);
    free(email);
    fflush(database.file);

    fseek(database.file, sizeof(person) * (line - 1), SEEK_SET);
    size_t write = fwrite(&person, sizeof(person), 1, database.file);
    if (write != 1) {
        die("fwrite");
    }

    fclose(database.file);
};

void listPerson(struct Database database) {

    fseek(database.file, 0, SEEK_END);
    long int sizeBytes = ftell(database.file);
    long int totalLines = sizeBytes / sizeof(struct Person);
    float sizeKiloBytes = sizeBytes / 1024.0;
    // Problem with conversion/rounding floating-point to decimal
    // We multiply by 100 for two decimals, if we wanted 3 we would multiply by
    // 1000 and divide by 1000
    float sizeKiloBytesNotRound = (long int)(sizeKiloBytes * 100) / 100.0;

    printf("\nSize: %li Bytes (%.2f Kb)\tRecords: %li\n", sizeBytes,
           sizeKiloBytesNotRound, totalLines);

    /* Seek to the beginning of the file */
    fseek(database.file, 0, SEEK_SET);

    printPersonHeader();

    // https://stackoverflow.com/questions/5431941/why-is-while-feof-file-always-wrong
    // https://faq.cprogramming.com/cgi-bin/smartfaq.cgi?id=1043284351&answer=1046476070
    struct Person person;
    while (fread(&person, sizeof(person), 1, database.file) == 1) {
        printPerson(person);
    }

    fclose(database.file);
};

void deletePerson(struct Database database) {

    int line = getInt("Enter the line number to be deleted:");

    line--;

    fseek(database.file, 0, SEEK_END);
    long int sizeBytes = ftell(database.file);
    long int totalLines = sizeBytes / sizeof(struct Person);
    long int length = totalLines - 1;
    long int i = line;

    for (; i <= length; i++) {
        struct Person nextPerson;
        fseek(database.file, sizeof(struct Person) * (i + 1), SEEK_SET);
        fread(&nextPerson, sizeof(nextPerson), 1, database.file);

        fseek(database.file, sizeof(struct Person) * i, SEEK_SET);
        size_t write =
            fwrite(&nextPerson, sizeof(nextPerson), 1, database.file);
        if (write != 1) {
            die("fwrite");
        }
    }
    fflush(database.file);
    fseek(database.file, 0, SEEK_SET);
    ftruncate(fileno(database.file), sizeof(struct Person) * length);
    fclose(database.file);
};

void printPerson(struct Person person) {
    printf("%-20i", person.id);
    printf("%-20s", person.name);
    printf("%-40s", person.email);
    printf("%-20hu", person.age);
    printf("\n");
    printf("\n");
}

void printPersonHeader() {
    printf("\n");
    printf("%-20s", "Id");
    printf("%-20s", "Name");
    printf("%-40s", "Email");
    printf("%-20s", "Age");
    char *separator = str_repeat("=", 100);
    printf("\n%s\n", separator);
    free(separator);
}