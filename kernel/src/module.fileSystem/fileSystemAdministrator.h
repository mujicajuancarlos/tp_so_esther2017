/*
 * fileSystemAdministrator.h
 *
 *  Created on: 11/6/2017
 *      Author: utnso
 */

#ifndef MODULE_FILESYSTEM_FILESYSTEMADMINISTRATOR_H_
#define MODULE_FILESYSTEM_FILESYSTEMADMINISTRATOR_H_

#include <stdbool.h>
#include <dc-commons/protocol-fileSystem-kernel.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/serialization.h>
#include <dc-commons/logger.h>
#include "../kernelStruct.h"
#include "../module.model/fileDescriptor.h"
#include "../module.model/process.h"
#include "fileSystemRequest.h"


bool isOpen(char* path);
int openFile(Process* process, char* path, char* openMode);
int closeFile(Process* process, char* path);
#endif /* MODULE_FILESYSTEM_FILESYSTEMADMINISTRATOR_H_ */
