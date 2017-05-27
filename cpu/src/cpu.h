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

#include <parser/parser.h>
#include <parser/metadata_program.h>
#include <commons/config.h>
#include <dc-commons/logger.h>
#include "cpuStruct.h"
#include "configuration.h"
#include "connection.h"
#include "module.kernel/handler-kernel.h"


void initializeStruct(cpu_struct* cpuStruct, Configuration* config);

void createSockets(cpu_struct* cpuStruct);

#endif /* CPU_H_ */
