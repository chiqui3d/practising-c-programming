#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *print_message_function(void *ptr);

int main(void) {

    pthread_t hilo1, hilo2;
    char *mensaje1 = "Subproceso 1";
    char *mensaje2 = "Subproceso 2";
    int iret1, iret2;

    /* Crear subprocesos independientes, cada uno de los cuales ejecutará la función */

    iret1 = pthread_create(&hilo1, NULL, print_message_function, (void *)mensaje1);
    iret2 = pthread_create(&hilo2, NULL, print_message_function, (void *)mensaje2);

    /* Espere hasta que los subprocesos estén completos antes de que main continúe. */

    /* el proceso y todos los subprocesos antes de que se hayan completado los subprocesos. */

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    printf("El subproceso 1 devuelve: %d\n", iret1);
    printf("El subproceso 2 devuelve: %d\n", iret2);
    exit(0);
}

void *print_message_function(void *ptr) {
    char *mensaje;
    mensaje = (char *)ptr;
    printf("%s\n", mensaje);
    return NULL;
}