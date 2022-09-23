#include <stdio.h>
#include <string.h>
/* Returns the clock speed of the system’s CPU in MHz, as reported by
/proc/cpuinfo. On a multiprocessor machine, returns the speed of
the first CPU. On error returns zero. */
float get_cpu_clock_speed() {

    char buffer[5500];
    /* Read the entire contents of /proc/cpuinfo into the buffer. */
    FILE *fp = fopen("/proc/cpuinfo", "r");
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    buffer[bytes_read-1] = '\0';
    //printf("%s\n", buffer);
    /* Bail if read failed or if buffer isn’t big enough. */
    if (bytes_read == 0 || bytes_read == sizeof(buffer)) {
        perror("fread, not enough bytes read");
        return 0;
    }
    /* Locate the line that starts with "cpu MHz". */
    char *match = strstr(buffer, "cpu MHz");
    if (match == NULL){
        perror("strstr, no match");
        return 0;
    }
    
    /* divide to end line  */
    strtok(match, "\n");
    float clock_speed;
    sscanf(match, "cpu MHz : %f", &clock_speed);

    return clock_speed;
}
int main() {
    printf("CPU clock speed: %.3f MHz\n", get_cpu_clock_speed());
    return 0;
}