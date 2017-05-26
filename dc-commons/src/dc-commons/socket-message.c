/*
 * socket-message.c
 *
 *  Created on: 13/4/2017
 *      Author: utnso
 */

#include "socket-message.h"

Package* createAndSendPackage(int fileDescriptor, uint32_t msgCode,
		uint32_t size, char* stream) {
	Package* package;
	int result;
	package = createPackage(msgCode, size, stream);
	result = sendPackage(fileDescriptor, package);
	if (result == SEND_OR_RECEIVE_SUCCESS)
		return package;
	else {
		destroyPackage(package);
		return NULL;
	}
}

Package* createAndReceivePackage(int fileDescriptor) {
	Package* package;
	int result;
	package = createEmptyPackage();
	result = receivePackage(fileDescriptor, package);
	if (result == SEND_OR_RECEIVE_SUCCESS)
		return package;
	else {
		destroyPackage(package);
		return NULL;
	}
}

int sendPackage(int fileDescriptor, Package *package) {
	int result;
	char* serializedPkg = serializePackage(package);
	result = sendMessage(fileDescriptor, serializedPkg, sizePackage(package),
			0);
	free(serializedPkg);
	if (result == SEND_OR_RECEIVE_FAILURE)
		return result;

	return SEND_OR_RECEIVE_SUCCESS;
}

int receivePackage(int fileDescriptor, Package *package) {

	int status = 0;
	size_t buffer_size = sizeof(uint32_t);
	char *buffer = malloc(buffer_size);

	status = receiveMessage(fileDescriptor, buffer, buffer_size, 0);
	if (status == SEND_OR_RECEIVE_FAILURE)
		return status;
	memcpy(&(package->msgCode), buffer, buffer_size);

	status = receiveMessage(fileDescriptor, buffer, buffer_size, 0);
	if (status == SEND_OR_RECEIVE_FAILURE)
		return status;
	memcpy(&(package->size), buffer, buffer_size);

	if (package->size > 0) {
		buffer_size = sizeof(char) * package->size;
		package->stream = malloc(buffer_size);
		status = receiveMessage(fileDescriptor, package->stream, buffer_size,
				0);
		if (status == SEND_OR_RECEIVE_FAILURE)
			return status;
	}

	free(buffer);
	return SEND_OR_RECEIVE_SUCCESS;
}

int sendMessage(int socket, char *buffer, int sizeOfMessage, int flags) {

	int total_bytes_written = 0;
	int bytes_written = 0;
	char *aux_buffer = buffer;

	if ((socket == -1) || (buffer == NULL) || (sizeOfMessage < 1)) {
		error_show(
				"Error de parametros. No se puede enviar porque FD->%d message->%s Tamaño->%d\n",
				socket, buffer, sizeOfMessage);
		return SEND_OR_RECEIVE_FAILURE;
	}

	while (total_bytes_written < sizeOfMessage) {

		bytes_written = send(socket, aux_buffer,
				sizeOfMessage - total_bytes_written, flags);

		if (bytes_written == 0)
			logWarning("La funcion 'recv(...)' retorno 0, FD: %d\n", socket);

		if (bytes_written == -1) {
			error_show(
					"La funcion 'send(...)' retorno -1, FD: %d, bytes enviados antes del error: %d\n",
					socket, total_bytes_written);
			return SEND_OR_RECEIVE_FAILURE;
		}

		total_bytes_written += bytes_written;
		aux_buffer += bytes_written;
	}

	if (total_bytes_written != sizeOfMessage) {
		error_show(
				"Se envio una cantidad de bytes distinta a lo esperado FD->%d, enviado->%d esperado->%d\n",
				socket, total_bytes_written, sizeOfMessage);
		return SEND_OR_RECEIVE_FAILURE;
	}

	return total_bytes_written;
}

int receiveMessage(int socket, char *buffer, int sizeOfMessage, int flags) {

	int total_bytes_received = 0;
	int bytes_received = 0;
	char *aux_buffer = buffer;

	if ((socket == -1) || (buffer == NULL) || (sizeOfMessage < 1)) {
		error_show(
				"Error de parametros. No se puede recibir mensajes porque FD->%d mensaje->%s Tamaño->%d\n",
				socket, buffer, sizeOfMessage);
		return SEND_OR_RECEIVE_FAILURE;
	}

	while (total_bytes_received < sizeOfMessage) {

		bytes_received = recv(socket, aux_buffer,
				sizeOfMessage - total_bytes_received, flags);

		if (bytes_received == 0)
			logWarning("La funcion 'recv(...)' retorno 0, FD: %d\n", socket);

		switch (errno) { //errno indica el tipo de error
		case EINTR: //tipo EINTR si hubo interrupcion en el sistema
		case EAGAIN: //tipo EAGAIN si el socket no esta disponible
			usleep(100); //demora de 100 microsegundos y vuelo a intentar leer
			break;
		default:
			if (bytes_received == -1) {
				error_show(
						"La funcion 'recv(...)' retorno -1, FD: %d, bytes recibidos antes del error: %d\n",
						socket, total_bytes_received);
				return SEND_OR_RECEIVE_FAILURE;
			}
		}

		total_bytes_received += bytes_received;
		aux_buffer += bytes_received;
	}

	if (total_bytes_received != sizeOfMessage) {
		error_show(
				"Se recibio una cantidad de bytes distinta a lo esperado FD->%d, recibido->%d esperado->%d\n",
				socket, total_bytes_received, sizeOfMessage);
		return SEND_OR_RECEIVE_FAILURE;
	}

	return total_bytes_received;
}
