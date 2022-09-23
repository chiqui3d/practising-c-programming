#include <stdio.h>
#include <unistd.h>
int main() {

    printf("Real uid = %d Effective uid = %d\n", (int)getuid(), (int)geteuid());

}