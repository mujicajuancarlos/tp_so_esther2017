/*
 * connection.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <dc-commons/socket-client.h>
#include <dc-commons/logger.h>
#include "cpuStruct.h"

void createMemoryClientSocket(cpu_struct* cpuStruct);

void createKernelClientSocket(cpu_struct* cpuStruct);

#endif /* CONNECTION_H_ */
