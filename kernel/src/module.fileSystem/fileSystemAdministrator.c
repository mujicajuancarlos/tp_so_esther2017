/*
 * fileSystemAdministrator.c
 *
 *  Created on: 11/6/2017
 *      Author: utnso
 */

#include "fileSystemAdministrator.h"

bool isOpen(char* path) {

		bool condicion(void* element) {
			t_fileDescriptor* unFD = element;
			return string_equals_ignore_case(unFD->path, path);
		}

		return list_any_satisfy(fileDescriptorGlobalList, condicion);
	}

int openFile(Process* process, char* path, char* openMode) {

		if (isOpen(path)) { //Aca veo si esta abierto, si es asi no creo fd, solo aumento Open

			bool condicion(void* element) {
						t_processFileDescriptor* unPFD = element;
						return string_equals_ignore_case(unPFD->fileDescriptor->path, path);
			}
			tablaGlobalFD_mutex_lock();

			t_fileDescriptor* fd = list_find(fileDescriptorGlobalList, condicion);

			incrementarOpen(fd);

			tablaGlobalFD_mutex_unlock();

			t_processFileDescriptor* pfd = createNew_t_processFileDescriptor(
					openMode, fd);

			agregarPFD_Alista(process, pfd);

			return fd->fd;
		}
		else {

			int create = createFile(process, path, openMode);	//Si no está abierto, creo el FD

			return create;
		}
	}

int closeFile(Process* process, char* path) { //Recibo el path del archivo a cerrar, y el proceso que lo contiene.

			if (isOpen(path)) {								// Si esta abierto =>

				bool condicion(void* element) {									//{
					t_processFileDescriptor* unPFD = element;					//
					return string_equals_ignore_case(unPFD->fileDescriptor->path, path);//Busco el proceso
				}																//

				t_processFileDescriptor* pfd = list_find(process->fileDescriptorList, condicion);				//}

				if (pfd != NULL) {

					decrementarOpen(pfd->fileDescriptor);						//

					removerPFD_Lista(pfd, process);

					logInfo(
							"Se ha cerrado el archivo abierto %s perteneciente al proceso %d",
							path, process->pid);
					return 0;
				} else
					logInfo("El archivo %s no es un archivo abierto del proceso %d",
							path, process->pid);
				return -1;
			} else
				logInfo("El archivo %s no se encuentra abiero por ningún proceso",
						path);
			return -2;

		}

size_t sizeOf_t_Read(t_Read* read) {
	size_t size = 0;
	size += sizeof(int);
	size += sizeof(read->size);
	size += sizeof(char) * read->size;
	return size;
}

size_t sizeOf_t_Write(t_Write* write){
	size_t size = 0;
	size += sizeof(int);
	size += sizeof(write->size);
	size += 2*(sizeof(char) * write->size);
	return size;
}

char* serialize_t_Read(t_Read* read) {
	char* buffer = malloc(sizeOf_t_Read(read));

	uint32_t offset = 0;

	serialize_and_copy_value(buffer, read->path,
			sizeof(char) * read->size, &offset);

	serialize_and_copy_value(buffer, &(read->offset), sizeof(int),
				&offset);

	serialize_and_copy_value(buffer, &(read->size), sizeof(size_t),
			&offset);

	return buffer;
}

char* serialize_t_Write (t_Write* write){
	char* buffer = malloc(sizeOf_t_Write(write));

	uint32_t offset = 0;

	serialize_and_copy_value(buffer,write->path,
			sizeof(char) * write->size , &offset);

	serialize_and_copy_value(buffer, &(write->offset),sizeof(int),
			&offset);

	serialize_and_copy_value(buffer, &(write->size),sizeof(size_t),
			&offset);

	serialize_and_copy_value(buffer,write->buffer,sizeof(char) * write->size,&offset);

	return buffer;
}

t_Read* deserialize_t_Read(char* buffer) {

	t_Read* read = malloc(sizeof(t_Read));

	uint32_t offset = 0;

	read->path = malloc(sizeof(char) * read->size);

	deserialize_and_copy_value(read->path, buffer,
			sizeof(char) * read->size, &offset);

	deserialize_and_copy_value(&(read->offset), buffer, sizeof(int),
				&offset);

	deserialize_and_copy_value(&(read->size), buffer, sizeof(size_t),
			&offset);
	return read;
}

t_Write* deserialize_t_Write(char* buffer){
	t_Write* write = malloc(sizeof(t_Write));
	uint32_t offset = 0;

	write->path = malloc(sizeof(char) * write->size);

	deserialize_and_copy_value(write->path, buffer,
			sizeof(char) * write->size,&offset);

	deserialize_and_copy_value(&(write->offset), buffer, sizeof(int),
					&offset);

	deserialize_and_copy_value(&(write->size), buffer, sizeof(size_t),
			&offset);

	write->buffer = malloc(sizeof(char) * write->size);

	deserialize_and_copy_value(write->buffer, buffer,
			sizeof(char) * write->size,&offset);

	return write;


}

