
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

/* Read the contents of FILENAME into a newly-allocated buffer.  The
   size of the buffer is stored in *LENGTH.  Returns the buffer, which
   the caller must free.  If FILENAME doesn't correspond to a regular
   file, returns NULL.  */

char *read_file(const char *filename, size_t *length) {
    int fd;
    struct stat file_info;
    char *buffer;

    /* Open the file.  */
    fd = open(filename, O_RDONLY);

    /* Get information about the file.  */
    fstat(fd, &file_info);
    *length = file_info.st_size;
    /* Make sure the file is an ordinary file.  */
    if (!S_ISREG(file_info.st_mode)) {
        /* It's not, so give up.  */
        close(fd);
        return NULL;
    }

    /* Allocate a buffer large enough to hold the file's contents.  */
    buffer = (char *)malloc(*length);
    /* Read the file into the buffer.  */
    read(fd, buffer, *length);

    /* Finish up.  */
    close(fd);
    return buffer;
}

/* Write all of COUNT bytes from BUFFER to file descriptor FD.
Returns -1 on error, or the number of bytes written. */
ssize_t write_all(int fd, const void *buffer, size_t count) {
    size_t left_to_write = count;
    while (left_to_write > 0) {
        size_t written = write(fd, buffer, count);
        if (written == -1)
            /* An error occurred; bail. */
            return -1;
        else
            /* Keep count of how much more we need to write. */
            left_to_write -= written;
    }
    /* We should have written no more than COUNT bytes! */
    assert(left_to_write == 0);
    /* The number of bytes written is exactly COUNT. */
    return count;
}

int main(int argc, char *argv[]) {
    /* The path at which to create the new file. */
    char *filename = "./files/stat.c";
    size_t length;

    char *buffer = read_file(filename, &length);

    write_all(STDOUT_FILENO, buffer, length);

    /* Read the contents of the file. */
    return 0;
}
