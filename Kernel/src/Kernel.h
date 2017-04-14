#ifndef KERNEL__SVR__KERNEL_H_
#define KERNEL__SVR__KERNEL_H_


#include <commons/config.h>

typedef struct{
    int PUERTO_PROG;
    int PUERTO_CPU;
    char IP_MEMORIA[15];
    int PUERTO_MEMORIA;
    char IP_FS[15];
    int PUERTO_FS;
    int QUANTUM;
    int QUANTUM_SLEEP;
    char ALGORITMO[15];
    int GRADO_MULTIPROG;
    // las sig variables son arrays
    //char **SEM_IDS;
    //int SEM_INIT;
    //char **SHARED_VARS;
    int STACK_SIZE;


}archivoConfiguracion;

#endif /* KERNEL__SVR__KERNEL_H_ */
