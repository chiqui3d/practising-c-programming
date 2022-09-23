#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define FILE_LENGTH 0x100 // 256 bytes
/* Return a uniformly random number in the range [low,high]. */
int random_range(unsigned const low, unsigned const high) {
    unsigned const range = high - low + 1;
    return low + (int)(((double)range) * rand() / (RAND_MAX + 1.0));
}

int main(int argc, char *const argv[]) {

    /* Seed the random number generator. */
    srand(time(NULL));
    /* Prepare a file large enough to hold an unsigned integer. */
    int fd = open("/tmp/mapped_memory", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    /* Extend the file to the appropriate size. */
    lseek(fd, FILE_LENGTH + 1, SEEK_SET);
    write(fd, "", 1);
    /* SEEK_SET is the beginning of the file. */
    lseek(fd, 0, SEEK_SET);
    /* Create the memory mapping. */
    void *file_memory = mmap(0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    /* Write a random integer to memory-mapped area. */
    int random = random_range(-100, 100);
    printf("random insert: %d\n", random);
    sprintf((char *)file_memory, "%d\n",random);
    /* Release the memory (unnecessary because the program exits). */
    munmap(file_memory, FILE_LENGTH);


    return 0;
}