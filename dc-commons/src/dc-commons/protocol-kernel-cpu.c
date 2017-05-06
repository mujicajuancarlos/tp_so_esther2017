/*
 * protocol-kernel-cpu.c
 *
 *  Created on: 5/5/2017
 *      Author: utnso
 */

#include "protocol-kernel-cpu.h"

/******************************************************************************************************************
 EXCUSIVO PBC
 ******************************************************************************************************************
 */
PCB* create_new_PBC(uint32_t pid, uint32_t stackFirstPage,
		t_metadata_program* metadata) {
	PCB* newPBC = calloc(1, sizeof(PCB));
	newPBC->pid = pid;
	newPBC->programCounter = 0;
	newPBC->stackFirstPage = stackFirstPage;
	newPBC->stackOffset = 0;
	newPBC->executedQuantums = 0;
	newPBC->metadata = metadata;
	newPBC->stack = NULL;
	newPBC->stackSize = 0;
	return newPBC;
}
void destroy_PBC(PCB* pbc) {
	metadata_destruir(pbc->metadata);
	destroy_stack(pbc->stack);
	free(pbc);
}
char* serialize_PCB(PCB* pcb) {

	char *stream = malloc(sizeOf_PCB(pcb));
	uint32_t offset = 0;

	serialize_and_copy_value(stream, &(pcb->pid), sizeof(uint32_t), &offset);
	serialize_and_copy_value(stream, &(pcb->programCounter), sizeof(uint32_t),
			&offset);
	serialize_and_copy_value(stream, &(pcb->stackFirstPage), sizeof(uint32_t),
			&offset);
	serialize_and_copy_value(stream, &(pcb->stackOffset), sizeof(uint32_t),
			&offset);

	//serializo la metadata
	uint32_t size_metadata_program = sizeOf_metadata_program(pcb->metadata);
	serialize_and_copy_value(stream, &(size_metadata_program), sizeof(uint32_t),
			&offset);
	char* streamMetadata = serialize_metadata_program(pcb->metadata);
	serialize_and_copy_value(stream, streamMetadata,
			sizeof(char) * size_metadata_program, &offset);
	free(streamMetadata);

	serialize_and_copy_value(stream, &(pcb->stackSize), sizeof(uint32_t),
			&offset);

	//serializar stack
	//TODO descomentar y validar
//	uint32_t size_stack = sizeOf_context(pcb->stack, pcb->stackSize);
//	serialize_and_copy_value(stream, &(size_stack), sizeof(uint32_t), &offset);
//
//	char* streamStack = serialize_stack(&(pcb->stack), pcb->stackSize);
//	serialize_and_copy_value(stream, streamStack, sizeof(char) * size_stack,
//			&offset);
//	free(streamStack);

	return stream;
}
uint32_t sizeOf_PCB(PCB* pcb) {
	uint32_t size = 0;
	size = sizeof(uint32_t) * 6; //corresponde a los 6 atributos de tipo uint32_t, si se agrega o se elimina algino hay que modificar esto
	size += sizeOf_stack(pcb->stack);
	size += sizeOf_metadata_program(pcb->metadata);
	return size;
}
PCB* deserialize_PCB(char* stream) {

	PCB* pcb = malloc(sizeof(PCB));
	uint32_t offset = 0;

	deserialize_and_copy_value(&(pcb->pid), stream, sizeof(uint32_t), &offset);
	deserialize_and_copy_value(&(pcb->programCounter), stream, sizeof(uint32_t),
			&offset);
	deserialize_and_copy_value(&(pcb->stackFirstPage), stream, sizeof(uint32_t),
			&offset);
	deserialize_and_copy_value(&(pcb->stackOffset), stream, sizeof(uint32_t),
			&offset);

	//deserializacion de metadata
	uint32_t size_metadata_program;
	deserialize_and_copy_value(&size_metadata_program, stream, sizeof(uint32_t),
			&offset);
	char* stream_metadata = malloc(sizeof(char) * size_metadata_program);
	deserialize_and_copy_value(stream_metadata, stream, size_metadata_program,
			&offset);
	pcb->metadata = deserialize_metadata_program(stream_metadata);
	free(stream_metadata);

	deserialize_and_copy_value(&(pcb->stackSize), stream, sizeof(uint32_t),
			&offset);

	//deserializacion de stack
	//TODO descomentar y validar
//	uint32_t size_stack;
//	deserialize_and_copy_value(&size_stack, stream, sizeof(uint32_t), &offset);
//
//	char* stream_stack = malloc(sizeof(char) * size_stack);
//	deserialize_and_copy_value(stream_stack, stream, size_stack, &offset);
//	pcb->stack = deserialize_stack(stream_stack, pcb->stack);
//	free(stream_stack);

	return pcb;
}
/******************************************************************************************************************
 ******************************************************************************************************************
 */

/******************************************************************************************************************
 EXCUSIVO STACK
 ******************************************************************************************************************
 */
t_stack_program* create_new_stack() {
	t_stack_program* stack = malloc(sizeof(stack));
	//todo completar
	return stack;
}
void destroy_stack(t_stack_program* stack) {
	//todo completar
}
char* serialize_stack(t_stack_program* stack) {
	char* stream = malloc(sizeOf_stack(stack));
	//todo completar
	return stream;
}
uint32_t sizeOf_stack(t_stack_program* stack) {
	uint32_t size = 0;
	//todo completar
	return size;
}
t_stack_program* deserialize_stack(char* stream) {
	t_stack_program* stack = malloc(sizeof(stack));
	//todo completar
	return stack;
}

/******************************************************************************************************************
 ******************************************************************************************************************
 */
