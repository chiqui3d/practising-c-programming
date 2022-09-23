#include <stdio.h>  // for fprintf()
#include <stdlib.h> // for exit()

#include <syslog.h>

// sudo service rsyslog start
int main(int argc, char *argv[]) {

    openlog("Syslog Charles", LOG_PID, LOG_USER);
    syslog(LOG_WARNING, "intruder alert!");
    syslog(LOG_ERR, "can't open configuration file: %m");
    closelog();

    return EXIT_SUCCESS;
}