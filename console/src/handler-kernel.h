/*
 * handler-kernel.h
 *
 *  Created on: 23/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_KERNEL_H_
#define HANDLER_KERNEL_H_

#include <commons/string.h>
#include <dc-commons/serialization.h>
#include <dc-commons/protocol-kernel-console.h>
#include "handler-program.h"

void handleKernelRequest(Program *program, Package* package);

#endif /* HANDLER_KERNEL_H_ */
