/*
 * cpuStruct.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef CPUSTRUCT_H_
#define CPUSTRUCT_H_

#include "configuration.h"

typedef struct cpu_struct {
	int socketClientKernel;
	int socketClientMemoria;
	Configuration* config;
} cpu_struct;

#endif /* CPUSTRUCT_H_ */
