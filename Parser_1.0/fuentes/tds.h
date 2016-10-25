#include "parametros.h"


//definición de la tabla de símbolos - organizacion y acceso lineal
//sufrirá modificaciones cuándo se presente el compilador completo

//tipos de objetos de pl0
enum objeto {TIPO_VARIABLE, TIPO_ARREGLO, TIPO_FUNCION}; //definidos aquí en el encabezado

typedef struct struct_reg {
	int cab;
	char nombre[50+1];
	enum objeto tipo;
	int tipoDato; //1: entero, 2: float, 3: cadena, 4: caracter, 5:booleano, 6:void
	struct struct_reg* sig;
	struct struct_reg* ant;
} registro;


//tabla de símbolos
extern registro *tabla; //MAXIT en parametros (+1 porque tabla[0] esta reservada)
extern int it;                  //índice para recorrer la tabla
extern int tipoDato; 
extern registro *regEncontrado; 
extern registro *regUltimo;

void poner(enum objeto k);
int posicion();

