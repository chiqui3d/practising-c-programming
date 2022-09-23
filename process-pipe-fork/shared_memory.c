#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int segment_id;
    char *shared_memory;
    struct shmid_ds shmbuffer;
    int segment_size;
    const size_t shared_segment_size = getpagesize(); // 0x6400 64Kb

    printf("%i\n", getpagesize());
    /* Allocate a shared memory segment. */
    segment_id = shmget(IPC_PRIVATE, shared_segment_size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    /* Attach the shared memory segment. */
    shared_memory = (char *)shmat(segment_id, 0, 0);
    printf("shared memory attached at address %p\n", shared_memory);
    /* Determine the segment’s size. */
    shmctl(segment_id, IPC_STAT, &shmbuffer);
    segment_size = shmbuffer.shm_segsz;
    printf("segment size: %d\n", segment_size);
    /* Write a string to the shared memory segment. */
    sprintf(shared_memory, "Hello, world.");
    printf("%s\n", shared_memory);
    printf("segment_id: %i\n", segment_id);
    sleep(5);
    
    /* Detach the shared memory segment. */
    shmdt(shared_memory);
    /* Reattach the shared memory segment, at a different address. */
    shared_memory = (char *)shmat(segment_id, (void *)0x5000000, 0); // 0x5000000 = 0x100000 * 16
    printf("shared memory reattached at address %p\n", shared_memory);
    /* Print out the string from shared memory. */
    printf("%s\n", shared_memory);
    /* Detach the shared memory segment. */
    shmdt(shared_memory);
    /* Deallocate the shared memory segment. */
    shmctl(segment_id, IPC_RMID, 0);
    return 0;
}