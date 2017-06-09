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

				logInfo(
						"El Archivo no se encuentra en la ruta %s",
						path);
		} else
			logInfo("No se pudo recibir los datos del FileSystem");
	} else
		logInfo("No se pudo enviar los datos al FileSystem");

	destroyPackage(package);

	return false;

}

uint32_t openFile(kernel_struct* kernelStruct, char* path, char* openMode,
		Process* process) {
/*----------------------------------------------------------------------------------------------------------------------------
 * Aca veo si esta abierto, si es asi no creo fd, solo aumento Open
 * ---------------------------------------------------------------------------------------------------------------------------
 */
	bool condicion(void* element) {
		t_fileDescriptor* unFD = element;
		return strcmp(unFD->path, path);
	}

	bool resultado = list_any_satisfy(fileDescriptorGlobalList, condicion);

	if (resultado) {

		t_fileDescriptor* fd = list_find(fileDescriptorGlobalList, condicion);

		incrementarOpen(fd);

		t_processFileDescriptor* pfd = createNew_t_processFileDescriptor(
				openMode, fd);

		agregarPFD_Alista(process, pfd);

		return fd->fd;
	}
/*-------------------------------------------------------------------------------------------------------------------------------
 * Si no esta abierto, creo el fd
 * ----------------------------------------------------------------------------------------------------------------------------
 */
	else {
		bool validacion = validateFile(kernelStruct,path);
		if(validacion){

		t_fileDescriptor* file = createNew_t_fileDescriptor(path);

		agregarFD_Alista(file);

		logInfo(
				"El Archivo de fileDescriptor %d ha sido abierto correctamente y se encuentra en la ruta %s",
				file->fd, file->path);

		t_processFileDescriptor* pfd = createNew_t_processFileDescriptor(openMode, file);

		agregarPFD_Alista(process, pfd);

		return file->fd;

			}
	}
	return -1; //TODO: mover a exit con exit code?
}




void closeFile(char* path, Process* process) { //Recibo el path del archivo a cerrar, y el proceso que lo contiene.

	if (isOpen(path)) {

		bool condicion(void* element) {

			t_processFileDescriptor* unPFD = element;

			return strcmp(unPFD->fileDescriptor->path, path);

		}

		t_processFileDescriptor* pfd = list_find(process->fileDescriptorList,condicion);

		decrementarOpen(pfd->fileDescriptor);

		removerPFD_Lista(pfd,process);

			logInfo("Se ha cerrado el archivo abierto %s perteneciente al proceso %d",
					path, process->pid);

	}else
			logInfo("El archivo %s no es un archivo abierto del proceso %d",
								path, process->pid);


}

bool isOpen(char* path) {

	bool condicion(void* element) {
		t_fileDescriptor* unFD = element;
		return strcmp(unFD->path,path);
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

readFile(uint32_t fd, int offset, size_t tamanio, Process* process){

	char* path = obtenerPath(fd);

	//TODO: Esta funcion envia y devuelve el package
	//TODO: hay que serializar los datos y meterlos en el stream ¿Estructuras?

	 int socket = process->kernelStruct->socketClientFileSystem;

	Package* package = solicitudAlFileSystem(socket,
	COD_OBTENER_DATOS,stream); //TODO:(path, offset,tamanio)

	if(package == NULL){
			logInfo("Error en solicitud a FileSystem");
			destroyPackage(package);
		}
		destroyPackage(package); //TODO: se podria poner aca solo

	package = createAndReceivePackage(socket);

	//TODO:	si surgio error ejecutar la rutina de finalizacion de proceso con el codigo de error adecuado
	//si no hay error enviar a ready (ver donde se almacena el dato obtenido del fs)
}


*/

Package* solicitudAlFileSystem(int socket,uint32_t msgCode, char* stream){


	Package* package = createAndSendPackage(socket, msgCode,strlen(stream),stream);

	if(package!=NULL){
		logInfo("Paquete enviado correctamente");

	}
	else{
			logInfo("Error al enviar paquete");
	}

	destroyPackage(package);

	package = createAndReceivePackage(socket);

	return package;

}








