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

#define COD_CPU_RUN_PCB 201



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
} PCB;


// PBC
PCB* create_new_PBC(uint32_t pid, uint32_t stackFirstPage, t_metadata_program* metadata);
void destroy_PBC(PCB* pbc);
char* serialize_PCB(PCB* pcb);
uint32_t sizeOf_PCB(PCB* pcb);
PCB* deserialize_PCB(char* stream);
//******************************************

// t_stack_program
t_stack_program* create_new_stack();
void destroy_stack(t_stack_program* stack);
char* serialize_stack(t_stack_program* stack);
uint32_t sizeOf_stack(t_stack_program* stack);
t_stack_program* deserialize_stack(char* stream);
//******************************************

// t_variable
//t_variable* create_variable();
//void destroy_variable(t_variable* variable);
//char* serialize_variable(t_variable* variable);
//uint32_t sizeOf_variable(t_variable* variable);
//t_variable* deserialize_variable(char* stream);
//******************************************

#endif /* SRC_DC_COMMONS_PROTOCOL_KERNEL_CPU_H_ */
