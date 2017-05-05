/*

 * planificador.h



 *

 *  Created on: 17/9/2015

 *      Author: utnso

 */

#ifndef PLANIFICADOR_H_

#define PLANIFICADOR_H_



#include <commons/config.h>
#include <dc-commons/logger.h>
#include <stdint.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/string.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <stdlib.h>

//VERIFICAR SI FALTAN INCLUDES


//Char del orden de la Estructuras para serializar/desserializar

#define SECUENCIA_PCB "hhhhs"

//estados del pcb

#define LISTO 1
#define BLOQUEADO 2
#define FINALIZADO 4
#define EJECUTANDO 3


/* el estado puede ser:

 * 1: listo

 * 2: bloqueado

 * 3: ejecutando

 * 4: finalizado

 */

typedef struct PCB {
	int PID;
//	int estado;
//	int pc;
//	int quantum;
	char *contextoEjecucion;

} t_pcb;



typedef struct NODO_MEM {

	uint32_t pid;
	char instruccion[20];

} __attribute__ ((packed)) t_nodo_mem;

typedef struct hilo {

	pthread_t thread;
	char m[100];

	int  r;

} t_hilos;

int contadorPID = 0;
pthread_mutex_t readyListMutex;
int clientePlanificador = 0;
int servidorPlanificador = 0;
int socketMemoria = -1;
sem_t mutexMemoria;
sem_t semProgramas;
char comando[100];
int quantumcfg = 0;


//void levantarCfg();

//void* consola();

//int programaValido(char * programa);

void agregarALista(char * programa);

void* enviarPCBaMemoria();

int enviarMensajeDePCBaMemoria(int socketMemoria, t_pcb * nodoPCB);

void empaquetarPCB(unsigned char *buffer,t_pcb * nodoPCB);

void desempaquetarPCB(unsigned char *buffer,t_pcb * nodoPCB);

//void interpretarLinea(t_resp_Memoria_plan * nodoRespuesta);

//static t_hilos *hilo_create(pthread_t thread, char * m, int  r);

//static t_pcb *pcb_create(int PID, char * contextoDeEjecucion);


t_list * listaDeListo;

t_list * listaDeBloqueado;

t_list * listaDeEjecutado;

t_list * listaDeHilos;


#endif
