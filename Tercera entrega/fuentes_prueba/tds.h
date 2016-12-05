#include "parametros.h"


//definici�n de la tabla de s�mbolos - organizacion y acceso lineal
//sufrir� modificaciones cu�ndo se presente el compilador completo

//tipos de objetos de pl0
enum objeto {TIPO_VARIABLE, TIPO_ARREGLO, TIPO_FUNCION}; //definidos aqu� en el encabezado

typedef struct {
	int nivel;
	int dir;
} nivel_y_direccion;

typedef struct struct_reg {
	int cab;
	char nombre[50 + 1];
	enum objeto tipoObjeto;
	int tipoDato;
	struct struct_reg* sig;
	struct struct_reg* ant;
	nivel_y_direccion nivdir;
} registro;


//tabla de s�mbolos
extern registro *tabla; //MAXIT en parametros (+1 porque tabla[0] esta reservada)
extern int it;                  //�ndice para recorrer la tabla
extern int tipoDato;
extern registro *regEncontrado;
extern registro *regUltimo;

void poner(enum objeto k, int *idat);
int posicion();
registro* getElemento(int indice);

