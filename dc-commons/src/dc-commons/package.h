/*
 * package.h
 *
 *  Created on: 12/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_PACKAGE_H_
#define SRC_DC_COMMONS_PACKAGE_H_

#include <stdint.h>

typedef struct Package {
	uint32_t msgCode;
	uint32_t message_long;
	char* message;
} Package;

#endif /* SRC_DC_COMMONS_PACKAGE_H_ */
