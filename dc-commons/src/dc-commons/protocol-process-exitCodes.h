/*
 * protocol-errors.h
 *
 *  Created on: 21/5/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_PROTOCOL_PROCESS_EXITCODES_H_
#define SRC_DC_COMMONS_PROTOCOL_PROCESS_EXITCODES_H_

/**
 * STATUS CODES definidos en la pag 18
 */

#define OK_STATUS 0
#define ERROR_WITHOUT_RESOURCES -1
#define ERROR_FILE_NOT_FOUND -2
#define ERROR_FILE_READ_REFUSED -3
#define ERROR_FILE_WRITE_REFUSED -4
#define ERROR_MEMORY_EXCEPTION -5
#define ERROR_END_PROCESS_BY_DISCONECT -6
#define ERROR_END_PROCESS_BY_REQUEST -7
#define ERROR_MEMORY_ALLOC_EXCEEDED -8
#define ERROR_ADD_PAGE_REFUSED -9

#define ERROR_NETWORK_DISCONNECTED -10
#define ERROR_MESSAGE_CODE_UNKNOWN -11

#define ERROR_UNKNOWN -20


#endif /* SRC_DC_COMMONS_PROTOCOL_PROCESS_EXITCODES_H_ */
