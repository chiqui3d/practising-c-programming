#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

int main(void) {

    /* Open the file. */
    int fd = open("/tmp/mapped_memory", O_RDWR, S_IRUSR | S_IWUSR);
    /* Create the memory mapping. */
    void *file_memory = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    int integer;
    /* Read the integer, print it out, and double it. */
    sscanf(file_memory, "%d", &integer);
    printf("value: %d\n", integer);
    sprintf((char *)file_memory, "%d\n", 2 * integer);
    /* Release the memory (unnecessary because the program exits). */
    munmap(file_memory, FILE_LENGTH);
    return 0;
}