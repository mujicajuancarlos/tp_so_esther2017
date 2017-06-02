/*
 * protocol-kernel-cpu.h
 *
 *  Created on: 14/4/2017
 *      Author: utnso
 */

#ifndef SRC_DC_COMMONS_PROTOCOL_KERNEL_CPU_H_
#define SRC_DC_COMMONS_PROTOCOL_KERNEL_CPU_H_

#include <stdint.h>
#include <stddef.h>
#include <parser/metadata_program.h>
#include "logger.h"
#include "serialization.h"

//prefijo = CPU ; between 200 - 299
#define COD_SET_STACK_PAGE_SIZE 200
#define COD_EXEC_NEW_PCB 201 //para ejecutar un nuevo pcb
#define COD_CONTINUE_EXECUTION 202 //para continuar la ejecucion
#define COD_CONTEXT_SWITCH_REQUEST 203 //para solicitar a la cpu que me devuelva el pcb actualizado
#define COD_CONTEXT_SWITCH_RESPONSE 204 //para responder el pcb
//para finalizar ejecucion
#define COD_END_INSTRUCCION 205 //fin de instruccion
#define COD_PROGRAM_FINISHED 206 //el programa termino
#define COD_SIGNAL_DISCONNECTED 207 //le llego una señal de desconeccion a la cpu -> dejara de dar servicio
//no bloqueantes
#define COD_GET_SHARED_VAR 208
#define COD_GET_SHARED_VAR_RESPONSE 209
#define COD_SET_SHARED_VAR 210
// MEMORY
#define COD_MALLOC_MEMORY 211
#define COD_FREE_MEMORY 212
//bloqueantes FS
#define COD_OPEN_FD 213
#define COD_DELETE_FD 214
#define COD_CLOSE_FD 215
#define COD_SEED_FD 216
#define COD_WRITE_FD 217
#define COD_READ_FD 218
//bloquantes semaforos
#define COD_SEM_WAIT 219
#define COD_SEM_SIGNAL 220

typedef struct dir_memoria {
	uint32_t pagina;
	uint32_t offset;
	uint32_t size;
} dir_memoria;

typedef struct {
	dir_memoria direccion;
	char nombre;
} t_variable;

typedef struct {
	uint32_t arg_len;
	t_variable* args;
	uint32_t var_len;
	t_variable* vars;
    t_puntero_instruccion retPos;
	dir_memoria retVar;
} t_stack_index;

typedef struct {
	uint32_t pid;					//identificador unico del proceso
	uint32_t programCounter;		//contadore del programa
	uint32_t stackFirstPage;		//numero de pagina de inicio del stack
	uint32_t stackOffset;			//offset actual donde agregar variables en el stack
	t_metadata_program* metadata;	//indice de codigo
	uint32_t stackSize;				//tamaño de items del stack
	t_stack_index* stackIndex;		//context
	uint32_t exit_code; //Agrego Exit Code a PCB (No hace falta serializar, usa solo Kernel)
} PCB;


//EXCLUSIVO PBC
PCB* create_new_PCB(uint32_t pid, uint32_t stackFirstPage, t_metadata_program* metadata);//ok
void destroy_PBC(PCB* pbc);//ok
uint32_t sizeOf_PCB(PCB* pcb);//ok
char* serialize_PCB(PCB* pcb);//ok
PCB* deserialize_PCB(char* stream);//ok
//******************************************

//EXCUSIVO METADATA PROGRAM
uint32_t sizeOf_metadata_program(t_metadata_program* metadata);//ok
char* serialize_metadata_program(t_metadata_program* metadata);//ok
t_metadata_program* deserialize_metadata_program(char* serialized);//ok

//EXCUSIVO STACK
uint32_t sizeof_stackArray(t_stack_index* stackArray, uint32_t stackSize);//ok
uint32_t sizeof_stackIndex(t_stack_index* contexto);//ok
char* serialize_stackArray(t_stack_index* stackArray, uint32_t stackSize);//todo: verificar
char* serialize_stackIndex(t_stack_index* stackIndex);//ok
t_stack_index* deserialize_stackArray(char* buffer, uint32_t stackSize);//todo: verificar
t_stack_index* deserialize_stackIndex(char* stream);//ok
void destroy_stackArray(t_stack_index* stackArray, uint32_t stackSize);//ok
void destroy_stackIndex(t_stack_index stackIndex);//ok

//EXCUSIVO VARIABLES
char* serializeVariablesArray(t_variable* variables, uint32_t len);
t_variable* deserializeVariablessssArray(char* stream, uint32_t len);

//  EXCUSIVO CONTEXTO
void createNewContext(PCB* pcb);//ok
void destroyCurrentContext(PCB* pcb);//ok

#endif /* SRC_DC_COMMONS_PROTOCOL_KERNEL_CPU_H_ */
