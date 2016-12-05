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
#include "lexico.h"
#include "tds.h"
#include "conjuntos.h"
#include "parser.h"
#include "codigo_p.h"


FILE *fp; //apuntador a archivo conteniendo el programa fuente
int no_de_errores; //contador de errores detectados (ya hay un contador de errores!---en pl0.h)
int niv;           //nivel de anidamiento de los bloques

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

//while (1) {
			//             obtoken();        //en scanner.cpp
			//          imprime_token();  //en auxiliares.cpp
			//  }
			//inicialización de conjuntos de estabilización (en conjuntos.cpp)
			inicializa_conjuntos();

			//inicializa los conjuntos siguientes. (en conjuntos.cpp)
			conjuntos_siguientes();

			//invocar al scanner (en scanner.cpp)
			obtoken();

			it = 0; //inicializamos el índice sobre la tds (it en tds.h)
			niv = 0; //inicializamos el nivel de anidamiento (niv en pl0.h)
			ic = 0; //inicializamos el índice sobre el codigo-p (ic en codigo_p.h)

			printf("*****************Programa\n");
			//activación del parser (en parser.h)
			PROGRAMA();//(set_arranque);

			printf("*****************Estadisticas\n");
			//si llegamos a este punto, no se han detectado errores sintácticos en el programa fuente (estadisticas en auxiliares.cpp)
			estadisticas();

			//cerrar el programa fuente
			fclose(fp);

			printf("*****************Validando numero de errores\n");
			//listar y escribir en disco el código-p resultado de la compilación (en codigo_p.cpp)
			if (no_de_errores == 0) {
				printf("*****************Listar P\n");
				listar_p();
				escribe_codigop(argv[1]);
			}
		}
	}
	return (0);
}




