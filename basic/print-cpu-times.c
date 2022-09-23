#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>


int main(void) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    printf(
        "CPU time: %ld.%06ld sec user, %ld.%06ld sec system\n", 
        usage.ru_utime.tv_sec, usage.ru_utime.tv_usec, usage.ru_stime.tv_sec, usage.ru_stime.tv_usec
        );

    return 0;
}