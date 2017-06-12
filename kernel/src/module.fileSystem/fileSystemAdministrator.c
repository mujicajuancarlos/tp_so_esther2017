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
