/*
 * fileSystemRequest.c
 *
 *  Created on: 1/6/2017
 *      Author: utnso
 */

#include "fileSystemRequest.h"

bool validarExistenciaFile(char* path, Process* process) {

	int socketFD = process->kernelStruct->socketClientFileSystem;

	bool busqueda = isOpen(path); //valida si existe fijandose si esta en la tabla global de fd

	if(busqueda){

		logInfo("El Archivo ha sido validado correctamente y se encuentra en la ruta %s",
							path);
		return busqueda;
	}

	Package* package = createAndSendPackage(
			socketFD, COD_VALIDAR_ARCHIVO,
			strlen(path), path);

	if (package != NULL) {

		destroyPackage(package);
		package = createAndReceivePackage(socketFD);

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



int createFile(Process* process, char* path, char* openMode) {

		int socketFS = process->kernelStruct->socketClientFileSystem;

		bool existencia = validarExistenciaFile(path,process);

		if(!existencia){

		if (strcmp(openMode, "c")) { //TODO: Verificar cual va a ser el manejo de flags al final

			Package* package = solicitudAlFileSystem(socketFS,COD_CREAR_ARCHIVO, path);
			destroyPackage(package);

			package=createAndReceivePackage(socketFS);

			if(package!=NULL){

				bool response = deserialize_int(package->stream);

				if(response){

					t_fileDescriptor* file = createNew_t_fileDescriptor(path);

								incrementarOpen(file);

								agregarFD_Alista(file);

								t_processFileDescriptor* pfd = createNew_t_processFileDescriptor(
										openMode, file);
								agregarPFD_Alista(process, pfd);

								logInfo(
										"El Archivo de fileDescriptor %d ha sido creado correctamente y se encuentra en la ruta %s",
										file->fd, file->path);

								return file->fd;
				}
				else
				{
					logInfo("El Archivo no se pudo crear");
				}
			}
			else {
				logInfo("Error en comunicarse con File System");
			}

		} else {
			logInfo(
					"El Archivo que se solicitó abrir no cuenta con los permisos necesarios para ser creado");
			return -1;
		}
	}
		t_fileDescriptor* file = createNew_t_fileDescriptor(path);

		incrementarOpen(file);

		agregarFD_Alista(file);

		t_processFileDescriptor* pfd = createNew_t_processFileDescriptor(openMode, file);
		agregarPFD_Alista(process, pfd);

		logInfo("El Archivo de fileDescriptor %d ha sido creado correctamente y"
				" se encuentra en la ruta %s",file->fd, file->path);

		return file->fd;

}



int deleteFile(Process* process, char* path) {
		if (isOpen(path)) {

			bool condicion(void* element) {
				t_processFileDescriptor* unPFD = element;
				return string_equals_ignore_case(unPFD->fileDescriptor->path, path);
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




int writeFile(uint32_t fd, int offset,char*buffer, size_t size, Process* process){
	 char* path = obtenerPath(fd);

	 //TODO: Esta funcion envia y devuelve el package
	 //TODO: hay que serializar los datos y meterlos en el stream ¿Estructuras?

	 int socket = process->kernelStruct->socketClientFileSystem;

	 t_Write* write = malloc(sizeof(t_Write));

	 write->buffer = buffer;
	 write->offset = offset;
	 write->path = path;
	 write->size = size;

	 char* stream = serialize_t_Write(write);

	 Package* package = solicitudAlFileSystem(socket,
	 COD_GUARDAR_DATOS,stream); //TODO: path,offset,size,buffer

	 if(package == NULL){
	 logInfo("Error en solicitud a FileSystem");
	 destroyPackage(package);
	 }
	 destroyPackage(package); //TODO: se podria poner aca solo

	 package = createAndReceivePackage(socket);

	 if(package!=NULL){

		 int respuesta = deserialize_int(package->stream); //TODO:agregar logs, finalizacion de la funcion, etc

		 return respuesta;

	 }


	 //TODO Evaluar si hubo error terminar, si no mandar a Ready

	 }

void readFile(Process* process, char* path, char* buffer, size_t size, int offset){

	 //TODO: Esta funcion SOLO ENVIA, la devolucion la manejara el handler correspondiente.
	 //TODO: hay que serializar los datos y meterlos en el stream ¿Estructuras?  Sí.

	 int socketFS = process->kernelStruct->socketClientFileSystem;

	 t_Read* read = malloc(sizeof(t_Read));

	 read->offset = offset;
	 read->path = path;
	 read->size = size;

	 char* stream = serialize_t_Read(read);

	 Package* package = solicitudAlFileSystem(socketFS,
	 COD_OBTENER_DATOS,stream); //TODO:(path,offset,size)  Struct

	 if(package == NULL){
	 logInfo("Error en solicitud a FileSystem");
	 destroyPackage(package);
	 }
	 destroyPackage(package); //TODO: se podria poner aca solo

	// package = createAndReceivePackage(socketFS); TODO: esto no va ya que dice que solo recibe..

	 //TODO:	si surgio error ejecutar la rutina de finalizacion de proceso con el codigo de error adecuado
	 //si no hay error enviar a ready (ver donde se almacena el dato obtenido del fs)
	 }




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




