/*
 * protocol-fileSystem-kernel.h
 *
 *  Created on: 14/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_PROTOCOL_FILESYSTEM_KERNEL_H_
#define SRC_DC_COMMONS_PROTOCOL_FILESYSTEM_KERNEL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "serialization.h"

//prefijo = KC ; between 600 - 699

#define COD_EXIST_FILE_REQUEST 601 // Validar que exista el archivo
#define COD_CREATE_FILE_REQUEST 602 // Crea archivo en el path solicitado
#define COD_DELETE_FILE_REQUEST 603 // Borra archivo del path (borra metadata y libera bloques del bitmap)
#define COD_READ_FILE_REQUEST 604 // Devuelve el path enviado de parámetro, requiere que el archivo este en modo lectura)
#define COD_WRITE_FILE_REQUEST 605 // Almacena en el path enviado, requiere archivo en modo escritura)

#define COD_FS_RESPONSE_OK 606 //usado para responder en los casos que la solicitud se haya realizado con exito
#define COD_FS_RESPONSE_ERROR 607 //usado para responder que la solicitud realizada no salio bien -> debe ir acompañado de un mensaje de error


typedef struct {
	uint32_t pathSize;
	char* path;
	uint32_t offset;
	uint32_t dataSize;
	char* data;
}t_fileData;

size_t sizeof_t_fileData(t_fileData* object);

char* serialize_t_fileData(t_fileData* object);
t_fileData* deserialize_t_fileData(char* stream);

t_fileData* create_t_fileData(char* path, uint32_t offset, uint32_t dataSize);
void destroy_t_fileData(t_fileData* object);

#endif /* SRC_DC_COMMONS_PROTOCOL_FILESYSTEM_KERNEL_H_ */
