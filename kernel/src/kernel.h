/*
 * kernel.h
 *
 *  Created on: 3/4/2017
 *      Author: utnso
 */

#ifndef KERNEL_H_
#define KERNEL_H_


#include <commons/config.h>
#include <commons/collections/list.h>
#include <dc-commons/logger.h>
#include "configuration.h"

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

/**
 * inicializo la estructura
 */
void initializeStruct(kernel_struct* args, Configuration* config);

#endif /* KERNEL_H_ */
