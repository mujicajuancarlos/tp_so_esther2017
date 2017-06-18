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
#include "../module.model/fileDescriptor.h"
#include "../module.model/process.h"

#define OPEN_FD_SUCCESS 1
#define DELETE_FD_SUCCESS 2
#define CLOSE_FD_SUCCESS 3
#define SEEK_FD_SUCCESS 4
#define WRITE_FD_SUCCESS 5
#define READ_FD_SUCCESS 6

#define PERMISSIONS_DENIED_FD_FAILURE 10
#define FILE_NOTFOUND_FD_FAILURE 11
#define FILE_IN_USED_FD_FAILUERE 12

#define VALIDATION_FD_OK 20

#endif /* MODULE_FILESYSTEM_FILESYSTEMMANAGER_H_ */
