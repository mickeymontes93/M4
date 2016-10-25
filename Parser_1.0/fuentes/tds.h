#include "parametros.h"

//definición de la tabla de símbolos - organizacion y acceso lineal
//sufrirá modificaciones cuándo se presente el compilador completo

//tipos de objetos de pl0
enum objeto {TIPO_VARIABLE,TIPO_ARREGLO,TIPO_FUNCION}; //definidos aquí en el encabezado

typedef struct struct_reg{
 int cab;
 char *nombre;
 enum objeto tipo;

 struct struct_reg* sig;
} registro;


//tabla de símbolos
extern registro *tabla; //MAXIT en parametros (+1 porque tabla[0] esta reservada)
extern int it;                  //índice para recorrer la tabla 

void poner(enum objeto k);
int posicion();

