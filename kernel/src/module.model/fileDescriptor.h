/*
 * fileDescriptor.h
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */

#ifndef MODULE_MODEL_FILEDESCRIPTOR_H_
#define MODULE_MODEL_FILEDESCRIPTOR_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <dc-commons/logger.h>
#include <dc-commons/serialization.h>
#include <commons/collections/list.h>

#define FIRST_PROCESS_FD 3
#define READ "r"
#define WRITE "w"
#define RW "rw"

typedef struct {
	bool read;
	bool write;
}flags;

typedef struct {
	char* path;  //Path del archivo
	uint32_t open; //cantidad de procesos que lo tienen abierto
}t_globalFile;

typedef struct {
	uint32_t fd;
	flags flag;
	t_globalFile* globalFile;
	int seekValue;

}t_processFile;

void initializeFileSystemModule();
void destroyFileSystemModule();

void globalFilesMutex_lock();
void globalFilesMutex_unlock();

t_globalFile* create_t_globalFile(char* path);
void destroy_t_globalFile(t_globalFile* fd);

t_processFile* create_t_processFile(uint32_t fd_number, t_globalFile* globalFile, flags flags);
void destroy_t_processFile(t_processFile* processFile);

void addGlobalFile(t_globalFile* globalFile);
void removeClosedGlobalFiles();

#endif /* MODULE_MODEL_FILEDESCRIPTOR_H_ */
