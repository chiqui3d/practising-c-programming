#include <fcntl.h>
#include <linux/cdrom.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    /* Open a file descriptor to the device specified on the command line. */
    int fd = open("/dev/hdc", O_RDONLY);
    /* Eject the CD-ROM. */
    ioctl(fd, CDROMEJECT);
    /* Close the file descriptor. */
    close(fd);
    return 0;
}