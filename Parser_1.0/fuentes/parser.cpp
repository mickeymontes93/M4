//un parser descendente determinístico para pl0
#include <stdlib.h>
#include "parser.h"
#include "tds.h"
#include "auxiliares.h"
#include "lexico.h"
#include "scanner.h"

//funciones internas al parser
void FUNCION_INSTRUCCION(), CONJUNVAR(), DECLARACIÓN(), OPERACION_NUM(), TERMINO(), ASIGNACIÓN(), FUN_SUBSTRING(), FUN_CONCAT();
void FUN_REPLACE(), INS_DO(), INS_CONSOLEWRITE(), INS_CONSOLEREAD(), INS_SWITCH(), CASE(), ABRIR_ARCHIVO(), INS_IF(), INS_WHILE();
void INS_FOR(), INSTRUCCION(), PAREN_CONDICIÓN(), LLAVE_INSTRUCCION(), DATA_NUM(), DATA_CAD(), ARREGLO(), IDENTIFICADOR();
void EXPRESION_NUM(), EXPRESIÓN_CAD(), EXPRESION_BOOL(), CONDICION(), COMPAGENERAL(), COMPANUM(), CAD_VAR(), FUNCION(), VARIABLE();
void TIPO_FILE(), BLOQUE();

void FUNCION_INSTRUCCION() {  }
void CONJUNVAR() {  }
void DECLARACIÓN() {  }
void OPERACION_NUM() {  }
void TERMINO() {  }
void ASIGNACIÓN() {  }
void FUN_SUBSTRING() {  }
void FUN_CONCAT() {  }
void FUN_REPLACE() {  }
void INS_DO() {  }
void INS_CONSOLEWRITE() {  }
void INS_CONSOLEREAD() {  }
void INS_SWITCH() {  }
void CASE() {  }
void ABRIR_ARCHIVO() {  }
void INS_IF() {  }
void INS_WHILE() {  }
void INS_FOR() {  }
void INSTRUCCION() {  }
void PAREN_CONDICIÓN() {  }
void LLAVE_INSTRUCCION() {  }
void DATA_NUM() {  }
void DATA_CAD() {  }
void ARREGLO() {  }
void IDENTIFICADOR() {  }
void EXPRESION_NUM() {  }
void EXPRESIÓN_CAD() {  }
void EXPRESION_BOOL() {  }
void CONDICION() {  }
void COMPAGENERAL() {  }
void COMPANUM() {  }
void CAD_VAR() {  }
void FUNCION() {  }
void VARIABLE() {  }
void TIPO_FILE() {  }
void BLOQUE() {  }
void PROGRAMA() {  }

