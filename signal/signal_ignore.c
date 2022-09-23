
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {

    struct sigaction sa; 
    
    sa.sa_handler = SIG_IGN; // ignore signal    
    sa.sa_sigaction = SIG_DFL;                  
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("Sigaction");
        exit(EXIT_FAILURE);
    }

    // or 
    // signal(SIGCHLD, SIG_IGN); // ignore signal


    exit(EXIT_SUCCESS);
}