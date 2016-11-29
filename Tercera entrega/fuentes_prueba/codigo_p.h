#include "parametros.h"

//instrucciones(mnem�nicos) del c�digo-p
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC}; //definido en este encabezado

typedef struct {
 enum fcn f; //mnem�nico
 int     ni; //nivel (0..MAXNIV)
 int     di; //direcci�n o desplazamiento (0..32767)
} codigo_intermedio;

extern codigo_intermedio codigo[MAXIC]; //array con las instrucciones de codigo-p
extern int ic;                          //�ndice sobre el array de c�digo-p

void gen (enum fcn x,int y,int z),listar_p(),escribe_codigop(char *fuente);