/*
 * stdout-printer.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "stdout-printer.h"

pthread_mutex_t printerMutex;

void printMessage(const char* message_template, ...) {
	lockPrinter();
	va_list arguments;
	va_start(arguments, message_template);
	char* message = string_from_vformat(message_template, arguments);
	puts(message);
	printf(" > ");
	free(message);
	va_end(arguments);
	unlockPrinter();
}

void printNewLine() {
	printMessage("");
}

void clearScreem() {
	lockPrinter();
	int systemRet = system("clear");
	if(systemRet != -1){
	  logInfo("Se ejecuto system(clear) correctamente");
	}else{
		logWarning("El comando system(clear) no se pudo ejecutar");
	}

	unlockPrinter();
}

void printPidNotFound(int pid) {
	printMessage("No se encontro un proceso con pid %d.\n", pid);
}

void printFileNotFound(char* filePath) {
	printMessage("No se pudo acceder al archivo «%s».", filePath);
}

void printInvalidCommand(char* command) {
	if (command != NULL)
		printMessage(
				"«%s» no es un comando válido.\nSi necesita ayuda use: «%s».",
				command, HELP_COMMAND);
}

void printInvalidArguments(char* argument, char* command) {
	printMessage(
			"El argumento «%s» no es valido para el comando «%s».\nSi necesita ayuda use: «%s».",
			argument, command, HELP_COMMAND);
}

void printInvalidOptions(char* option, char* command) {
	printMessage(
			"La opción «%s» no es valida para el comando «%s».\nSi necesita ayuda use: «%s».",
			option, command, HELP_COMMAND);
}

void printWelcome(char* processName) {
	int tab = 10 - (strlen(processName) / 2);
	lockPrinter();
	puts("__________________________________________________________");
	puts("___________ Bienvenido a Esther - Los 5 Fantasticos ___________");
	puts("__________________ UTN-FRBA SO 1C-2017 ___________________");
	printf("______________<<< %*s%s%*s >>>_______________\n", tab, "",
			processName, tab, "");
	puts("__________________________________________________________");
	unlockPrinter();
}

void lockPrinter() {
	pthread_mutex_lock(&printerMutex);
}

void unlockPrinter() {
	pthread_mutex_unlock(&printerMutex);
}

void initPrinterMutex() {
	pthread_mutex_init(&printerMutex, NULL);
}

void printerDestroy() {
	pthread_mutex_destroy(&printerMutex);
}
