//tds: la tabla de símbolos
#include <string.h>
#include <stdlib.h>
#include "tds.h"
#include "auxiliares.h"
#include "scanner.h"
#include "parametros.h"

int it = 0; //Indice para realizar busqueda de posicion       

registro* agregar_registro(registro*, enum objeto, int);

void poner(enum objeto k)
{
it++;
tabla = agregar_registro(tabla,k,it);
}

//Funciones para manejar la TDS
//Se crea la lista de registros, esto inicializa la posición "0"
registro* crear_registro(enum objeto k, int it) {
        registro* newReg = malloc(sizeof(registro));
        if (NULL != newReg){
		newReg->cab = it;
                newReg->tipo = k;
		newReg->nombre = lex;
                newReg->sig = NULL;
        }
        return newReg;
}

void borrar_registro(registro* regViejo) {
        if (NULL != regViejo->sig) {
                borrar_registro(regViejo->sig);
        }
        free(regViejo);
}
	
//Con agregar registro se van agregando los elementos de la TDS
registro* agregar_registro(registro* reg, enum objeto k, int it) {
        registro* newReg = crear_registro(k, it);
        if (NULL != newReg) {
                newReg->sig = reg;
        }
        return newReg;
}

//poner un objeto: CONSTANTE, VARIABLE o PROCEDIMIENTO en la tds

//posicion: encontrar en la tds al identificador para ver si ya fue declarado y si su uso es semánticamente legal

int posicion(){
	int i=0;
	registro* Auxiliar;
	Auxiliar = tabla;
	while(Auxiliar!= NULL)
	{
		if (Auxiliar->nombre == lex)
		{
			break;
		}
		Auxiliar = Auxiliar->sig;
		i++;
	}
	if (Auxiliar == NULL){
		i=0;
	}
	return(i);
}





