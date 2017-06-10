/*
 * fileSystemRequest.c
 *
 *  Created on: 1/6/2017
 *      Author: utnso
 */

#include "fileSystemRequest.h"

bool validateFile(kernel_struct* kernelStruct, char* path) {

	Package* package = createAndSendPackage(
			kernelStruct->socketClientFileSystem, COD_VALIDAR_ARCHIVO,
			strlen(path), path);
	if (package != NULL) {
		destroyPackage(package);
		package = createAndReceivePackage(kernelStruct->socketClientFileSystem);
		if (package != NULL) {
			bool response = deserialize_int(package->stream);
			if (response) {
				logInfo(
						"El Archivo ha sido validado correctamente y se encuentra en la ruta %s",
						path);
				destroyPackage(package);
				return true;
			} else
				logInfo("El Archivo no se encuentra en la ruta %s", path);
		} else
			logInfo("No se pudo recibir los datos del FileSystem");
	} else
		logInfo("No se pudo enviar los datos al FileSystem");
	destroyPackage(package);
	return false;
}

int openFile(Process* process, char* path, char* openMode) {

	if (isOpen(path)) { //Aca veo si esta abierto, si es asi no creo fd, solo aumento Open

		bool condicion(void* element) {
					t_processFileDescriptor* unPFD = element;
					return strcmp(unPFD->fileDescriptor->path, path);
		}

		t_fileDescriptor* fd = list_find(fileDescriptorGlobalList, condicion);
		incrementarOpen(fd);
		t_processFileDescriptor* pfd = createNew_t_processFileDescriptor(
				openMode, fd);
		agregarPFD_Alista(process, pfd);
		return fd->fd;
	} else {
		int create = createFile(process, path, openMode);	//Si no está abierto, creo el FD
		return create;
	}
}

	int createFile(Process* process, char* path, char* openMode) {

		if (strcmp(openMode, "c")) { //Verifico permisos de creacion
			t_fileDescriptor* file = createNew_t_fileDescriptor(path);
			agregarFD_Alista(file);

			t_processFileDescriptor* pfd = createNew_t_processFileDescriptor(
					openMode, file);
			agregarPFD_Alista(process, pfd);

			logInfo(
					"El Archivo de fileDescriptor %d ha sido creado correctamente y se encuentra en la ruta %s",
					file->fd, file->path);

			return file->fd;
		} else {
			logInfo(
					"El Archivo que se solicitó abrir no cuenta con los permisos necesarios para ser creado");
			return -1;
		}
	}

	int closeFile(Process* process, char* path) { //Recibo el path del archivo a cerrar, y el proceso que lo contiene.

		if (isOpen(path)) {								// Si esta abierto =>
			bool condicion(void* element) {									//{
				t_processFileDescriptor* unPFD = element;					//
				return strcmp(unPFD->fileDescriptor->path, path);//Busco el proceso
			}																//
			t_processFileDescriptor* pfd = list_find(
					process->fileDescriptorList, condicion);				//}
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

	int deleteFile(Process* process, char* path) {
		if (isOpen(path)) {

			bool condicion(void* element) {
				t_processFileDescriptor* unPFD = element;
				return strcmp(unPFD->fileDescriptor->path, path);
			}

			t_processFileDescriptor* processFDFound = list_find(
					process->fileDescriptorList, condicion);

			if (processFDFound != NULL) {
				solicitudAlFileSystem(
						process->kernelStruct->socketClientFileSystem,
						COD_BORRAR_ARCHIVO, path);
				return 0;
			} else {
				logInfo("El proceso %d no tiene abierto el archivo %s",
						process->pid, path);
				return -1;
			}
		} else
			logInfo("El archivo %s no se encuentra abiero por ningún proceso",
					path);
		return -2;

	}

	bool isOpen(char* path) {

		bool condicion(void* element) {
			t_fileDescriptor* unFD = element;
			return strcmp(unFD->path, path);
		}

		return list_any_satisfy(fileDescriptorGlobalList, condicion);
	}





	/*
	 writeFile(uint32_t fd, int offset,char*buffer, t_size tamanio, Process* process){
	 char* path = obtenerPath(fd);

	 //TODO: Esta funcion envia y devuelve el package
	 //TODO: hay que serializar los datos y meterlos en el stream ¿Estructuras?

	 int socket = process->kernelStruct->socketClientFileSystem;

	 Package* package = solicitudAlFileSystem(socket,
	 COD_GUARDAR_DATOS,stream); //TODO: path,offset,tamanio,buffer

	 if(package == NULL){
	 logInfo("Error en solicitud a FileSystem");
	 destroyPackage(package);
	 }
	 destroyPackage(package); //TODO: se podria poner aca solo

	 package = createAndReceivePackage(socket);

	 //TODO Evaluar si hubo error terminar, si no mandar a Ready

	 }

	 readFile(Process* process, char* path, char* buffer, int size){

	 //TODO: Esta funcion SOLO ENVIA, la devolucion la manejara el handler correspondiente.
	 //TODO: hay que serializar los datos y meterlos en el stream ¿Estructuras?  Sí.

	 int socketFS = process->kernelStruct->socketClientFileSystem;

	 Package* package = solicitudAlFileSystem(socketFS,
	 COD_OBTENER_DATOS,stream); //TODO:(path, buffer,size)  Struct

	 if(package == NULL){
	 logInfo("Error en solicitud a FileSystem");
	 destroyPackage(package);
	 }
	 destroyPackage(package); //TODO: se podria poner aca solo

	 package = createAndReceivePackage(socketFS);

	 //TODO:	si surgio error ejecutar la rutina de finalizacion de proceso con el codigo de error adecuado
	 //si no hay error enviar a ready (ver donde se almacena el dato obtenido del fs)
	 }


	 */

	Package* solicitudAlFileSystem(int socket, uint32_t msgCode, char* stream) {

		Package* package = createAndSendPackage(socket, msgCode, strlen(stream),
				stream);
		if (package != NULL){
			logInfo("Paquete enviado correctamente");
		}
		else
		{
			logInfo("Error al enviar paquete");
		}

		destroyPackage(package);
		package = createAndReceivePackage(socket);
		return package;

	}




