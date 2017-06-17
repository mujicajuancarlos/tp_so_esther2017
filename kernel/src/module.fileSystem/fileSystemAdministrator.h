/*
 * fileSystemAdministrator.h
 *
 *  Created on: 11/6/2017
 *      Author: utnso
 */

#ifndef MODULE_FILESYSTEM_FILESYSTEMADMINISTRATOR_H_
#define MODULE_FILESYSTEM_FILESYSTEMADMINISTRATOR_H_

#define COD_ERROR_FILE_ISNOT_OPEN -10
#define COD_ERROR_FILE_NOT_OPEN_FOR_FILE -20
#define COD_ERROR_PERMISSIONS_DENIED -30
#define COD_ERROR_FILE_NOT_FOUND_ON_PATH -40
#define COD_ERROR_FS_COULDNT_CREATE_FILE -50
#define COD_ERROR_FS_PACKAGE_COMMUNICATION -100


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

typedef struct {
	char* path;
	int offset;
	size_t size;
} t_Read;

typedef struct {
	char* path;
	int offset;
	size_t size;
	char* buffer;
} t_Write;

bool isOpen(char* path);
int openFile(Process* process, char* path, char* openMode);
t_globalFile* getFileDescriptor(char* path);
int closeFile(Process* process, char* path);
char* serialize_t_Read(t_Read* read);
char* serialize_t_Write(t_Write* write);
#endif /* MODULE_FILESYSTEM_FILESYSTEMADMINISTRATOR_H_ */
