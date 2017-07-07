/*
 * fileSystemRequest.h
 *
 *  Created on: 1/6/2017
 *      Author: utnso
 */

#ifndef MODULE_FILESYSTEM_FILESYSTEMREQUEST_H_
#define MODULE_FILESYSTEM_FILESYSTEMREQUEST_H_

#include <stdbool.h>
#include <pthread.h>
#include <dc-commons/protocol-fileSystem-kernel.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/logger.h>
#include "../module.model/process.h"
#include "fileSystemManager.h"

void initializeFileSystemRequestMutex();
void destroyFileSystemRequestMutex();
void fileSystemRequestMutex_lock();
void fileSystemRequestMutex_unlock();

void validateExistFileRequest(Process* process, char* path, int* status);

void createFileRequest(Process* process, char* path, int* status);

void deleteFileRequest(Process* process, char* path, int* status);

void writeFileRequest(Process* process, t_fileData* data, int* status);

void readFileRequest(Process* process, t_fileData* data, int* status);

#endif /* MODULE_FILESYSTEM_FILESYSTEMREQUEST_H_ */
