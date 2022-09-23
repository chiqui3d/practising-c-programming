#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/wait.h>

#define FIFONAME "myfifo"

// run server and client, and then write in stdin for client and read from stdout for server

int main(void) {
    int n, fd;
    char buf[1024];

    /*
     * Open the FIFO for writing.  It was
     * created by the server.
     */
    if ((fd = open(FIFONAME, O_WRONLY)) < 0) {
        perror("open");
        exit(1);
    }

    /*
     * Read from standard input, and copy the
     * data to the FIFO.
     */
    while ((n = read(0, buf, sizeof(buf))) > 0)
        write(fd, buf, n);

    close(fd);
    exit(0);
}
