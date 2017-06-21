/*
 * fileSystemManager.h
 *
 *  Created on: 17/6/2017
 *      Author: utnso
 */

#ifndef MODULE_FILESYSTEM_FILESYSTEMMANAGER_H_
#define MODULE_FILESYSTEM_FILESYSTEMMANAGER_H_

#include <parser/parser.h>
#include <dc-commons/logger.h>
#include <dc-commons/protocol-kernel-cpu.h>
#include <dc-commons/protocol-fileSystem-kernel.h>
#include "../module.memory/memoryRequests.h"
#include "../module.model/fileDescriptor.h"
#include "../module.model/process.h"
#include "fileSystemRequest.h"

#define OPEN_FD_SUCCESS 1
#define DELETE_FD_SUCCESS 2
#define CLOSE_FD_SUCCESS 3
#define SEEK_FD_SUCCESS 4
#define WRITE_FD_SUCCESS 5
#define READ_FD_SUCCESS 6

#define PERMISSIONS_DENIED_FD_FAILURE 10
#define FILE_NOTFOUND_FD_FAILURE 11
#define FILE_IN_USED_FD_FAILUERE 12
#define FS_ERROR_UNKNOWN 13
#define WITHOUT_RESOURCES_FD_FAILURE 14
#define MEMORY_SAVE_FAILURE 15

#define VALIDATION_FD_OK 20

int basicOpenProcessFile(Process* process, t_new_FD_request* dataRequest,
		int* assignedFD);

int basicDeleteProcessFile(Process* process, int fileDescriptor);

int basicCloseProcessFile(Process* process, int fileDescriptor);

int basicSeekProcessFile(Process* process, t_seed_FD_request* dataRequest);

int basicWriteProcessFile(Process* process, t_data_FD_request* dataRequest);

int basicReadProcessFile(Process* process, t_dataPointer_FD_request* dataRequest);

int createProcessFileWith(Process* process, t_globalFile* globalFile,
		t_banderas externalFlags);

t_globalFile* createGlobalFileWith(t_new_FD_request* dataRequest, int* status);

void validateDeleteFile(t_globalFile* globalFile, int* status);
void validatePermissionForCreateFile(t_banderas flags, int* status);
void validateExistFile(t_processFile* file, int* status);
void validatePermissionForWriteFile(t_processFile* file, int* status);
void validatePermissionForReadFile(t_processFile* file, int* status);

#endif /* MODULE_FILESYSTEM_FILESYSTEMMANAGER_H_ */
