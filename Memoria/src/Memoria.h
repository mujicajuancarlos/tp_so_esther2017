/*
 * Memoria.h
 *
 *  Created on: 13/4/2017
 *      Author: utnso
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

#include <commons/config.h>

typedef struct{
    int PUERTO;
    int MARCOS;
    int MARCO_SIZE;
    int ENTRADAS_CACHE;
    int CACHE_X_PROC;
    char REEMPLAZO_CACHE[15];
    int RETARDO_MEMORIA;

}archivoConfiguracion;


#endif /* MEMORIA_H_ */
