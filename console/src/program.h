/*
 * program.h
 *
 *  Created on: 23/4/2017
 *      Author: utnso
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <stddef.h>
#include <stdlib.h>
#include "console.h"

typedef struct {
	int pid;
	time_t startDate;
	time_t endDate;
	int fd_client;
	int printSize;
	char* sourceCodePath;
	console_struct* console;
} Program;

void printHeaderProgram();

void printProgram(Program* program);

void printIntervalTime(time_t* startTime, time_t* endTime);

Program* createNewProgram(console_struct* console, char* sourceCodePath);

void destroyProgram(Program* program);

#endif /* PROGRAM_H_ */
