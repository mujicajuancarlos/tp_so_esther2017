/*
 * client.h
 *
 *  Created on: 20/4/2016
 *      Author: utnso
 */


#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#include <dc-commons/package.h>

Package* createPackage();
void destroyPackage(Package* package);
int crearSocketCliente(char* ip,int puerto);

#endif /* CLIENT_CLIENT_H_ */
