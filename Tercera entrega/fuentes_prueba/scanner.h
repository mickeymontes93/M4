#include "parametros.h"

extern char *linea; //[MAXLINEA]; //buffer de líneas
extern int ll;               //contador de caracteres
extern int offset;           //corrimiento en la lectura de los caracteres del programa fuente (se usa en scanner.cpp y auxiliares.cpp)
extern int fin_de_archivo;   //bandera de fin de archivo (se usa en pl0.cpp y scanner.cpp)
extern int ch;               //último caracter leído (se usa en pl0.cpp y scanner.cpp)
extern char *lex;//[MAXID+1];    //último lexeme leído ( +1 para colocar "\0")

//union valor de una lexeme correspondiene a un tipo de dato
typedef union {
	int entero;
	float flotante;
	int booleano;
	char caracter;
	char cadena[MAXSTRING];
} valorPorTipo;

extern valorPorTipo valor;

extern int ln;

void obtoken();
int busqueda_binaria_palabras_reservadas(int medio, char *palabra);
//int busqueda_binaria_palabras_reservadas_2(char *palabra);
