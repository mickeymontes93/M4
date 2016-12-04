#include "parametros.h"
#include "scanner.h"

//instrucciones(mnem�nicos) del c�digo-p
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC}; //definido en este encabezado
enum tipo_dato {TIPO_ENTERO, TIPO_FLOAT, TIPO_CADENA, TIPO_CARACTER, TIPO_BOOLEAN, TIPO_VOID}; //definidos aqu� en el encabezado

typedef struct {
  enum fcn f; // mnem�nico
  int      ni; //nivel
  enum tipo_dato t;
  valorPorTipo Dato;
} codigo_intermedio;

extern codigo_intermedio codigo[MAXIC]; //array con las instrucciones de codigo-p
extern int ic;                          //�ndice sobre el array de c�digo-p

void gen (enum fcn f, int nivel, enum tipo_dato tipoDato, valorPorTipo valor),listar_p(),escribe_codigop(char *fuente);