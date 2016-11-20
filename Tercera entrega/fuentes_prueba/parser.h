void PROGRAMA();

//funciones internas al parser

void FUNCION_INSTRUCCION(int toksig[]), CONJUNVAR(int toksig[]), DECLARACION(), OPERACION_NUM(int toksig[]), TERMINO(int toksig[]), ASIGNACION(int toksig[]), FUN_SUBSTRING(int toksig[]), FUN_CONCAT(int toksig[]),
	FUN_REPLACE(int toksig[]), INS_DO(int toksig[]), INS_CONSOLEWRITE(int toksig[]), INS_CONSOLEREAD(), INS_SWITCH(int toksig[]), CASE(int toksig[]), ABRIR_ARCHIVO(int toksig[]),  INS_WHILE(int toksig[]),
	INS_FOR(int toksig[]), PAREN_CONDICION(int toksig[]), LLAVE_INSTRUCCION(int toksig[]), DATA_NUM(int toksig[]), DATA_CAD(int toksig[]), ARREGLO(), IDENTIFICADOR(int toksig[]), EXPRESION_ARR(int toksig[]),
	EXPRESION_NUM(int toksig[]), EXPRESION_CAD(int toksig[]), EXPRESION_BOOL(int toksig[]), COMPAGENERAL(), COMPANUM(), CAD_VAR(int toksig[]), FUNCION(), VARIABLE(),
	TIPO_FILE();

//Para tratamiento de errores.
void BLOQUE(int toksig[]),PAREN_CONDICION(int toksig[]);
void INSTRUCCION(int toksig[]),INS_IF(int toksig[]),CONDICION(int toksig[]);

//funciones auxiliares del parser
int IS_VARIABLE(),IS_DATA_NUM(),IS_IDENTIFICADOR(),IS_OPERACION_NUM(),
	IS_FUN_SUBSTRING(),IS_FUN_CONCAT(),IS_FUN_REPLACE(),IS_DATA_CAD(),
	IS_EXPRESION_BOOL(),IS_EXPRESION_ARR(),IS_EXPRESION_CAD(),IS_EXPRESION_NUM(),
	IS_COMPANUM(),IS_COMPAGENERAL(),IS_DECLARACION(),IS_FUNCION(),
	IS_FUNCION_INSTRUCCION(),IS_ARREGLO(),GET_TIPO_DATO();