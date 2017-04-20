/*
 * packagesCPU.c
 *
 *  Created on: 20/4/2017
 *      Author: utnso
 */

void comunicacionConCPU(unsigned long int hiloCPU){

	int init = 1;
	int exit = 1;

	while (init){
		packagesReceptionCPU(hiloCPU, init);
	}
	while (exit){
		packagesSenderCPU(hiloCPU, exit);
	}

}

void packagesReceptionCPU(unsigned long int hiloCPU, int continuador){

	Package *packageRecv;

		if(receivePackage(hiloCPU, packageRecv) != 0){
		switch(packageRecv->msgCode)
		{
		case COD_SOLICITAR_BYTES_PAGE:
			//Solicitar bytes de una página
			break;
		case COD_ESCRITURA_PAGE:
			//Almacenar bytes en una página
			break;
		case COD_INFO_PROGRAMA_EXE:
			//Hace lo que tiene que hacer
			break;
		case COD_ACTUALIZAR_ESTRUCTURA:
			//Hace lo que tiene que hacer
			break;
		case COD_SALUDO:
			//Hace lo que tiene que hacer

		}
		}
}

void packagesSenderCPU(unsigned long int hiloCPU, int expulsador){

}
