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
	newPBC->metadata = metadata;
	newPBC->stackSize = 0;
	newPBC->stackIndex = NULL;
	newPBC->exit_code = 0;
	return newPBC;
}
void destroy_PBC(PCB* pcb) {
	if (pcb->metadata != NULL) {
		metadata_destruir(pcb->metadata);
		pcb->metadata = NULL;
	}
	if (pcb->stackIndex != NULL) {
		destroy_stackArray(pcb->stackIndex, pcb->stackSize);
		pcb->stackIndex = NULL;
	}
	free(pcb);
}
uint32_t sizeOf_PCB(PCB* pcb) {
	uint32_t size = 0;
	size = sizeof(uint32_t) * 6; //corresponde a los 6 atributos de tipo uint32_t, si se agrega o se elimina algino hay que modificar esto
	size += sizeOf_metadata_program(pcb->metadata);
	size += sizeof_stackArray(pcb->stackIndex, pcb->stackSize);
	return size;
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
	//serializo la metadata

	serialize_and_copy_value(stream, &(pcb->stackSize), sizeof(uint32_t),
			&offset);

	//serializar stack
	uint32_t size_stack = sizeof_stackArray(pcb->stackIndex, pcb->stackSize);
	serialize_and_copy_value(stream, &(size_stack), sizeof(uint32_t), &offset);

	char* streamStack = serialize_stackArray(pcb->stackIndex, pcb->stackSize);
	serialize_and_copy_value(stream, streamStack, sizeof(char) * size_stack,
			&offset);
	free(streamStack);
	//serializar stack
	serialize_and_copy_value(stream, &(pcb->exit_code), sizeof(uint32_t),
			&offset);

	return stream;
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
	uint32_t size_stack;
	deserialize_and_copy_value(&size_stack, stream, sizeof(uint32_t), &offset);

	char* stream_stack = malloc(sizeof(char) * size_stack);
	deserialize_and_copy_value(stream_stack, stream, size_stack, &offset);
	pcb->stackIndex = deserialize_stackArray(stream_stack, pcb->stackSize);
	free(stream_stack);

	deserialize_and_copy_value(&(pcb->exit_code), stream, sizeof(uint32_t),
			&offset);
	return pcb;
}

/******************************************************************************************************************
 EXCUSIVO METADATA PROGRAM
 ******************************************************************************************************************
 */
uint32_t sizeOf_metadata_program(t_metadata_program* metadata) {
	uint32_t total_size = 0;
	total_size += sizeof(metadata->instruccion_inicio);	//inst_inicio
	total_size += sizeof(metadata->instrucciones_size);	//size int32
	total_size += sizeof(t_intructions) * metadata->instrucciones_size;	//size indice de codigo
	total_size += sizeof(metadata->etiquetas_size);	//size etiquetas
	total_size += sizeof(char) * metadata->etiquetas_size;//string de etiquetas serializado
	total_size += sizeof(int) * 2;	//cant funciones y etiquetas
	return total_size;
}

char* serialize_metadata_program(t_metadata_program* metadata) {
	uint32_t total_size = sizeOf_metadata_program(metadata);
	char* serialized = malloc(total_size);
	uint32_t offset = 0;
	int i;

	serialize_and_copy_value(serialized, &(metadata->instruccion_inicio),
			sizeof(metadata->instruccion_inicio), &offset);
	serialize_and_copy_value(serialized, &(metadata->instrucciones_size),
			sizeof(metadata->instrucciones_size), &offset);

	for (i = 0; i < metadata->instrucciones_size; i++) {
		serialize_and_copy_value(serialized,
				&(metadata->instrucciones_serializado[i].start),
				sizeof(metadata->instrucciones_serializado[i].start), &offset);
		serialize_and_copy_value(serialized,
				&(metadata->instrucciones_serializado[i].offset),
				sizeof(metadata->instrucciones_serializado[i].offset), &offset);
	}

	serialize_and_copy_value(serialized, &(metadata->etiquetas_size),
			sizeof(metadata->etiquetas_size), &offset);
	serialize_and_copy_value(serialized, metadata->etiquetas,
			sizeof(char) * metadata->etiquetas_size, &offset);
	serialize_and_copy_value(serialized, &(metadata->cantidad_de_funciones),
			sizeof(metadata->cantidad_de_funciones), &offset);
	serialize_and_copy_value(serialized, &(metadata->cantidad_de_etiquetas),
			sizeof(metadata->cantidad_de_etiquetas), &offset);

	return serialized;
}

t_metadata_program* deserialize_metadata_program(char* serialized) {
	t_metadata_program* metadata = malloc(sizeof(t_metadata_program));
	uint32_t offset = 0;
	int i;

	deserialize_and_copy_value(&(metadata->instruccion_inicio), serialized,
			sizeof(metadata->instruccion_inicio), &offset);
	deserialize_and_copy_value(&(metadata->instrucciones_size), serialized,
			sizeof(metadata->instrucciones_size), &offset);

	metadata->instrucciones_serializado = malloc(
			sizeof(t_intructions) * metadata->instrucciones_size);
	for (i = 0; i < metadata->instrucciones_size; i++) {
		deserialize_and_copy_value(
				&(metadata->instrucciones_serializado[i].start), serialized,
				sizeof(metadata->instrucciones_serializado[i].start), &offset);
		deserialize_and_copy_value(
				&(metadata->instrucciones_serializado[i].offset), serialized,
				sizeof(metadata->instrucciones_serializado[i].offset), &offset);
	}

	deserialize_and_copy_value(&(metadata->etiquetas_size), serialized,
			sizeof(metadata->etiquetas_size), &offset);
	metadata->etiquetas = malloc(sizeof(char) * metadata->etiquetas_size);
	deserialize_and_copy_value(metadata->etiquetas, serialized,
			sizeof(char) * metadata->etiquetas_size, &offset);
	deserialize_and_copy_value(&(metadata->cantidad_de_funciones), serialized,
			sizeof(metadata->cantidad_de_funciones), &offset);
	deserialize_and_copy_value(&(metadata->cantidad_de_etiquetas), serialized,
			sizeof(metadata->cantidad_de_etiquetas), &offset);

	return metadata;
}

/******************************************************************************************************************
 EXCUSIVO STACK
 ******************************************************************************************************************
 */
uint32_t sizeof_stackArray(t_stack_index* stackArray, uint32_t stackSize) {
	uint32_t total = 0;
	int i;
	for (i = 0; i < stackSize; i++) {
		total += sizeof(uint32_t); //esto es para la serializacion, guarda el tamaño de stackIndex
		total += sizeof_stackIndex(&(stackArray[i]));
	}
	return total;
}

uint32_t sizeof_stackIndex(t_stack_index* stackIndex) {
	uint32_t longitud = 0;
	longitud += sizeof(uint32_t);
	longitud += sizeof(t_variable) * stackIndex->arg_len;
	longitud += sizeof(uint32_t);
	longitud += sizeof(t_variable) * stackIndex->var_len;
	longitud += sizeof(t_puntero_instruccion);
	longitud += sizeof(dir_memoria);
	return longitud;
}

char* serialize_stackArray(t_stack_index* stackArray, uint32_t stackSize) {
	uint32_t total_size = sizeof_stackArray(stackArray, stackSize);
	char* stream = malloc(sizeof(char) * total_size);
	uint32_t offset = 0;
	int i;
	for (i = 0; i < stackSize; i++) {
		uint32_t stackIndexSize = sizeof_stackIndex(&stackArray[i]);
		char* stackIndexBuffer = serialize_stackIndex(&stackArray[i]);
		//TODO: ver como pasarle el puntero como parametro -> &stackArray[i] |||| stackArray + i
		serialize_and_copy_value(stream, &(stackIndexSize), sizeof(uint32_t),
				&offset);
		serialize_and_copy_value(stream, stackIndexBuffer,
				sizeof(char) * stackIndexSize, &offset);
		free(stackIndexBuffer);
	}
	return stream;
}

char* serialize_stackIndex(t_stack_index* stackIndex) {
	uint32_t total_size = sizeof_stackIndex(stackIndex);
	char *stream = malloc(sizeof(char) * total_size);
	char* tmpBuffer;
	uint32_t offset = 0;

	serialize_and_copy_value(stream, &(stackIndex->arg_len), sizeof(uint32_t),
			&offset);
	tmpBuffer = serializeVariablesArray(stackIndex->args,
			stackIndex->arg_len);
	serialize_and_copy_value(stream, tmpBuffer,
			sizeof(t_variable) * stackIndex->arg_len, &offset);
	free(tmpBuffer);

	serialize_and_copy_value(stream, &(stackIndex->var_len), sizeof(uint32_t),
			&offset);
	tmpBuffer = serializeVariablesArray(stackIndex->vars,
			stackIndex->var_len);
	serialize_and_copy_value(stream, tmpBuffer,
			sizeof(t_variable) * stackIndex->var_len, &offset);
	free(tmpBuffer);

	serialize_and_copy_value(stream, &(stackIndex->retPos),
			sizeof(t_puntero_instruccion), &offset);
	serialize_and_copy_value(stream, &(stackIndex->retVar.pagina),
			sizeof(uint32_t), &offset);
	serialize_and_copy_value(stream, &(stackIndex->retVar.offset),
			sizeof(uint32_t), &offset);
	serialize_and_copy_value(stream, &(stackIndex->retVar.size),
			sizeof(uint32_t), &offset);

	return stream;
}
t_stack_index* deserialize_stackArray(char* buffer, uint32_t stackSize) {
	uint32_t offset = 0;
	int i;
	t_stack_index* stackArray = malloc(sizeof(t_stack_index) * stackSize);
	for (i = 0; i < stackSize; i++) {
		uint32_t stackIndexSize;
		deserialize_and_copy_value(&(stackIndexSize), buffer, sizeof(uint32_t),
				&offset);
		char* stackIndexBuffer = malloc(sizeof(char) * stackIndexSize);
		deserialize_and_copy_value(stackIndexBuffer, buffer, stackIndexSize,
				&offset);
		t_stack_index* stackIndex = deserialize_stackIndex(stackIndexBuffer);
		stackArray[i] = *(stackIndex);  //TODO: verificar
		free(stackIndex);  //verificar
		free(stackIndexBuffer);
	}
	return stackArray;
}

t_stack_index* deserialize_stackIndex(char* stream) {
	t_stack_index* context = malloc(sizeof(t_stack_index));
	uint32_t offset = 0;
	char* tmpBuffer;

	deserialize_and_copy_value(&(context->arg_len), stream, sizeof(uint32_t),
			&offset);

	tmpBuffer = malloc((sizeof(t_variable)) * context->arg_len);
	deserialize_and_copy_value(tmpBuffer, stream,
			(sizeof(t_variable)) * context->arg_len, &offset);
	context->args = deserializeVariablessssArray(tmpBuffer, context->arg_len);
	free(tmpBuffer);

	deserialize_and_copy_value(&(context->var_len), stream, sizeof(uint32_t),
			&offset);
	tmpBuffer = malloc((sizeof(t_variable)) * context->var_len);
	deserialize_and_copy_value(tmpBuffer, stream,
			(sizeof(t_variable)) * context->var_len, &offset);
	context->vars = deserializeVariablessssArray(tmpBuffer, context->var_len);
	free(tmpBuffer);

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

void destroy_stackArray(t_stack_index* stackArray, uint32_t stackSize) {
	int i;
	for (i = 0; i < stackSize; i++) {
		destroy_stackIndex(stackArray[i]);
	}
	free(stackArray);
}

void destroy_stackIndex(t_stack_index stackIndex) {
	if (stackIndex.vars != NULL && stackIndex.var_len > 0) {
		free(stackIndex.vars);
	}
	if (stackIndex.args != NULL && stackIndex.arg_len > 0) {
		free(stackIndex.args);
	}
}

/******************************************************************************************************************
 EXCUSIVO VARIABLES - ARGUMENTOS
 ******************************************************************************************************************
 */

char* serializeVariablesArray(t_variable* variables, uint32_t len) {
	char *stream = malloc(sizeof(t_variable) * len);
	uint32_t offset = 0;
	int i;
	for (i = 0; i < len; i++) {
		serialize_and_copy_value(stream, &(variables[i].nombre),
				sizeof(char), &offset);
		serialize_and_copy_value(stream, &(variables[i].direccion.pagina),
				sizeof(uint32_t), &offset);
		serialize_and_copy_value(stream, &(variables[i].direccion.offset),
				sizeof(uint32_t), &offset);
		serialize_and_copy_value(stream, &(variables[i].direccion.size),
				sizeof(uint32_t), &offset);
	}
	return stream;
}

t_variable* deserializeVariablessssArray(char* stream, uint32_t len) {
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
 EXCUSIVO CONTEXTO
 Es un array de t_stack_index
 ******************************************************************************************************************
 */
void createNewContext(PCB* pcb) {
	pcb->stackIndex = realloc(pcb->stackIndex,
			sizeof(t_stack_index) * (pcb->stackSize + 1));
	pcb->stackIndex[pcb->stackSize].arg_len = 0;
	pcb->stackIndex[pcb->stackSize].args = NULL;
	pcb->stackIndex[pcb->stackSize].var_len = 0;
	pcb->stackIndex[pcb->stackSize].vars = NULL;
	pcb->stackSize++;
	logInfo("Se creó el contexto principal (mail) del stack para el pid: %d", pcb->pid);
}

void destroyCurrentContext(PCB* pcb) {
	pcb->stackSize--;
	t_stack_index* contextoActual = &(pcb->stackIndex[pcb->stackSize]);
	pcb->programCounter = contextoActual->retPos;
	if (contextoActual->vars != NULL) {
		free(contextoActual->vars);
	}
	if (contextoActual->args != NULL) {
		free(contextoActual->args);
	}
	pcb->stackOffset -= (contextoActual->var_len * sizeof(uint32_t));
	pcb->stackOffset -= (contextoActual->arg_len * sizeof(uint32_t));
	pcb->stackIndex = realloc(pcb->stackIndex,
			sizeof(t_stack_index) * (pcb->stackSize));
}

/******************************************************************************************************************
//EXCLUSIVO SET SHARED VALUE
 Es un array de t_stack_index
 ******************************************************************************************************************
 */
set_shared_var* createSetSharedVar(char* name, int value){
	set_shared_var* object = malloc(sizeof(set_shared_var));
	object->name = malloc(strlen(name));
	object->sizeName = strlen(name);
	object->newValue = value;
	return object;
}
void destroySetSharedVar(set_shared_var* object){
	if(object->name != NULL)
		free(object->name);
	free(object);
}
size_t sizeOf_SetSharedVar(set_shared_var* object){
	size_t size = 0;
	size += sizeof(uint32_t) * 2;
	size += sizeof(char) * object->sizeName;
	return size;
}
char* serialize_SetSharedVar(set_shared_var* object){
	char* buffer = malloc(sizeOf_SetSharedVar(object));
	uint32_t offset = 0;
	serialize_and_copy_value(buffer, &(object->sizeName), sizeof(uint32_t), &offset);
	serialize_and_copy_value(buffer, object->name, sizeof(char) * object->sizeName, &offset);
	serialize_and_copy_value(buffer, &(object->newValue), sizeof(uint32_t), &offset);
	return buffer;
}
set_shared_var* deserialize_SetSharedVar(char* buffer){
	set_shared_var* object = malloc(sizeof(set_shared_var));
	uint32_t offset = 0;
	deserialize_and_copy_value(&(object->sizeName), buffer, sizeof(uint32_t), &offset);
	object->name = malloc(sizeof(char)*object->sizeName);
	deserialize_and_copy_value(object->name, buffer, sizeof(char)*object->sizeName, &offset);
	deserialize_and_copy_value(&(object->newValue), buffer, sizeof(uint32_t), &offset);
	return object;
}
