/*
 * socket-message.c
 *
 *  Created on: 13/4/2017
 *      Author: utnso
 */

#include "socket-message.h"

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
	int buffer_size = sizeof(uint32_t);
	char *buffer = malloc(buffer_size );

	status = receiveMessage(fileDescriptor, buffer, sizeof(package->msgCode));
	memcpy(&(package->msgCode), buffer, buffer_size);
	if (status == SEND_OR_RECEIVE_FAILURE)
		return status;

	status = receiveMessage(fileDescriptor, buffer, sizeof(package->message_long));
	memcpy(&(package->message_long), buffer, buffer_size);
	if (status == SEND_OR_RECEIVE_FAILURE)
			return status;

	package->message = malloc(sizeof(char) * package->message_long);
	status = receiveMessage(fileDescriptor, package->message, package->message_long);
	if (status == SEND_OR_RECEIVE_FAILURE)
			return status;

	free(buffer);
	return SEND_OR_RECEIVE_SUCCESS;
}

int sendMessage(int fileDescriptor, char *message, int sizeOfMessage) {

	int total_bytes_written = 0;
	int bytes_written;

	if ((fileDescriptor == -1) || (message == NULL) || (sizeOfMessage < 1)) {
		error_show("Invalid request FD: %d message: %s sizeOfMessage: %d\n",
				fileDescriptor, message, sizeOfMessage);
		return SEND_OR_RECEIVE_FAILURE;
	}

	while (total_bytes_written < sizeOfMessage) {
		bytes_written = write(fileDescriptor, &message[total_bytes_written],
				sizeOfMessage - total_bytes_written);
		if (bytes_written == -1) {
			error_show("Write failure FD: %d \n", fileDescriptor);
			return SEND_OR_RECEIVE_FAILURE;
		}
		total_bytes_written += bytes_written;
	}

	if (total_bytes_written != sizeOfMessage) {
		error_show("Write failure FD: %d, send %d bytes \n", fileDescriptor,
				total_bytes_written);
		return SEND_OR_RECEIVE_FAILURE;
	}

	return total_bytes_written;
}

int receiveMessage(int fileDescriptor, char *message, int sizeOfMessage) {

	int total_bytes_received = 0;
	int bytes_received;

	if ((fileDescriptor == -1) || (message == NULL) || (sizeOfMessage < 1)) {
		error_show("Invalid request FD: %d message: %s sizeOfMessage: %d\n",
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
				error_show("Received failure FD: %d \n", fileDescriptor);
				return SEND_OR_RECEIVE_FAILURE;
			}
		}
		total_bytes_received += bytes_received;
	}

	if (total_bytes_received != sizeOfMessage) {
		error_show("Received failure FD: %d, send %d bytes \n", fileDescriptor,
				total_bytes_received);
		return SEND_OR_RECEIVE_FAILURE;
	}

	return total_bytes_received;
}
