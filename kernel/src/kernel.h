/*
 * kernel.h
 *
 *  Created on: 3/4/2017
 *      Author: utnso
 */

#ifndef KERNEL_H_
#define KERNEL_H_


#include <commons/config.h>
#include <dc-commons/logger.h>
#include "configuration/configuration.h"

#define MAX_CPUS 20	//cantidad de CPUs que se pueden conectar
#define MAX_CONSOLAS 20	//cantidad de Consolas que se pueden conectar

/**
 * DEFINICION DE ESTRUCTURAS
 */

//estructura princial del kernel
typedef struct kernel_struct {
	int socketServerCPU;
	int socketServerConsola;
	int socketClientMemoria;
	int socketClientFileSystem;
	int cpuSockets[MAX_CPUS];
	int consolaSockets[MAX_CONSOLAS];
	t_list* listaCPUs;
	t_list* listaConsolas;
	Configuration* config;
} kernel_struct;

//estructura para representar una CPU conectada al kernel
typedef struct CPU {
    int cpuFD;	//file descriptor del socket del cpu
    int pid;	//PID del proceso que esta ejecutando actualmente
    int libre;	// 0:ocupado / 1:libre
} CPU;


#endif /* KERNEL_H_ */
