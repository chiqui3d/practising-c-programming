#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    uid_t uid = geteuid();
    gid_t gid = getegid();
    printf("uid = %d gid = %d\n", (int)uid, (int)gid);

    return 0;
}