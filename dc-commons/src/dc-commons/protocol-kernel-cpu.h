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
#include "serialization.h"

//prefijo = CPU ; between 200 - 299

#define COD_EXEC_NEW_PCB 201 //para ejecutar un nuevo pcb
#define COD_CONTINUE_EXECUTION 202 //para continuar la ejecucion
#define COD_CONTEXT_SWITCH_REQUEST 203 //para solicitar a la cpu que me devuelva el pcb actualizado
#define COD_CONTEXT_SWITCH_RESPONSE 204 //para responder el pcb
//para finalizar ejecucion
#define COD_PROGRAM_FINISHED 205 //el programa termino
#define COD_SIGNAL_DISCONNECTED 206 //le llego una señal de desconeccion a la cpu -> dejara de dar servicio
//no bloqueantes
#define COD_GET_SHARED_VAR 207
#define COD_SET_SHARED_VAR 208
#define COD_SEM_WAIT 209
#define COD_SEM_SIGNAL 210
//bloqueantes FS
#define COD_OPEN_FD 211
#define COD_DELETE_FD 212
#define COD_CLOSE_FD 213
#define COD_SEED_FD 214
#define COD_WRITE_FD 215
#define COD_READ_FD 216
//bloqueantes MEMORY
#define COD_MALLOC_MEMORY 217
#define COD_FREE_MEMORY 218


typedef struct dir_memoria {
	uint32_t pagina;
	uint32_t offset;
	uint32_t size;
} dir_memoria;

typedef struct {
	dir_memoria direccion;
	char* nombre;
} t_variable;

typedef struct {
	t_variable* argumentos;
	uint32_t arg_len;
	t_variable* variables;
	uint32_t var_len;
    t_puntero_instruccion retPos;
	dir_memoria retVar;
} t_stack_program;

typedef struct {
	uint32_t pid;					//identificador unico del proceso
	uint32_t programCounter;		//contadore del programa
	uint32_t stackFirstPage;		//numero de pagina de inicio del stack en la UMC
	uint32_t stackOffset;			//offset actual donde agregar variables en el stack
	uint32_t executedQuantums;		//cantidad de quantums ya ejecutados
	t_metadata_program* metadata;	//indice de codigo
	t_stack_program* stack;	//context
	uint32_t stackSize;			//tamaño stackIndex
	uint32_t exit_code; //Agrego Exit Code a PCB (No hace falta serializar, usa solo Kernel)
} PCB;


// PBC
PCB* create_new_PCB(uint32_t pid, uint32_t stackFirstPage, t_metadata_program* metadata);
void destroy_PBC(PCB* pbc);
char* serialize_PCB(PCB* pcb);
uint32_t sizeOf_PCB(PCB* pcb);
PCB* deserialize_PCB(char* stream);
//******************************************

// t_stack_program
t_stack_program* create_new_stack();
void destroy_stackIndex(t_stack_program* contexto, uint32_t context_len);
char* serialize_stack(t_stack_program** contextos, uint32_t contextos_length);
//uint32_t sizeOf_stack(t_stack_program* stack);
t_stack_program* deserialize_stack(char* stream,uint32_t contextos_length);
uint32_t getLong_stack(t_stack_program* contextos, uint32_t contextos_length);
//******************************************

// t_variable
//t_variable* create_variable();
//void destroy_variable(t_variable* variable);
//char* serialize_variable(t_variable* variable);
//uint32_t sizeOf_variable(t_variable* variable);
//t_variable* deserialize_variable(char* stream);
char* serializar_array_variables(t_variable** variables, uint32_t len);
t_variable* deserializar_array_variables(char* stream, uint32_t len);
//******************************************
void crearNuevoContexto(PCB* pcb);
void destruirContextoActual(PCB* pcb, int size_pagina);
char* serializar_contexto(t_stack_program* contexto);
uint32_t getLong_contexto(t_stack_program* contexto);
t_stack_program* deserializar_contexto(char* stream);

#endif /* SRC_DC_COMMONS_PROTOCOL_KERNEL_CPU_H_ */
