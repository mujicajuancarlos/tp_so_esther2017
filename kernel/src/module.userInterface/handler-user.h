/*
 * handler-user.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_USER_C_
#define HANDLER_USER_C_

#include <stdbool.h>
#include <pthread.h>

#include "printing-messages.h"
#include "protocol-kernel-user.h"
#include "kernel.h"


void handleUserRequest(kernel_struct* kernelStruct);

#endif /* HANDLER_USER_C_ */
