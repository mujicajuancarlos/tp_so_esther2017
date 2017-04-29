/*
 * connection.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

void crearServerSocketParaCpus(kernel_struct* args);

void createConsoleServerSocket(kernel_struct* args);

void createMemoryClientSocket(kernel_struct* kernel_struct);

void createFileSystemClientSocket(kernel_struct* kernel_struct);

#endif /* CONNECTION_H_ */
