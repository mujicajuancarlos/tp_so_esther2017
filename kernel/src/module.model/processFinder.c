#include "processFinder.h"

Process* getProcessByPID(int pidParam) {
	t_planningStates* states = getStates();
	bool condicion(void* element) {
		Process* aProcess = element;
		return aProcess->pid == pidParam;
	}
	Process* encontrado;
	if ((encontrado = list_find(states->new, condicion)) != NULL)
		return encontrado;
	if ((encontrado = list_find(states->ready->elements, condicion)) != NULL)
		return encontrado;
	if ((encontrado = list_find(states->execute, condicion)) != NULL)
		return encontrado;
	if ((encontrado = list_find(states->block, condicion)) != NULL)
		return encontrado;
	if ((encontrado = list_find(states->exit->elements, condicion)) != NULL)
		return encontrado;

	return NULL;
}

char* getProcessState(Process* proceso) {
	t_planningStates* states = getStates();
	bool condicion(void* element) {
		Process* aProcess = element;
		return aProcess == proceso;
	}
	if (list_find(states->new, condicion) != NULL)
		return STATE_NEW;
	if (list_find(states->ready->elements, condicion) != NULL)
		return STATE_READY;
	if (list_find(states->execute, condicion) != NULL)
		return STATE_EXECUTE;
	if (list_find(states->block, condicion) != NULL)
		return STATE_BLOCK;
	if (list_find(states->exit->elements, condicion) != NULL)
		return STATE_EXIT;

	return STATE_NOTFOUND;
}
