/*
 * handler-user.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "handler-user.h"

void handleUserRequest(kernel_struct* kernelStruct){

	bool exit = false;
	bool shouldCompareCommand;
	while (!exit) {
		printNewLine(kernelStruct->stdoutMutex);
		char** commands = get_user_commands();
		shouldCompareCommand = true;

		if (*commands != NULL) {
			if (shouldCompareCommand
					&& equal_user_command(commands[0], COD_CONSOLE_HELP)) {
				shouldCompareCommand = false;
				printCommandsHelp(kernelStruct->stdoutMutex);
			}
		}

		if (shouldCompareCommand) {
			pthread_mutex_lock(&(kernelStruct->stdoutMutex));
			printf("«%s» no es un comando válido. Si necesita ayuda use: «%s»",
					commands[0], COD_CONSOLE_HELP);
			pthread_mutex_unlock(&(kernelStruct->stdoutMutex));
		}

		free_user_commands(commands);
	}
}
