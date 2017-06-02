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
#include "../kernelStruct.h"
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/serialization.h>
#include <dc-commons/logger.h>
#include "../module.model/fileDescriptor.h"

bool validateFile(kernel_struct* kernelStruct, t_fileDescriptor* fileDescriptor);

#endif /* MODULE_FILESYSTEM_FILESYSTEMREQUEST_H_ */
