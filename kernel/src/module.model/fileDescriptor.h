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
#include <commons/string.h>
#include <dc-commons/logger.h>
#include <dc-commons/serialization.h>
#include <commons/collections/list.h>

typedef struct {
	bool read;
	bool write;
} t_flags;

typedef struct {
	char* path;  //Path del archivo
	uint32_t open; //cantidad de procesos que lo tienen abierto
} t_globalFile;

typedef struct {
	uint32_t fd;
	t_flags flag;
	t_globalFile* globalFile;
	int seekValue;

} t_processFile;

void initializeFileSystemModule();
void destroyFileSystemModule();

void globalFilesMutex_lock();
void globalFilesMutex_unlock();

t_globalFile* create_t_globalFile(char* path);
void destroy_t_globalFile(t_globalFile* fd);

t_processFile* create_t_processFile(uint32_t fd_number,
		t_globalFile* globalFile, t_flags flags);
void destroy_t_processFile(t_processFile* processFile);

void addGlobalFile(t_globalFile* globalFile);
void removeClosedGlobalFiles();

t_globalFile* getGlobalFileFor(char* path);
void dumpGlobalFiles();

#endif /* MODULE_MODEL_FILEDESCRIPTOR_H_ */
