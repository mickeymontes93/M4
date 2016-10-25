//pl0 versión 3.0-solo scanner (analizador lexicográfico)

//abril de 2011,Agosto 2012
//se usó Microsoft Visual C++ 2010 para escribir este scanner

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "pl0.h"
#include "parametros.h"
#include "auxiliares.h"
#include "scanner.h"
#include "lexico.h"
#include "tds.h"
#include "parser.h"

FILE *fp; //apuntador a archivo conteniendo el programa fuente

//main: inicia el compilador...solo scanner
int main (int argc, char *argv[]) {
//printf("\nENTROOO AL MAIN");
//verificar si hay archivo fuente
	if (argc != 2)
		printf("\nNo se ha proporcionado el nombre del programa fuente (uso: scanner progfuente)");
	else {
		parametrizarScanner();
		fp = fopen(argv[1], "r"); //abrir el fuente solo para lectura
		if (fp == NULL)
			printf("\nNo se encontro el programa fuente indicado");
		else {
			printf("\nCompilador de M4 --- Ciclo 02-2016\n\n");
			//inicializacion de tokens de símbolos especiales (en auxiliares.cpp)
			inicializar_espec() ;

			//inicializacion de otras variables (en scanner.h)
			ch = ' ';
			fin_de_archivo = 0;
			offset = -1; ll = 0;

			//invocar al scanner (en scanner.cpp)
			obtoken();

			it = 0; //inicializamos el índice sobre la tds (it en tds.h)

			//activación del parser (en parser.h)
			PROGRAMA();

			//si llegamos a este punto, no se han detectado errores sintácticos en el programa fuente (estadisticas en auxiliares.cpp)
			estadisticas();

			//cerrar el programa fuente
			fclose(fp);
		}
	}
	return (0);
}




