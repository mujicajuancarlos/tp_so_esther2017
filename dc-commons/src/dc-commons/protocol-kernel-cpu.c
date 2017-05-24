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
PCB* create_new_PCB(uint32_t pid, uint32_t stackFirstPage,
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
	newPBC->exit_code = 0;
	return newPBC;
}
void destroy_PBC(PCB* pbc) {
	metadata_destruir(pbc->metadata);
	destroy_stackIndex(pbc->stack, pbc->stackSize);
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
	uint32_t size_stack = getLong_stack(pcb->stack, pcb->stackSize);
	serialize_and_copy_value(stream, &(size_stack), sizeof(uint32_t), &offset);

	char* streamStack = serialize_stack(&(pcb->stack), pcb->stackSize);
	serialize_and_copy_value(stream, streamStack, sizeof(char) * size_stack,
			&offset);
	free(streamStack);

	return stream;
}
uint32_t sizeOf_PCB(PCB* pcb) {
	uint32_t size = 0;
	size = sizeof(uint32_t) * 6; //corresponde a los 6 atributos de tipo uint32_t, si se agrega o se elimina algino hay que modificar esto
	size += sizeof(uint32_t);
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
	uint32_t size_stack;
	deserialize_and_copy_value(&size_stack, stream, sizeof(uint32_t), &offset);

	char* stream_stack = malloc(sizeof(char) * size_stack);
	deserialize_and_copy_value(stream_stack, stream, size_stack, &offset);
	pcb->stack = deserialize_stack(stream_stack, pcb->stackSize);
	free(stream_stack);

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
void destroy_stackIndex(t_stack_program* contexto, uint32_t context_len) {
	int i;
	for (i = 0; i < context_len; i++) {
		if (contexto[i].variables != NULL && contexto[i].var_len > 0) {
			free(contexto[i].variables);
		}
		if (contexto[i].argumentos != NULL && contexto[i].arg_len > 0) {
			free(contexto[i].argumentos);
		}
	}
	free(contexto);
}
char* serialize_stack(t_stack_program** contextos, uint32_t contextos_length) {

	t_stack_program* aux_contextos = *contextos;
	uint32_t total_size = getLong_stack(aux_contextos, contextos_length);
	char *stream = malloc(sizeof(char) * total_size);

	uint32_t offset = 0;

	int i;
	for (i = 0; i < contextos_length; i++) {
		char* serialized_contexto = serializar_contexto(&aux_contextos[i]);	//TODO: ver como pasarle el puntero como parametro
		uint32_t size_contexto = getLong_contexto(&aux_contextos[i]);
		serialize_and_copy_value(stream, &(size_contexto), sizeof(uint32_t),
				&offset);	//size contexto
		serialize_and_copy_value(stream, serialized_contexto,
				sizeof(char) * size_contexto, &offset);	//contexto
		free(serialized_contexto);
	}
	return stream;

}

t_stack_program* deserialize_stack(char* stream, uint32_t contextos_length) {
	//t_stack_program* stack = malloc(sizeof(stack));
	uint32_t offset = 0;

	//t_stack_program* contextos = NULL;
	t_stack_program* contextos = malloc(
			sizeof(t_stack_program) * contextos_length);
	int i;
	for (i = 0; i < contextos_length; i++) {
		uint32_t size_contexto;
		deserialize_and_copy_value(&(size_contexto), stream, sizeof(uint32_t),
				&offset);
		char* serialized_contexto = malloc(sizeof(char) * size_contexto);
		//contextos = realloc(contextos,sizeof(contexto)*(i+1));
		deserialize_and_copy_value(serialized_contexto, stream, size_contexto,
				&offset);
		t_stack_program* aux = deserializar_contexto(serialized_contexto);
		contextos[i] = *(aux);
		free(aux);
		free(serialized_contexto);
	}
	return contextos;
}

//SizeOf_Stack()
uint32_t getLong_stack(t_stack_program* contextos, uint32_t contextos_length) {
	uint32_t total = 0;
	int i;
	for (i = 0; i < contextos_length; i++) {
		total += sizeof(uint32_t);
		total += getLong_contexto(&(contextos[i]));
	}
	return total;
}

/******************************************************************************************************************
 EXCUSIVO CONTEXTO
 ******************************************************************************************************************
 */
void crearNuevoContexto(PCB* pcb) {
	pcb->stack = realloc(pcb->stack,
			sizeof(t_stack_program) * (pcb->stackSize + 1));
	pcb->stack[pcb->stackSize].arg_len = 0;
	pcb->stack[pcb->stackSize].argumentos = NULL;
	pcb->stack[pcb->stackSize].var_len = 0;
	pcb->stack[pcb->stackSize].variables = NULL;
	pcb->stackSize++;
}

void destruirContextoActual(PCB* pcb, int size_pagina) {
	pcb->stackSize--;
	t_stack_program* contextoActual = &(pcb->stack[pcb->stackSize]);
	//pongo el program counter en la direccion de retorno de la funcion
	pcb->programCounter = contextoActual->retPos;
	//libero variables y argumentos
	if (contextoActual->variables != NULL) {
		free(contextoActual->variables);
	}
	if (contextoActual->argumentos != NULL) {
		free(contextoActual->argumentos);
	}
	//reestablezco el valor del stack offset para que se pueda volver a usar el espacio para crear variables
	pcb->stackOffset -= (contextoActual->var_len * sizeof(uint32_t));
	pcb->stackOffset -= (contextoActual->arg_len * sizeof(uint32_t));
	//libero el ultimo contexto
	pcb->stack = realloc(pcb->stack,
			sizeof(t_stack_program) * (pcb->stackSize));
}

char* serializar_contexto(t_stack_program* contexto) {
	uint32_t total_size = getLong_contexto(contexto);
	char *stream = malloc(sizeof(char) * total_size);

	uint32_t offset = 0;

	//cantidad de argumentos
	serialize_and_copy_value(stream, &(contexto->arg_len), sizeof(uint32_t),
			&offset);

	//serializar array de argumentos
	char* serialized_dictionary = serializar_array_variables(
			&(contexto->argumentos), contexto->arg_len);
	serialize_and_copy_value(stream, serialized_dictionary,
			sizeof(t_variable) * contexto->arg_len, &offset);
	free(serialized_dictionary);

	//cantidad de variables
	serialize_and_copy_value(stream, &(contexto->var_len), sizeof(uint32_t),
			&offset);

	//serializar array de variables
	serialized_dictionary = serializar_array_variables(&(contexto->variables),
			contexto->var_len);
	serialize_and_copy_value(stream, serialized_dictionary,
			sizeof(t_variable) * contexto->var_len, &offset);
	free(serialized_dictionary);

	//posicion de retorno
	serialize_and_copy_value(stream, &(contexto->retPos),
			sizeof(t_puntero_instruccion), &offset);

	//direccion de variable de retorno
	serialize_and_copy_value(stream, &(contexto->retVar.pagina),
			sizeof(uint32_t), &offset);
	serialize_and_copy_value(stream, &(contexto->retVar.offset),
			sizeof(uint32_t), &offset);
	serialize_and_copy_value(stream, &(contexto->retVar.size), sizeof(uint32_t),
			&offset);

	return stream;
}

t_stack_program* deserializar_contexto(char* stream) {
	t_stack_program* context = malloc(sizeof(t_stack_program));
	uint32_t offset = 0;

	//cantidad de argumentos
	deserialize_and_copy_value(&(context->arg_len), stream, sizeof(uint32_t),
			&offset);

	char* serialized_dictionary = malloc(
			(sizeof(t_variable)) * context->arg_len);
	deserialize_and_copy_value(serialized_dictionary, stream,
			(sizeof(t_variable)) * context->arg_len, &offset);

	context->argumentos = deserializar_array_variables(serialized_dictionary,
			context->arg_len);
	free(serialized_dictionary);

	//cantidad de variables
	deserialize_and_copy_value(&(context->var_len), stream, sizeof(uint32_t),
			&offset);

	serialized_dictionary = malloc((sizeof(t_variable)) * context->var_len);
	deserialize_and_copy_value(serialized_dictionary, stream,
			(sizeof(t_variable)) * context->var_len, &offset);

	context->variables = deserializar_array_variables(serialized_dictionary,
			context->var_len);
	free(serialized_dictionary);

	deserialize_and_copy_value(&(context->retPos), stream,
			sizeof(t_puntero_instruccion), &offset);

	deserialize_and_copy_value(&(context->retVar.pagina), stream,
			sizeof(uint32_t), &offset);
	deserialize_and_copy_value(&(context->retVar.offset), stream,
			sizeof(uint32_t), &offset);
	deserialize_and_copy_value(&(context->retVar.size), stream,
			sizeof(uint32_t), &offset);

	return context;
}

uint32_t getLong_contexto(t_stack_program* contexto) {
	uint32_t longitud = 0;
	longitud += sizeof(uint32_t);
	longitud += sizeof(t_variable) * contexto->arg_len;
	longitud += sizeof(uint32_t);
	longitud += sizeof(t_variable) * contexto->var_len;
	longitud += sizeof(t_puntero_instruccion);
	longitud += sizeof(dir_memoria);
	return longitud;
}
/******************************************************************************************************************
 EXCUSIVO VARIABLE
 ******************************************************************************************************************
 */

char* serializar_array_variables(t_variable** variables, uint32_t len) {
	char *stream = malloc(sizeof(t_variable) * len);
	t_variable* aux_variables = *variables;

	uint32_t offset = 0;

	int i;
	for (i = 0; i < len; i++) {
		serialize_and_copy_value(stream, &(aux_variables[i].nombre),
				sizeof(char), &offset);
		serialize_and_copy_value(stream, &(aux_variables[i].direccion.pagina),
				sizeof(uint32_t), &offset);
		serialize_and_copy_value(stream, &(aux_variables[i].direccion.offset),
				sizeof(uint32_t), &offset);
		serialize_and_copy_value(stream, &(aux_variables[i].direccion.size),
				sizeof(uint32_t), &offset);
	}
	return stream;
}

t_variable* deserializar_array_variables(char* stream, uint32_t len) {
	uint32_t offset = 0;

	t_variable* variables = NULL;
	int i;
	for (i = 0; i < len; i++) {
		variables = realloc(variables, sizeof(t_variable) * (i + 1));
		deserialize_and_copy_value(&(variables[i].nombre), stream, sizeof(char),
				&offset);
		deserialize_and_copy_value(&(variables[i].direccion.pagina), stream,
				sizeof(uint32_t), &offset);
		deserialize_and_copy_value(&(variables[i].direccion.offset), stream,
				sizeof(uint32_t), &offset);
		deserialize_and_copy_value(&(variables[i].direccion.size), stream,
				sizeof(uint32_t), &offset);
	}
	return variables;
}
/******************************************************************************************************************
 ******************************************************************************************************************
 */
