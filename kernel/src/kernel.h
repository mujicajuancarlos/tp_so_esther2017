/*
 * kernel.h
 *
 *  Created on: 3/4/2017
 *      Author: utnso
 */

#ifndef KERNEL_H_
#define KERNEL_H_

//standart
#include <stdio.h>
#include <stdlib.h>
//commoms
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>
//sockets
#include <sys/socket.h>
//configuracion
#include "configuration/configuration.h"
#include "log/logger.h"

/**
 * DEFINICION DE ESTRUCTURAS
 */

//estructura princial del kernel
typedef struct kernel_struct {
    int socketServerCPU;
    int socketServerConsola;
    t_list* listaCPUs;
    Configuration* config;
} kernel_struct;

/**
 * DEFINICION DE FUNCIONES
 */


#endif /* KERNEL_H_ */
