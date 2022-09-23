#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "../include/get_num.h"
#include "../include/h_error.h"


/* 
 * Usage:
 * make process-pipe-fork/nice_value
 *
 */
int main(int argc, char *argv[]) {
    
    if (argc != 4 || strchr("pgu", argv[1][0]) == NULL){
        die("%s {p|g|u} who priority\n"
                 " set priority of: p=process; g=process group; "
                 "u=processes for user\n",
                 argv[0]);
    }

    /* Set nice value according to command-line arguments */
    int which = (argv[1][0] == 'p') ? PRIO_PROCESS : (argv[1][0] == 'g') ? PRIO_PGRP : PRIO_USER;
    id_t who = getLong(argv[2], 0, "who");
    int prio = getInt2(argv[3], 0, "prio");
    if (setpriority(which, who, prio) == -1){
        die("setpriority");
    }
    /* Retrieve nice value to check the change */
    errno = 0; /* Because successful call may return -1 */
    prio = getpriority(which, who);
    if (prio == -1 && errno != 0){
        die("getpriority"); 
    }
    printf("Nice value = %d\n", prio);
    exit(EXIT_SUCCESS);
}