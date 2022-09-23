
#include "../include/h_debug.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {

    char path[] = "inputfile.txt";

    int rval = open(path, O_RDONLY);

    if (rval != 0) {
        /* Save errno because it’s clobbered by the next system call. */
        int error_code = errno;
        /* The operation didn’t succeed; chown should return -1 on error. */
        assert(rval == -1);
        /* Check the value of errno, and take appropriate action. */
        switch (error_code) {
            case EPERM:        /* Permission denied. */
            case EROFS:        /* PATH is on a read-only file system. */
            case ENAMETOOLONG: /* PATH is too long. */
            case ENOENT:       /* PATH does not exit. */
            case ENOTDIR:      /* A component of PATH is not a directory. */
            case EACCES:       /* A component of PATH is not accessible. */
                /* Something’s wrong with the file. Print an error message. */
                fprintf(stderr, "\n%s: %s\n\n", path, strerror(error_code));
                //perror("open");
                /* Don’t end the program; perhaps give the user a chance to
                choose another file... */
                exit(EXIT_FAILURE);
            case EFAULT:
                /* PATH contains an invalid memory address. This is probably a bug. */
                abort();
            case ENOMEM:
                /* Ran out of kernel memory. */
                fprintf(stderr, "%s\n", strerror(error_code));
                exit(1);
            default:
                /* Some other, unexpected, error code. We’ve tried to handle all
                possible error codes; if we’ve missed one, that’s a bug! */
                abort();
        };
    }
}
