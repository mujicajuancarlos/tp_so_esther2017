/*
 * connection.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-client.h>
#include <dc-commons/socket-server.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-memory-identification.h>
#include <dc-commons/protocol-memory-kernel.h>
#include "kernelStruct.h"
#include "module.memory/memoryRequests.h"

void createCpuServerSocket(kernel_struct* args);

void createConsoleServerSocket(kernel_struct* args);

void createMemoryClientSocket(kernel_struct* kernel_struct);

void createFileSystemClientSocket(kernel_struct* kernel_struct);

#endif /* CONNECTION_H_ */
