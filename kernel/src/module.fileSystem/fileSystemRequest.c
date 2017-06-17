/*
 * fileSystemRequest.c
 *
 *  Created on: 1/6/2017
 *      Author: utnso
 */

#include "fileSystemRequest.h"

bool validateFileExistence(Process* process, char* path) {

	int socketFD = process->kernelStruct->socketClientFileSystem;

	bool busqueda = isOpen(path); //valida si existe fijandose si esta en la tabla global de fd

	if (busqueda) {

		logInfo(
				"El Archivo ha sido validado correctamente y se encuentra en la ruta %s",
				path);
		return busqueda;
	}

	Package* package = createAndSendPackage(socketFD, COD_VALIDAR_ARCHIVO,
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

	bool existencia = validateFileExistence(process, path);

	if (!existencia) {

		if (string_equals_ignore_case(openMode, "c")) {

			Package* package = solicitudAlFileSystem(socketFS,
			COD_CREAR_ARCHIVO, path);
			destroyPackage(package);

			package = createAndReceivePackage(socketFS);

			if (package != NULL) {

				bool response = deserialize_int(package->stream);

				if (response) {

					t_globalFile* file = create_t_globalFile(path);

					incrementarOpen(file);

					addGlobalFile(file);

					t_processFile* pfd =
							createNew_t_processFileDescriptor(openMode, file);
					agregarPFD_Alista(process, pfd);

					logInfo(
							"El Archivo de fileDescriptor %d ha sido creado correctamente y se encuentra en la ruta %s",
							file->fd, file->path);

					return file->fd;
				} else {
					logInfo("El Archivo no se pudo crear");
					return COD_ERROR_FS_COULDNT_CREATE_FILE;
				}
			} else {
				logInfo("Error en comunicarse con File System");
				return COD_ERROR_FS_PACKAGE_COMMUNICATION;
			}

		} else {
			logInfo(
					"El Archivo que se solicitó abrir no cuenta con los permisos necesarios para ser creado");
			return COD_ERROR_PERMISSIONS_DENIED;
		}
	}
	t_globalFile* file = create_t_globalFile(path);

	incrementarOpen(file);

	addGlobalFile(file);

	t_processFile* pfd = createNew_t_processFileDescriptor(openMode,
			file);
	agregarPFD_Alista(process, pfd);

	logInfo("El Archivo de fileDescriptor %d ha sido creado correctamente y"
			" se encuentra en la ruta %s", file->fd, file->path);

	return file->fd;

}

int deleteFile(Process* process, char* path) {
	if (isOpen(path)) {

		bool condicion(void* element) {
			t_processFile* unPFD = element;
			return string_equals_ignore_case(unPFD->globalFile->path, path);
		}

		t_processFile* processFDFound = list_find(
				process->files, condicion);

		if (processFDFound != NULL) {
			solicitudAlFileSystem(process->kernelStruct->socketClientFileSystem,
			COD_BORRAR_ARCHIVO, path);
			return 0;
		} else {
			logInfo("El proceso %d no tiene abierto el archivo %s",
					process->pid, path);
			return COD_ERROR_FILE_NOT_OPEN_FOR_FILE;
		}
	} else
		logInfo("El archivo %s no se encuentra abiero por ningún proceso",
				path);
	return COD_ERROR_FILE_ISNOT_OPEN;

}

int writeFile(Process* process, int fd, int offset, char*buffer, size_t size) {
	char* path = getPathFromFD(fd);

	int socket = process->kernelStruct->socketClientFileSystem;

	t_Write* write = malloc(sizeof(t_Write));

	write->buffer = buffer;
	write->offset = offset;
	write->path = path;
	write->size = size;

	char* stream = serialize_t_Write(write);

	Package* package = solicitudAlFileSystem(socket,
	COD_GUARDAR_DATOS, stream); //TODO: path,offset,size,buffer

	if (package != NULL) {
		logInfo(
				"Se ha enviado la solicitud de escritura en el archivo de fileDescriptor %d por parte del proceso PID: %d",
				fd, process->pid);
	} else {
		return COD_ERROR_FS_PACKAGE_COMMUNICATION;
	}

	destroyPackage(package);
	package = createAndReceivePackage(socket);

	if (package != NULL) {
		int respuesta = deserialize_int(package->stream);
		return respuesta;

	} else {
		return COD_ERROR_FS_PACKAGE_COMMUNICATION;
	}

}

int readFile(Process* process, int fileDesc_file, int offset, size_t size) {

	int socketFS = process->kernelStruct->socketClientFileSystem;
	char* path = getPathFromFD(fileDesc_file);

	t_Read* read = malloc(sizeof(t_Read));

	read->path = path;
	read->offset = offset;
	read->size = size;

	char* stream = serialize_t_Read(read);

	Package* package = solicitudAlFileSystem(socketFS,
	COD_OBTENER_DATOS, stream); //TODO:(path,offset,size)  Struct

	if (package != NULL) {
		logInfo(
				"Se ha enviado la solicitud de lectura en el archivo de path %s por parte del proceso PID: %d",
				path, process->pid);
	}
	destroyPackage(package);
	package = createAndReceivePackage(socketFS);

	if (package != NULL) {
		int respuesta = deserialize_int(package->stream);
		return respuesta;

	} else {
		return COD_ERROR_FS_PACKAGE_COMMUNICATION;
	}

	//TODO:	si surgio error ejecutar la rutina de finalizacion de proceso con el codigo de error adecuado
	//si no hay error enviar a ready (ver donde se almacena el dato obtenido del fs)
}

Package* solicitudAlFileSystem(int socket, uint32_t msgCode, char* stream) {

	Package* package = createAndSendPackage(socket, msgCode, strlen(stream),
			stream);
	if (package != NULL) {
		logInfo("Paquete enviado correctamente");
	} else {
		logInfo("Error al enviar paquete");
	}

	destroyPackage(package);
	package = createAndReceivePackage(socket);
	return package;

}

