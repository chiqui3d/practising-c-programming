#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    /* The path at which to create the new file. */
    char *path = "./files/create_file.txt";
    /* The permissions for the new file. */
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    /* Create the file. */
    int fd = open(path, O_WRONLY | O_EXCL | O_CREAT, mode);
    if (fd == -1) {
        /* An error occurred. Print an error message and bail. */
        perror("open");
        return 1;
    }
    return 0;
}