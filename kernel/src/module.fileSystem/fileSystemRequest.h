/*
 * fileSystemRequest.h
 *
 *  Created on: 1/6/2017
 *      Author: utnso
 */

#ifndef MODULE_FILESYSTEM_FILESYSTEMREQUEST_H_
#define MODULE_FILESYSTEM_FILESYSTEMREQUEST_H_

#include <stdbool.h>
#include <dc-commons/protocol-fileSystem-kernel.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/serialization.h>
#include <dc-commons/logger.h>
#include "../kernelStruct.h"
#include "../module.model/fileDescriptor.h"
#include "../module.model/process.h"
#include "fileSystemAdministrator.h"

bool validateFileExistence(Process* process, char* path);
int openFile(Process* process, char* path, char* openMode);
bool isOpen(char* path);
int createFile(Process* process, char* path, char* openMode);
int closeFile(Process* process, char* path);
int deleteFile(Process* process, char* path);
int readFile(Process* process, int fileDesc_file, int offset, size_t size);
Package* solicitudAlFileSystem(int socket, uint32_t msgCode, char* stream);
#endif /* MODULE_FILESYSTEM_FILESYSTEMREQUEST_H_ */
