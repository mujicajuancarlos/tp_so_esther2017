/*
 * primitiveFunctions.c
 *
 *  Created on: 28/5/2017
 *      Author: utnso
 */

#include "primitiveFunctions.h"

t_puntero ansisop_definirVariable(t_nombre_variable identificador_variable) {
	/*t_puntero puntero = -1;
	validateStackOverflow(ver que tamaño necesito);
	if(getErrorFlag() == FLAG_OK){

		logTrace("Definir la variable %c", identificador_variable);

		t_stack_program* contexto = getContextoActual();

		dir_memoria dir;
		if (isdigit(variable_nom)) {
			crearArgumento(variable_nom);
			logTrace("Argumento definido: Nom: %c",
					contexto->args[contexto->arg_len - 1].nombre);
			puntero = direccion_logica_a_puntero(
					&(contexto->args[contexto->arg_len - 1].direccion));
			dir = contexto->args[contexto->arg_len - 1].direccion;
		} else {
			crearVariable(variable_nom);
			logTrace("Variable definida: Nom: %c",
					contexto->vars[contexto->var_len - 1].nombre);
			puntero = direccion_logica_a_puntero(
					&(contexto->vars[contexto->var_len - 1].direccion));
			dir = contexto->vars[contexto->var_len - 1].direccion;
		}
		logTrace(
				"Direccion logica a puntero: Pag:%d,Off:%d,Size:%d, puntero:%d",
				dir.pagina, dir.offset, dir.size, puntero);

	}
	t_puntero puntero = -1;
	return puntero;*/
	return 0;
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
