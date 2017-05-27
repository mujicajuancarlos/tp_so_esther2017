/*
 * protocol-memory-kernel.c
 *
 *  Created on: 4/5/2017
 *      Author: utnso
 */

#include "protocol-memory-kernel.h"

size_t sizeof_t_AddPagesToProcess(){
	return (sizeof(uint32_t) * 2);
}

t_AddPagesToProcess* create_t_AddPagesToProcess(int pid, int size){
	t_AddPagesToProcess* object = malloc(sizeof_t_AddPagesToProcess());
	object->pid = pid;
	object->size = size;
	return object;
}
