/*
 * stateConverter.h
 *
 *  Created on: 10/6/2017
 *      Author: utnso
 */

#ifndef MODULE_PLANNING_STATECONVERTER_H_
#define MODULE_PLANNING_STATECONVERTER_H_

#include <commons/string.h>


#define STATE_NEW "new"
#define STATE_READY "ready"
#define STATE_EXECUTE "excecute"
#define STATE_BLOCK "block"
#define STATE_EXIT "exit"
#define STATE_NOTFOUND "notFound"
#define STATE_CODE_NEW 1
#define STATE_CODE_READY 2
#define STATE_CODE_EXECUTE 3
#define STATE_CODE_BLOCK 4
#define STATE_CODE_EXIT 5
#define STATE_CODE_NOTFOUND 6

char* stateIndexToString(int stateCode);

int stringToStateIndex(char* state);

#endif /* MODULE_PLANNING_STATECONVERTER_H_ */
