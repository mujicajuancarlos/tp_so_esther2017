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

void printFileNotFound(char* filePath) {
	printMessage("No se pudo acceder al archivo «%s».", filePath);
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
	lockPrinter();
	puts("__________________________________________________________");
	puts("___________ Bienvenido a Esther - Death Coders ___________");
	puts("__________________ UTN-FRBA SO 1C-2017 ___________________");
	printf("________________<<%*s>>__________________",10 - strlen(processName)/2, processName);
	puts("__________________________________________________________");
	unlockPrinter();
}

void lockPrinter(){
	pthread_mutex_lock(&printerMutex);
}

void unlockPrinter(){
	pthread_mutex_unlock(&printerMutex);
}

void initPrinterMutex() {
	pthread_mutex_init(&printerMutex, NULL);
}

void printerDestroy() {
	pthread_mutex_destroy(&printerMutex);
}
