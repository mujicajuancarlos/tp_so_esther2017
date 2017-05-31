/*
 * primitiveFunctions.c
 *
 *  Created on: 28/5/2017
 *      Author: utnso
 */

#include "primitiveFunctions.h"

t_puntero ansisop_definirVariable(t_nombre_variable identificador_variable) {
	logTrace("Ejecutando ansisop_definirVariable(%c)", identificador_variable);
	t_variable* newArgOrVar;
	t_puntero pointer = -1;
	validateStackOverflow(sizeof(t_puntero));
	if (getErrorFlag() == FLAG_OK) {
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
				"Se ejecutó ansisop_definirVariable(%c) se creó la variable en: pag:%d, off:%d, size:%d, ptr:%d",
				identificador_variable, newArgOrVar->direccion.pagina,
				newArgOrVar->direccion.offset, newArgOrVar->direccion.size,
				pointer);
	}
	return pointer;
}

t_puntero ansisop_obtenerPosicionVariable(
		t_nombre_variable identificador_variable) {
	return 0;
}

t_valor_variable ansisop_dereferenciar(t_puntero direccion_variable) {
	return 0;
}

void ansisop_asignar(t_puntero direccion_variable, t_valor_variable valor) {

}

t_valor_variable ansisop_obtenerValorCompartida(t_nombre_compartida variable) {
	return 0;
}

t_valor_variable ansisop_asignarValorCompartida(t_nombre_compartida variable,
		t_valor_variable valor) {
	return 0;
}

void ansisop_irAlLabel(t_nombre_etiqueta t_nombre_etiqueta) {

}

void ansisop_llamarSinRetorno(t_nombre_etiqueta etiqueta) {

}

void ansisop_llamarConRetorno(t_nombre_etiqueta etiqueta,
		t_puntero donde_retornar) {

}

void ansisop_finalizar(void) {

}

void ansisop_retornar(t_valor_variable retorno) {

}
