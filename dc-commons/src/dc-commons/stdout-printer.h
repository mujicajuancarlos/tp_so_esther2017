/*
 * stdout-printer.h
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_STDOUT_PRINTER_H_
#define SRC_DC_COMMONS_STDOUT_PRINTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <commons/string.h>

#define HELP_COMMAND "-help"

/**
 * @NAME: printMessage
 * @DESC: usalo como un printf, pero sin preocuparte de la sincronizacion,
 * @PARAMS:
 * 		char*
 * 		arguments
 */
void printMessage(const char* message_template, ...);

/**
 * @NAME: lockPrinter
 * @DESC: lock printer mutex
 */
void lockPrinter();

/**
 * @NAME: lockPrinter
 * @DESC: unlock printer mutex
 */
void unlockPrinter();

/**
 * @NAME: initPrinterMutex
 * @DESC: inicializa el printer mutex
 */
void initPrinterMutex();

/**
 * @NAME: printerDestroy
 * @DESC: destruye el printer mutex
 */
void printerDestroy();

void printNewLine();

void printFileNotFound(char* filePath);

void printInvalidArguments(char* argument, char* command);

void printInvalidOptions(char* option, char* command);

void printWelcome(char* processName);

#endif /* SRC_DC_COMMONS_STDOUT_PRINTER_H_ */
