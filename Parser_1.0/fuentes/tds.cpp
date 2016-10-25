//tds: la tabla de s�mbolos
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tds.h"
#include "auxiliares.h"
#include "scanner.h"
#include "parametros.h"

int it = 0, tipoDato; //Indice para realizar busqueda de posicion
registro *tabla = NULL;
registro *regEncontrado;
registro *regUltimo;
int posicion();
registro* agregar_registro(registro* reg, enum objeto k, int it);
void borrar_registro(registro* regViejo);
void imprimirTDS() ;

void poner(enum objeto k) {
	it++;

	if (tabla == NULL) {
		tabla = (registro*) malloc(sizeof(registro));
		tabla->cab = it;
		tabla->tipo = k;
		strcpy(tabla->nombre, lex);
		tabla->tipoDato = tipoDato;
		tabla->sig = NULL;
		tabla->ant = NULL;
		regUltimo = tabla;
	} else {
		registro* aux = tabla;
		while (aux->sig != NULL) {
			aux = aux->sig;
		}
		registro* newReg = (registro*) malloc(sizeof(registro));
		newReg->cab = it;
		newReg->tipo = k;
		strcpy(newReg->nombre, lex);
		newReg->tipoDato = tipoDato;
		newReg->sig = NULL;
		newReg->ant = aux;
		aux->sig = newReg;
		regUltimo = newReg;
	}
	imprimirTDS();
}

void imprimirTDS() {
	registro* aux = tabla;
	//printf("\n\n**************** TDS\n");
	while (aux != NULL) {
		//printf("%s, ", aux->nombre);
		aux = aux->sig;
	}
	//printf("\n**************** FIN TDS \n\n");
}


void borrar_registro(registro* regViejo) {
	if (NULL != regViejo->sig) {
		borrar_registro(regViejo->sig);
	}
	free(regViejo);
}

//poner un objeto: CONSTANTE, VARIABLE o PROCEDIMIENTO en la tds

//posicion: encontrar en la tds al identificador para ver si ya fue declarado y si su uso es sem�nticamente legal

int posicion() {
	if (tabla == NULL) return 0;

	registro* aux = regUltimo;
	while (aux != NULL) {
		if (strcmp(aux->nombre , lex) == 0) {
			regEncontrado = aux;
			return aux->cab;
		}
		aux = aux->ant;
	}
	return 0;
}





