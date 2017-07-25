/*
 * cpuAdministrator.c
 *
 *  Created on: 23/5/2017
 *      Author: utnso
 */

#include "cpuAdministrator.h"

sem_t freeCPU_sem;
t_list* cpuList;
pthread_mutex_t cpuListMutex;

void initializeCpuAdministrator() {
	pthread_mutex_init(&cpuListMutex, NULL);
	sem_init(&freeCPU_sem, 1, 0);
	cpuList = list_create();
}

void destroyCpuAdministrator() {
	pthread_mutex_destroy(&cpuListMutex);
	sem_destroy(&freeCPU_sem);
	list_destroy_and_destroy_elements(cpuList, (void*) destroyCPU);
}

/*
 * Marco la cpu como libre (protejo la operacion completa con mutex)
 * incremento el semaforo
 * debo ser usado cuado (fin ejecucion, fin quantum, syscall, error en cpu)
 */
void markFreeCPU(CPU* cpu) {
	pthread_mutex_lock(&cpuListMutex);
	cpu->process = NULL;
	cpu->libre = true;
	sem_post(&freeCPU_sem);
	logInfo("Se marco como libre la cpu %d", cpu->fileDescriptor);
	pthread_mutex_unlock(&cpuListMutex);
}

/*
 * wait del semaforo
 * busco un cpu libre y lo devuelvo (protejo la busqueda con mutex)
 * debo ser usado en el planificador
 */
CPU* searchAndMarkBusyCPU() {
	CPU* cpu;
	bool condicion(void* element) {
		CPU* anCpu = element;
		return anCpu->libre;
	}
	sem_wait(&freeCPU_sem);
	pthread_mutex_lock(&cpuListMutex);
	cpu = list_find(cpuList, condicion);
	cpu->libre = false;
	logInfo("Se marco como ocupada la cpu %d", cpu->fileDescriptor);
	pthread_mutex_unlock(&cpuListMutex);
	return cpu;
}

void addCPU(CPU* cpu) {
	pthread_mutex_lock(&cpuListMutex);
	list_add(cpuList, cpu);
	sem_post(&freeCPU_sem);
	pthread_mutex_unlock(&cpuListMutex);
}

/*
 * realizo el wait solo si la cpu es valida y esta marcada como libre
 */
void removeCPU(CPU* cpu) {
	int cpuFD = cpu->fileDescriptor;
	close(cpuFD);
	bool condition(void* element) {
		CPU* otherCpu = element;
		return otherCpu->fileDescriptor == cpuFD;
	}
	if (cpu != NULL) {
		if (cpu->libre == true) {
			sem_wait(&freeCPU_sem);
		}
		pthread_mutex_lock(&cpuListMutex);
		list_remove_and_destroy_by_condition(cpuList, condition,
					(void*) destroyCPU);
		pthread_mutex_unlock(&cpuListMutex);
	}
}
