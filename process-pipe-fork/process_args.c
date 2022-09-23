#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Prints the argument list, one argument to a line, of the process
given by PID. */
void print_process_arg_list(pid_t pid) {
    char filename[24];
    char arg_list[1024];
    /* Generate the name of the cmdline file for the process. */
    snprintf(filename, sizeof(filename), "/proc/%d/cmdline", (int)pid);
    /* Read the contents of the file. */
    int fd = open(filename, O_RDONLY);
    size_t length = read(fd, arg_list, sizeof(arg_list));
    close(fd);
    /* read does not NUL-terminate the buffer, so do it here. */
    arg_list[length] = '\0';
    /* Loop over arguments. Arguments are separated by NULs. */
    printf("%zu\n", length);
    printf("%s\n\n", arg_list);
    char *next_arg = arg_list;
    while (*next_arg != '\0') {
        printf("%s\n", next_arg);
        next_arg += strlen(next_arg) + 1;
    }
    //    while (next_arg < arg_list + length) {
    //         /* Print the argument. Each is NUL-terminated, so just treat it
    //         like an ordinary string. */
    //         printf("%s\n", next_arg);
    //         /* Advance to the next argument. Since each argument is
    //         NUL-terminated, strlen counts the length of the next argument,
    //         not the entire argument list. */
    //         next_arg += strlen(next_arg) + 1;
    //     }
}
int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        return 1;
    }

    pid_t pid = (pid_t)atoi(argv[1]);
    print_process_arg_list(pid);
    return 0;
}