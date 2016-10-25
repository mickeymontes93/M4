#include "parametros.h"

//definición de la tabla de símbolos - organizacion y acceso lineal
//sufrirá modificaciones cuándo se presente el compilador completo

//tipos de objetos de pl0
enum objeto {VARIABLE,ARREGLO,FUNCION}; //definidos aquí en el encabezado

typedef struct struct_reg{
 int cab;
 char nombre[MAXID+1];
 enum objeto tipo;

 struct struct_reg* sig;
} registro;


//tabla de símbolos
extern registro tabla;
extern int it;                  //índice para recorrer la tabla 

void poner(enum objeto k);
int posicion();

