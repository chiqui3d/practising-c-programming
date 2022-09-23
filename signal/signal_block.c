
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {

    /* ... */
    sigset_t signal_set; /* We don't need oldset in this program. You can add it,
                            but it's best to use different sigset for the second
                            and third argument of sigprocmask. */
    sigemptyset(&signal_set);

    sigaddset(&signal_set, SIGSEGV);
    sigaddset(&signal_set, SIGRTMIN);

    /* now signal_set == {SIGSEGV, SIGRTMIN} */

    sigprocmask(SIG_BLOCK, &signal_set, NULL); /* As i said, we don't bother with the oldset argument. */

    kill(0, SIGSEGV);
    kill(0, SIGSEGV); /* SIGSEGV is not a realtime signal, so we can send it twice, but it will be received just once */

    sigprocmask(SIG_UNBLOCK, &signal_set, NULL); /* Again, don't bother with oldset */

    /* SIGSEGV will be received here */

    exit(EXIT_SUCCESS);
}