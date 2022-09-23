#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    struct rlimit rl;
    /* Obtain the current limits. */
    getrlimit(RLIMIT_CPU, &rl);
    /* Set a CPU limit of 1 second. */
    rl.rlim_cur = 1;
    setrlimit(RLIMIT_CPU, &rl);
    /* Do busy work. */
    int count = 0;
    while (1);
    
    return 0;
}