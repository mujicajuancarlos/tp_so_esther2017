/*
 * fileSystemRequest.c
 *
 *  Created on: 1/6/2017
 *      Author: utnso
 */

#include "fileSystemRequest.h"

bool validateFile(kernel_struct* kernelStruct, t_fileDescriptor* fileDescriptor) {
	Package* package = createAndSendPackage(
			kernelStruct->socketClientFileSystem, COD_VALIDAR_ARCHIVO,
			strlen(fileDescriptor->path), fileDescriptor->path);
	if (package != NULL) {
		destroyPackage(package);
		package = createAndReceivePackage(kernelStruct->socketClientFileSystem);
		if (package != NULL) {
			bool response = deserialize_int(package->stream);
			if (response) {
				logInfo(
						"El Archivo de fileDescriptor %d ha sido validado correctamente y se encuentra en la ruta %s",
						fileDescriptor->fd, fileDescriptor->path);
				destroyPackage(package);
				return true;
			} else
				logInfo(
						"El Archivo de fileDescriptor %d no se encuentra en la ruta %s",
						fileDescriptor->fd, fileDescriptor->path);
		} else
			logInfo("No se pudo recibir los datos del FileSystem");
	} else
		logInfo("No se pudo enviar los datos al FileSystem");

	destroyPackage(package);
	return false;

}
/*
void openFile(kernel_struct* kernelStruct,char* path,char* openMode){
	t_fileDescriptor* file = createNew_t_fileDescriptor(path);

	if(validateFile(kernelStruct,file)){
		Package* package = createAndSendPackage(
					kernelStruct->socketClientFileSystem, COD_CREAR_ARCHIVO,
					sizeof(t_fileDescriptor), &file);//TODO: Funcion para serializar el t_fileDescriptor
		if (package != NULL) {
			agregarFD_Alista(file);
			logInfo("El Archivo de fileDescriptor %d ha sido abierto correctamente y se encuentra en la ruta %s",file->fd, file->path);
			//TODO: Agregar a tabla de proceso.
		}else
			logInfo("No se pudo enviar los datos al FileSystem");

	}


}


void createFile(kernel_struct* kernelStruct,char* path,char* openMode){


}
*/
