/*
 * serialization.c
 *
 *  Created on: 24/4/2017
 *      Author: utnso
 */

#include "serialization.h"

/**
 * Serializador generico
 */
void serialize_and_copy_value(char* buffer, void* value, size_t size_to_copy,
		uint32_t* offset) {
	memcpy(buffer + *offset, value, size_to_copy);
	*offset += size_to_copy;
}

/**
 * Deserializador generico
 */
void deserialize_and_copy_value(void* value, char* buffer,
		size_t size_to_copy, uint32_t* offset) {
	memcpy(value, buffer + *offset, size_to_copy);
	*offset += size_to_copy;
}

char* serialize_int(int value) {
	size_t size = sizeof(int);
	char *stream = calloc(1, size);
	memcpy(stream, &value, size);
	return stream;
}

int deserialize_int(char* stream) {
	size_t size = sizeof(int);
	int value;
	memcpy(&value, stream, size);
	return value;
}

char* deserialize_string(char* stream, size_t length) {
	return string_substring_until(stream, length);
}

uint32_t sizeOf_metadata_program(t_metadata_program* metadata) {
	uint32_t total_size = 0;
	total_size += sizeof(metadata->instruccion_inicio); //inst_inicio
	total_size += sizeof(metadata->instrucciones_size); //size int32
	total_size += sizeof(t_intructions) * metadata->instrucciones_size; //size indice de codigo
	total_size += sizeof(metadata->etiquetas_size); //size etiquetas
	total_size += sizeof(char) * metadata->etiquetas_size; //string de etiquetas serializado
	total_size += sizeof(int) * 2; //cant funciones y etiquetas
	return total_size;
}

char* serialize_metadata_program(t_metadata_program* metadata){

	uint32_t total_size = sizeOf_metadata_program(metadata);
	char *stream = malloc(total_size);
	uint32_t offset = 0;

	serialize_and_copy_value(stream,&(metadata->instruccion_inicio),sizeof(metadata->instruccion_inicio),&offset);
	serialize_and_copy_value(stream,&(metadata->instrucciones_size),sizeof(metadata->instrucciones_size),&offset);
	//array instrucciones_serializado
	int i;
	for(i=0; i < metadata->instrucciones_size; i++){
		serialize_and_copy_value(stream,&(metadata->instrucciones_serializado[i].start),sizeof(metadata->instrucciones_serializado[i].start),&offset);
		serialize_and_copy_value(stream,&(metadata->instrucciones_serializado[i].offset),sizeof(metadata->instrucciones_serializado[i].offset),&offset);
	}

	serialize_and_copy_value(stream,&(metadata->etiquetas_size),sizeof(metadata->etiquetas_size),&offset);
	serialize_and_copy_value(stream,metadata->etiquetas,sizeof(char)*metadata->etiquetas_size,&offset);
	serialize_and_copy_value(stream,&(metadata->cantidad_de_funciones),sizeof(metadata->cantidad_de_funciones),&offset);
	serialize_and_copy_value(stream,&(metadata->cantidad_de_etiquetas),sizeof(metadata->cantidad_de_etiquetas),&offset);

	return stream;
}

t_metadata_program* deserialize_metadata_program(char* stream){

	t_metadata_program* metadata = malloc(sizeof(t_metadata_program));
	uint32_t offset = 0;

	deserialize_and_copy_value(&(metadata->instruccion_inicio), stream, sizeof(metadata->instruccion_inicio), &offset);
	deserialize_and_copy_value(&(metadata->instrucciones_size), stream, sizeof(metadata->instrucciones_size), &offset);
	//array instrucciones_serializado
	metadata->instrucciones_serializado = malloc(sizeof(t_intructions)*metadata->instrucciones_size);
	int i;
	for(i=0; i < metadata->instrucciones_size; i++){
		deserialize_and_copy_value(&(metadata->instrucciones_serializado[i].start), stream, sizeof(metadata->instrucciones_serializado[i].start),&offset);
		deserialize_and_copy_value(&(metadata->instrucciones_serializado[i].offset), stream, sizeof(metadata->instrucciones_serializado[i].offset),&offset);
	}

	deserialize_and_copy_value(&(metadata->etiquetas_size), stream, sizeof(metadata->etiquetas_size), &offset);
	deserialize_and_copy_value(metadata->etiquetas, stream, sizeof(char)*metadata->etiquetas_size,&offset);
	deserialize_and_copy_value(&(metadata->cantidad_de_funciones), stream, sizeof(metadata->cantidad_de_funciones),&offset);
	deserialize_and_copy_value(&(metadata->cantidad_de_etiquetas), stream, sizeof(metadata->cantidad_de_etiquetas),&offset);

	return metadata;
}
