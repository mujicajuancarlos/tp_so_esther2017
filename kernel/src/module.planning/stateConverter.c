/*
 * stateConverter.c
 *
 *  Created on: 10/6/2017
 *      Author: utnso
 */

#include "stateConverter.h"


char* stateIndexToString(int stateCode) {
	switch (stateCode) {
	case STATE_CODE_NEW:
		return STATE_NEW;
		break;
	case STATE_CODE_READY:
		return STATE_READY;
		break;
	case STATE_CODE_EXECUTE:
		return STATE_EXECUTE;
		break;
	case STATE_CODE_BLOCK:
		return STATE_BLOCK;
		break;
	case STATE_CODE_EXIT:
		return STATE_EXIT;
		break;
	default:
		return STATE_NOTFOUND;
		break;
	}
}

int stringToStateIndex(char* state) {
	if (string_equals_ignore_case(state, STATE_NEW))
		return STATE_CODE_NEW;
	if (string_equals_ignore_case(state, STATE_READY))
		return STATE_CODE_READY;
	if (string_equals_ignore_case(state, STATE_EXECUTE))
		return STATE_CODE_EXECUTE;
	if (string_equals_ignore_case(state, STATE_BLOCK))
		return STATE_CODE_BLOCK;
	if (string_equals_ignore_case(state, STATE_EXIT))
		return STATE_CODE_EXIT;
	return STATE_CODE_NOTFOUND;
}
