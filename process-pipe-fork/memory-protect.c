#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int alloc_size;
static char *memory;

void segv_handler(int signal_number) {
    printf("memory accessed !\n");
    mprotect(memory, alloc_size, PROT_READ | PROT_WRITE);
}
int main() {
    /* Install segv_handler as the handler for SIGSEGV. */
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &segv_handler;
    // SIGSEGV signal that Linux sends to the program when it tries to access that memory.
    sigaction(SIGSEGV, &sa, NULL);
    /* Allocate one page of memory by mapping /dev/zero. Map the memory as write-only, initially. */
    alloc_size = getpagesize();
    int fd = open("/dev/zero", O_RDONLY);
    memory = mmap(NULL, alloc_size, PROT_WRITE, MAP_PRIVATE, fd, 0);
    close(fd);
    /* Write to the page to obtain a private copy. */
    memory[0] = 0;
    /* Make the memory unwritable. */
    printf("memory protected !\n");
    mprotect(memory, alloc_size, PROT_NONE);
    /* Write to the allocated memory region. */
    memory[0] = 1;
    /* All done; unmap the memory. */
    printf("all done\n");
    printf("memory value: %i\n", memory[0]);
    munmap(memory, alloc_size);
    return 0;
}