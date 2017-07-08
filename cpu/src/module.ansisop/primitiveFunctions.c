/*
 * primitiveFunctions.c
 *
 *  Created on: 28/5/2017
 *      Author: utnso
 */

#include "primitiveFunctions.h"

t_puntero ansisop_definirVariable(t_nombre_variable identificador_variable) {
	t_variable* newArgOrVar;
	t_puntero pointer = NULL_POINTER;
	validateStackOverflow(sizeof(t_puntero));
	if (getErrorFlag() == FLAG_OK) {
		logTrace("Ejecutando ansisop_definirVariable(%c)", identificador_variable);
		if (isdigit(identificador_variable)) {
			newArgOrVar = createArgumentForCurrentPCB(identificador_variable);
			logTrace("Argumento definido: Nom: %c", newArgOrVar->nombre);
			pointer = logicalAddressToPointer(&(newArgOrVar->direccion));
		} else {
			newArgOrVar = createVariableForCurrentPCB(identificador_variable);
			logTrace("Variable definida: Nom: %c", newArgOrVar->nombre);
			pointer = logicalAddressToPointer(&(newArgOrVar->direccion));
		}
		logTrace(
				"Se ejecutó ansisop_definirVariable(%c) se creó la variable/argumento en: pag:%d, off:%d, size:%d, ptr:%d",
				identificador_variable, newArgOrVar->direccion.pagina,
				newArgOrVar->direccion.offset, newArgOrVar->direccion.size,
				pointer);
	}
	return pointer;
}

t_puntero ansisop_obtenerPosicionVariable(
		t_nombre_variable identificador_variable) {
	t_variable* newArgOrVar;
	t_puntero pointer = NULL_POINTER;
	if (getErrorFlag() == FLAG_OK) {
		logTrace("Ejecutando ansisop_obtenerPosicionVariable(%c)",
					identificador_variable);
		if (isdigit(identificador_variable)) {
			newArgOrVar = getArgument(identificador_variable);
		} else {
			newArgOrVar = getVariable(identificador_variable);
		}
		if (newArgOrVar == NULL) {
			setErrorFlag(FLAG_SINTAX_ERROR);
			logError(
					"No se pudo encontrar la variable/argumento (%c) en el stack, verificar que no se trate de un error sintactico ansisop");
		} else {
			pointer = logicalAddressToPointer(&(newArgOrVar->direccion));
			logTrace(
					"Se ejecutó ansisop_obtenerPosicionVariable(%c) se encontro la variable/argumento en: pag:%d, off:%d, size:%d, ptr:%d",
					identificador_variable, newArgOrVar->direccion.pagina,
					newArgOrVar->direccion.offset, newArgOrVar->direccion.size,
					pointer);
		}
	}
	return pointer;
}

t_valor_variable ansisop_dereferenciar(t_puntero pointer) {
	t_valor_variable value = NULL_VALUE;
	if (getErrorFlag() == FLAG_OK) {
		logTrace("Ejecutando ansisop_dereferenciar(%d)", pointer);
		dir_memoria* address = pointerToMemoryLogicalAddress(pointer);
		char* buffer = getDataFromMemory(getCPUStruct(), address->pagina,
				address->offset, address->size);
		if (getErrorFlag() == FLAG_OK) {
			value = deserialize_int(buffer);
			free(buffer);
			logTrace(
					"Se derefencio el puntero %d el valor optenido de la memoria es: %d",
					pointer, value);
		} else {
			logError(
					"Fallo la solicitud a la memoria de pag:%d, off:%d, size:%d, ptr:%d",
					address->pagina, address->offset, address->size, pointer);
		}
		free(address);
	}
	return value;
}

void ansisop_asignar(t_puntero pointer, t_valor_variable value) {
	if (getErrorFlag() == FLAG_OK) {
		logTrace("Ejecutando ansisop_asignar(%d,%d)", pointer, value);
		dir_memoria* address = pointerToMemoryLogicalAddress(pointer);
		char* buffer = serialize_int(value);
		saveDataOnMemory(getCPUStruct(), address->pagina, address->offset,
				address->size, buffer);
		if (getErrorFlag() == FLAG_OK) {
			logTrace(
					"Se envio el valor %d a la direccion pag:%d, off:%d, size:%d",
					value, address->pagina, address->offset, address->size);
		} else {
			logError(
					"Fallo la solicitud a la memoria de pag:%d, off:%d, size:%d, ptr:%d",
					address->pagina, address->offset, address->size, pointer);
		}
		free(buffer);
		free(address);
	}
}

t_valor_variable ansisop_obtenerValorCompartida(t_nombre_compartida name) {
	char* identificadorVariable = normalizeString(name);
	t_valor_variable value = NULL_VALUE;
	if (getErrorFlag() == FLAG_OK) {
		logTrace("Ejecutando ansisop_obtenerValorCompartida(%s)", identificadorVariable);
		value = getSharedVarriableValue(getCPUStruct(), identificadorVariable);
		if (getErrorFlag() == FLAG_OK) {
			logTrace(
					"Socilitud al kernel realizada, variable compartida: %s value:%d",
					identificadorVariable, value);
		} else {
			logError(
					"Fallo la solicitud al kernel para la variable compartida: %s",
					identificadorVariable);
		}
	}
	free(identificadorVariable);
	return value;
}

t_valor_variable ansisop_asignarValorCompartida(t_nombre_compartida name,
		t_valor_variable value) {
	char* identificadorVariable = normalizeString(name);
	t_valor_variable assignedValue = NULL_VALUE;
	if (getErrorFlag() == FLAG_OK) {
		logTrace("Ejecutando ansisop_asignarValorCompartida(%s,%d)", identificadorVariable, value);
		assignedValue = setSharedVarriableValue(getCPUStruct(), identificadorVariable, value);
		if (getErrorFlag() == FLAG_OK) {
			logTrace(
					"Socilitud al kernel realizada, variable compartida: %s value:%d",
					identificadorVariable, value);
		} else {
			logError(
					"Fallo la solicitud al kernel para la variable compartida: %s",
					identificadorVariable);
		}
	}
	free(identificadorVariable);
	return assignedValue;
}

void ansisop_irAlLabel(t_nombre_etiqueta name) {
	char* identificadorEtiqueta = normalizeString(name);
	if (getErrorFlag() == FLAG_OK) {
		logTrace("Ejecutando ansisop_irAlLabel(%s)", identificadorEtiqueta);
		int status = NULL_VALUE;
		t_puntero_instruccion newProgramCounter = getProgramCounterByLabel(identificadorEtiqueta,
				&status);
		if (status != NULL_VALUE) {
			getPCB()->programCounter = newProgramCounter;
			logTrace("Se definio el program counter con el valor: %d",
					newProgramCounter);
		} else {
			logError("No se encontro la etiqueta %s", identificadorEtiqueta);
			setErrorFlag(FLAG_SINTAX_ERROR);
		}
	}
	free(identificadorEtiqueta);
}

void ansisop_llamarSinRetorno(t_nombre_etiqueta name) {
	char* identificadorEtiqueta = normalizeString(name);
	if (getErrorFlag() == FLAG_OK) {
		logTrace("Ejecutando ansisop_llamarSinRetorno(%s,)", identificadorEtiqueta);
		int status = NULL_VALUE;
		t_puntero_instruccion newProgramCounter = getProgramCounterByLabel(
				identificadorEtiqueta, &status);
		if (status != NULL_VALUE) {
			PCB* tmpPcb = getPCB();
			createNewContext(tmpPcb);
			logTrace("Se genero un nuevo contexto en el stack");
			t_stack_index* stackIndex = getCurrentContext();
			stackIndex->retPos = tmpPcb->programCounter;
			logTrace("Se definio el puntero de retorno en: %d",
					tmpPcb->programCounter);
			tmpPcb->programCounter = newProgramCounter;
			logTrace("Se definio el program counter en: %d",
					tmpPcb->programCounter);
			stackIndex->retVar.pagina = 0;
			stackIndex->retVar.offset = 0;
			stackIndex->retVar.size = 0;
		} else {
			logError(
					"No se pudo obtener un program counter para la etiqueta %s",
					identificadorEtiqueta);
			setErrorFlag(FLAG_SINTAX_ERROR);
		}
		logTrace("Fin de ansisop_llamarSinRetorno(%s)", identificadorEtiqueta);
	}
	free(identificadorEtiqueta);
}

void ansisop_llamarConRetorno(t_nombre_etiqueta name, t_puntero pointer) {
	char* identificadorEtiqueta = normalizeString(name);
	if (getErrorFlag() == FLAG_OK) {
		logTrace("Ejecutando ansisop_llamarConRetorno(%s, %d)", identificadorEtiqueta, pointer);
		int status = NULL_VALUE;
		t_puntero_instruccion newProgramCounter = getProgramCounterByLabel(
				identificadorEtiqueta, &status);
		if (status != NULL_VALUE) {
			PCB* tmpPcb = getPCB();
			createNewContext(tmpPcb);
			logTrace("Se genero un nuevo contexto en el stack");
			t_stack_index* stackIndex = getCurrentContext();
			stackIndex->retPos = tmpPcb->programCounter;
			logTrace("Se definio el puntero de retorno en: %d",
					tmpPcb->programCounter);
			tmpPcb->programCounter = newProgramCounter;
			logTrace("Se definio el program counter en: %d",
					tmpPcb->programCounter);
			dir_memoria* address = pointerToMemoryLogicalAddress(pointer);
			stackIndex->retVar.pagina = address->pagina;
			stackIndex->retVar.offset = address->offset;
			stackIndex->retVar.size = address->size;
			logTrace(
					"Se definio la direccion del valor de retorno en: pag:%d, offset:%d, size:%d",
					address->pagina, address->offset, address->size);
			free(address);
		} else {
			logError(
					"No se pudo obtener un program counter para la etiqueta %s",
					identificadorEtiqueta);
			setErrorFlag(FLAG_SINTAX_ERROR);
		}
		logTrace("Fin de ansisop_llamarConRetorno(%s, %d)", identificadorEtiqueta, pointer);
	}

	free(name);
}

void ansisop_finalizar() {
	if (getErrorFlag() == FLAG_OK) {
		logTrace("Ejecutando ansisop_finalizar()");
		setErrorFlag(FLAG_END_PROGRAM);
	}
}

void ansisop_retornar(t_valor_variable value) {
	if (getErrorFlag() == FLAG_OK) {
		logTrace("Ejecutando ansisop_retornar(%d)", value);
		t_stack_index* stackIndex = getCurrentContext();
		dir_memoria* address = &(stackIndex->retVar);
		char* buffer = serialize_int(value);
		saveDataOnMemory(getCPUStruct(), address->pagina, address->offset,
				address->size, buffer);
		free(buffer);
		logTrace(
				"Se guardó el valor: %d en la direccion pag:%d, offset:%d, size:%d",
				value, address->pagina, address->offset, address->size);

		destroyCurrentContext(getPCB());
	}
}
