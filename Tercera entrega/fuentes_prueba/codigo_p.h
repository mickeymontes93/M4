#include "parametros.h"
#include "scanner.h"

//instrucciones(mnemónicos) del código-p
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC}; //definido en este encabezado
enum tipo_dato {TIPO_ENTERO, TIPO_FLOAT, TIPO_CADENA, TIPO_CARACTER, TIPO_BOOLEAN, TIPO_VOID}; //definidos aquí en el encabezado

typedef struct {
  enum fcn f; // mnemónico
  int      ni; //nivel
  enum tipo_dato t;
  valorPorTipo Dato;
} codigo_intermedio;

extern codigo_intermedio codigo[MAXIC]; //array con las instrucciones de codigo-p
extern int ic;                          //índice sobre el array de código-p

void gen (enum fcn f, int nivel, enum tipo_dato tipoDato, valorPorTipo valor),listar_p(),escribe_codigop(char *fuente);