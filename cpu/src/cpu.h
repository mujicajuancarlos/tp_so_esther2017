/*
 * cpu.h
 *
 *  Created on: 6/4/2017
 *      Author: utnso
 */

#ifndef CPU_H_
#define CPU_H_

//standar
#include <stdio.h>
#include <stdlib.h>

#include <commons/config.h>
#include <dc-commons/socket-client.h>
#include <dc-commons/logger.h>
#include "configuration.h"

//estructura princial del cpu
typedef struct cpu_struct {
	int socketClientKernel;
	int socketClientMemoria;
	Configuration* config;
} cpu_struct;

#endif /* CPU_H_ */
