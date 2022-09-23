#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

void otherMethod(){

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char datetime[100];

    strftime(datetime, sizeof(datetime) + 1, "%Y-%m-%d %H:%M:%S %z", tm);

    printf("%s\n", datetime);
}

int main() {

    /* Obtain the time of day, and convert it to a tm struct. */
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *ptm = localtime(&tv.tv_sec);
    /* Format the date and time, down to a single second. */
    char time_string[40];
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", ptm);
    /* Compute milliseconds from microseconds. */
    long milliseconds = tv.tv_usec / 1000;
    /* Print the formatted time, in seconds, followed by a decimal point
    and the milliseconds. */
    printf("%s.%03ld\n\n", time_string, milliseconds);

    otherMethod();

    return 0;
}