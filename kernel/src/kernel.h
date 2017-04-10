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

#define MAX_CPUS 20	//cantidad de CPUs que se pueden conectar
#define MAX_CONSOLAS 20	//cantidad de Consolas que se pueden conectar

/**
 * DEFINICION DE ESTRUCTURAS
 */

//estructura princial del kernel
typedef struct kernel_struct {
	int socketServerCPU;
	int socketServerConsola;
	int cpuSockets[MAX_CPUS];
	int consolaSockets[MAX_CONSOLAS];
	t_list* listaCPUs;
	Configuration* config;
} kernel_struct;

//estructura para representar una CPU conectada al kernel
typedef struct CPU {
    int cpuFD;	//file descriptor del socket del cpu
    int pid;	//PID del proceso que esta ejecutando actualmente
    int libre;	// 0:ocupado / 1:libre
} CPU;

/**
 * DEFINICION DE FUNCIONES
 */

#endif /* KERNEL_H_ */
