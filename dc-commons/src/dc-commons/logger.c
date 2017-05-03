/*
 * logger.c
 *
 *  Created on: 10/04/2017
 *      Author: JuanCarlosMujica
 */

#include "logger.h"

pthread_mutex_t logMutex;
t_log* logger;

void initLogMutex(char* file, char* program_name, bool is_active_console, t_log_level level){
	pthread_mutex_init(&logMutex,NULL);
	logger = log_create(file, program_name, is_active_console, level);
}

void logDestroy(){
	log_destroy(logger);
	pthread_mutex_destroy(&logMutex);
}

void logInfo(const char* message_template, ...){
	pthread_mutex_lock(&logMutex);
	va_list arguments;
	va_start(arguments, message_template);
	char* message = string_from_vformat(message_template, arguments);
	log_info(logger,message);
	free(message);
	va_end(arguments);
	pthread_mutex_unlock(&logMutex);
}

void logDebug(const char* message_template, ...){
	pthread_mutex_lock(&logMutex);
	va_list arguments;
	va_start(arguments, message_template);
	char* message = string_from_vformat(message_template, arguments);
	log_debug(logger,message);
	free(message);
	va_end(arguments);
	pthread_mutex_unlock(&logMutex);
}

void logTrace(const char* message_template, ...){
	pthread_mutex_lock(&logMutex);
	va_list arguments;
	va_start(arguments, message_template);
	char* message = string_from_vformat(message_template, arguments);
	log_trace(logger,message);
	free(message);
	va_end(arguments);
	pthread_mutex_unlock(&logMutex);
}

void logWarning(const char* message_template, ...){
	pthread_mutex_lock(&logMutex);
	va_list arguments;
	va_start(arguments, message_template);
	char* message = string_from_vformat(message_template, arguments);
	log_warning(logger,message);
	free(message);
	va_end(arguments);
	pthread_mutex_unlock(&logMutex);
}

void logError(const char* message_template, ...){
	pthread_mutex_lock(&logMutex);
	va_list arguments;
	va_start(arguments, message_template);
	char* message = string_from_vformat(message_template, arguments);
	log_error(logger,message);
	free(message);
	va_end(arguments);
	pthread_mutex_unlock(&logMutex);
}
