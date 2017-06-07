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
		return "new";
	if (list_find(states->ready->elements, condicion) != NULL)
		return "ready";
	if (list_find(states->execute, condicion) != NULL)
		return "execute";
	if (list_find(states->block, condicion) != NULL)
		return "block";
	if (list_find(states->exit->elements, condicion) != NULL)
		return "exit";

	return "notFound";
}
