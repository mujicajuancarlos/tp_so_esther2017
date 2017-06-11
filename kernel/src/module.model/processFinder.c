#include "processFinder.h"

/*
 * devuelvo en la misma consulta un puntero a process
 * y ademas seteo el stateIndex
 * si no se encontro un proceso devuelvo null
 */
Process* getProcessAndStateIndexByPID(int pidParam, int* stateIndex) {
	t_planningStates* states = getStates();
	bool condicion(void* element) {
		Process* aProcess = element;
		return aProcess->pid == pidParam;
	}
	Process* encontrado;
	if ((encontrado = list_find(states->new, condicion)) != NULL){
		*stateIndex = STATE_CODE_NEW;
		return encontrado;
	}
	if ((encontrado = list_find(states->ready->elements, condicion)) != NULL){
		*stateIndex = STATE_CODE_READY;
		return encontrado;
	}
	if ((encontrado = list_find(states->execute, condicion)) != NULL){
		*stateIndex = STATE_CODE_EXECUTE;
		return encontrado;
	}
	if ((encontrado = list_find(states->block, condicion)) != NULL){
		*stateIndex = STATE_CODE_BLOCK;
		return encontrado;
	}
	if ((encontrado = list_find(states->exit->elements, condicion)) != NULL){
		*stateIndex = STATE_CODE_EXIT;
		return encontrado;
	}
	return NULL;
}

int getProcessStateIndex(Process* proceso) {
	t_planningStates* states = getStates();
	bool condicion(void* element) {
		Process* aProcess = element;
		return aProcess == proceso;
	}
	if (list_find(states->new, condicion) != NULL)
		return STATE_CODE_NEW;
	if (list_find(states->ready->elements, condicion) != NULL)
		return STATE_CODE_READY;
	if (list_find(states->execute, condicion) != NULL)
		return STATE_CODE_EXECUTE;
	if (list_find(states->block, condicion) != NULL)
		return STATE_CODE_BLOCK;
	if (list_find(states->exit->elements, condicion) != NULL)
		return STATE_CODE_EXIT;

	return STATE_CODE_NOTFOUND;
}
