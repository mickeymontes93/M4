#include "parametros.h"

//definici�n de la tabla de s�mbolos - organizacion y acceso lineal
//sufrir� modificaciones cu�ndo se presente el compilador completo

//tipos de objetos de pl0
enum objeto {VARIABLE,ARREGLO,FUNCION}; //definidos aqu� en el encabezado

typedef struct {
 char nombre[MAXID+1];
 enum objeto tipo;
} registro;

//tabla de s�mbolos
extern registro tabla[MAXIT+1]; //MAXIT en parametros (+1 porque tabla[0] esta reservada)
extern int it;                  //�ndice para recorrer la tabla 

void poner(enum objeto k);
int posicion();