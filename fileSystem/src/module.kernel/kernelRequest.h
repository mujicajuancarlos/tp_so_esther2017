/*
 * kernelRequest.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MODULE_KERNEL_KERNELREQUEST_H_
#define MODULE_KERNEL_KERNELREQUEST_H_

#include <stdbool.h>
#include <stddef.h>
#include <commons/string.h>
#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-fileSystem-kernel.h>
#include "../module.core/fsStatusCodes.h"
#include "../module.core/sadicaManager.h"
#include "../fileSystemStruct.h"


void executeExistFileRequest(fileSystem_struct* fsStruct, Package* package);

void executeCreateFileRequest(fileSystem_struct* fsStruct, Package* package);

void executeDeleteFileRequest(fileSystem_struct* fsStruct, Package* package);

void executeReadFileRequest(fileSystem_struct* fsStruct, Package* package);

void executeWriteFileRequest(fileSystem_struct* fsStruct, Package* package);


#endif /* MODULE_KERNEL_KERNELREQUEST_H_ */
