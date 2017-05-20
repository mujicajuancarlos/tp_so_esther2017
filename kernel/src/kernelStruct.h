/*
 * kernelStruct.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef KERNELSTRUCT_H_
#define KERNELSTRUCT_H_

#include <pthread.h>
#include <commons/collections/list.h>
#include "configuration.h"

#define MAX_CPUS 20	//cantidad de CPUs que se pueden conectar
#define MAX_CONSOLAS 20	//cantidad de Consolas que se pueden conectar

//estructura princial del kernel
typedef struct kernel_struct {
	int socketServerCPU;
	int socketServerConsola;
	int socketClientMemoria;
	int socketClientFileSystem;
	int cpuSockets[MAX_CPUS];
	int consolaSockets[MAX_CONSOLAS];
	int pageSize;
	pthread_mutex_t stdoutMutex;
	pthread_mutex_t cpuListMutex;
	pthread_mutex_t processListMutex;
	t_list* cpuList;
	t_list* processList;
	Configuration* config;
} kernel_struct;

#endif /* KERNELSTRUCT_H_ */
