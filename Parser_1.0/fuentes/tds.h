#include "parametros.h"

//definici�n de la tabla de s�mbolos - organizacion y acceso lineal
//sufrir� modificaciones cu�ndo se presente el compilador completo

//tipos de objetos de pl0
enum objeto {VARIABLE,ARREGLO,FUNCION}; //definidos aqu� en el encabezado

typedef struct struct_reg{
 int cab;
 char nombre[MAXID+1];
 enum objeto tipo;

 struct struct_reg* sig;
} registro;


//tabla de s�mbolos
extern registro tabla;
extern int it;                  //�ndice para recorrer la tabla 

void poner(enum objeto k);
int posicion();

