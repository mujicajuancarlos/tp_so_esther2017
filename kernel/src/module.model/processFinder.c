
#include "processFinder.h"


Process* getProcessByPID(int pidParam){
	t_planningStates* states = getStates();

	bool condicion(void* element) {
		Process* aProcess = element;
		return aProcess->pid == pidParam;
	}

	Process* encontrado;

	while (encontrado == 0){
		encontrado = list_find(states->new,condicion);
		encontrado = list_find(states->ready->elements,condicion);
		encontrado = list_find(states->execute,condicion);
		encontrado = list_find(states->block->elements,condicion);
		encontrado = list_find(states->exit->elements,condicion);
	}

	return encontrado;
}

char*  getProcessState(Process* proceso){
	t_planningStates* states = getStates();

		bool condicion(void* element) {
			Process* aProcess = element;
			return aProcess == proceso;
		}

		Process* new = list_find(states->new,condicion);
		Process* ready = list_find(states->ready->elements,condicion);
		Process* execute = list_find(states->execute,condicion);
		Process* block = list_find(states->block->elements,condicion);
		Process* exit = list_find(states->exit->elements,condicion);


		if (new != 0){
			return "new";
		}
		else if (ready!= 0){
			return "ready";
		}
		else if (execute != 0){
			return "execute";
		}
		else if (block != 0){
			return "block";
		}
		else if (exit != 0){
			return "exit";
		}
		else
			return "NO ENCONTRADO";

}
