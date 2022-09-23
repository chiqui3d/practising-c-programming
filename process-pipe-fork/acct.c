#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/acct.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


static double comptToDouble(comp_t comptime)
{
	return (comptime & 0x1fff) << (((comptime >> 13) & 0x7) * 3);
}

// sudo apt-get install acct
// https://es.linux-console.net/?p=527
// CONFIG_BSD_PROCESS_ACCT_V3
//https://man7.org/linux/man-pages/man5/acct.5.html

int main() {

    int acc = acct("/var/log/pacct");
    if (acc == -1) {
        perror("acct");
        exit(EXIT_FAILURE);
    }

    struct acct_v3 acct_info;
    int fd = open("/var/log/pacct", O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int ret;
    while ((ret = read(fd, &acct_info, sizeof(acct_info)))) {
        if (ret == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        printf("tty=%d, comm=%s, etime=%.2f\n",
               acct_info.ac_tty,
               acct_info.ac_comm,
               comptToDouble(acct_info.ac_etime));
    }

    close(fd);
    
    // use poll and remove file when SIGINT is received
    //acct(NULL);


    return 0;
}