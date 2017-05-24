/*
 * fileDescriptor.h
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */

#ifndef MODULE_MODEL_FILEDESCRIPTOR_H_
#define MODULE_MODEL_FILEDESCRIPTOR_H_

#include <stdlib.h>

typedef struct {
	uint32_t fd;
	char* path;
	uint32_t open;
}t_fIleDescriptor;


#endif /* MODULE_MODEL_FILEDESCRIPTOR_H_ */
