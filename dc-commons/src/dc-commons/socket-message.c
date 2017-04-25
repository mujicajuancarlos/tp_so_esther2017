/*
 * socket-message.c
 *
 *  Created on: 13/4/2017
 *      Author: utnso
 */

#include "socket-message.h"
#include <stdio.h>

int sendPackage(int fileDescriptor, Package *package) {
	int result;
	char* serializedPkg = serializePackage(package);
	result = sendMessage(fileDescriptor, (char *) serializedPkg,
			sizePackage(package));
	free(serializedPkg);
	if (result == SEND_OR_RECEIVE_FAILURE)
		return result;

	return SEND_OR_RECEIVE_SUCCESS;
}

int receivePackage(int fileDescriptor, Package *package) {

	int status = 0;
	size_t buffer_size = sizeof(uint32_t);
	char *buffer = malloc(buffer_size );

	status = receiveMessage(fileDescriptor, buffer, buffer_size);
	memcpy(&(package->msgCode), buffer, buffer_size);
	if (status == SEND_OR_RECEIVE_FAILURE){
		return status;
	}

	status = receiveMessage(fileDescriptor, buffer, buffer_size);
	memcpy(&(package->size), buffer, buffer_size);
	if (status == SEND_OR_RECEIVE_FAILURE)
			return status;

	package->stream = malloc(sizeof(char) * package->size);
	status = receiveMessage(fileDescriptor, package->stream, package->size);
	if (status == SEND_OR_RECEIVE_FAILURE)
			return status;

	free(buffer);
	return SEND_OR_RECEIVE_SUCCESS;
}

int sendMessage(int fileDescriptor, char *message, int sizeOfMessage) {

	int total_bytes_written = 0;
	int bytes_written;

	if ((fileDescriptor == -1) || (message == NULL) || (sizeOfMessage < 1)) {
		error_show("Error de parametros. No se puede enviar porque FD->%d message->%s Tamaño->%d\n",
				fileDescriptor, message, sizeOfMessage);
		return SEND_OR_RECEIVE_FAILURE;
	}

	while (total_bytes_written < sizeOfMessage) {
		bytes_written = write(fileDescriptor, &message[total_bytes_written],
				sizeOfMessage - total_bytes_written);
		if (bytes_written == -1) {
			error_show("No se pudo escribir en FD: %d \n", fileDescriptor);
			return SEND_OR_RECEIVE_FAILURE;
		}
		total_bytes_written += bytes_written;
	}

	if (total_bytes_written != sizeOfMessage) {
		error_show("Se envio una cantidad de bytes distinta a lo esperado FD->%d, enviado->%d esperado->%d\n", fileDescriptor,
				total_bytes_written, sizeOfMessage);
		return SEND_OR_RECEIVE_FAILURE;
	}

	return total_bytes_written;
}

int receiveMessage(int fileDescriptor, char *message, int sizeOfMessage) {

	int total_bytes_received = 0;
	int bytes_received;

	if ((fileDescriptor == -1) || (message == NULL) || (sizeOfMessage < 1)) {
		error_show("Error de parametros. No se puede recibir mensajes porque FD->%d mensaje->%s Tamaño->%d\n",
				fileDescriptor, message, sizeOfMessage);
		return SEND_OR_RECEIVE_FAILURE;
	}

	while (total_bytes_received < sizeOfMessage) {
		bytes_received = read(fileDescriptor, &message[total_bytes_received],
				sizeOfMessage - total_bytes_received);

		switch (errno) { //errno indica el tipo de error
		case EINTR: //tipo EINTR si hubo interrupcion en el sistema
		case EAGAIN: //tipo EAGAIN si el socket no esta disponible
			usleep(100); //demora de 100 microsegundos y vuelo a intentar leer
			break;
		default:
			if (bytes_received == -1) {
				error_show("Error al leer el mensaje de FD->%d \n", fileDescriptor);
				return SEND_OR_RECEIVE_FAILURE;
			}
		}
		total_bytes_received += bytes_received;
	}

	if (total_bytes_received != sizeOfMessage) {
		error_show("Se recibio una cantidad de bytes distinta a lo esperado FD->%d, recibido->%d esperado->%d\n", fileDescriptor,
				total_bytes_received, sizeOfMessage);
		return SEND_OR_RECEIVE_FAILURE;
	}

	return total_bytes_received;
}
