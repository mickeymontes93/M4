//un parser descendente deterministico para pl0
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "tds.h"
#include "auxiliares.h"
#include "lexico.h"
#include "codigo_p.h"


int IS_VARIABLE() {

	if ( token == tok_arrEntero ||
	        token == tok_arrFlotante ||
	        token == tok_arrBooleano ||
	        token == tok_arrCadena ||
	        token == tok_arrCaracter ||
	        token == tok_varint ||
	        token == tok_varfloat ||
	        token == tok_varstr ||
	        token == tok_varchar ||
	        token == tok_varbool ||
	        token == tok_file)
	{
		return 1;
	} else {
		return 0;
	}
}

int IS_DATA_NUM() {
	if ( token == tok_numero ||
	        token == tok_flotante ||
	        IS_IDENTIFICADOR())
	{
		return 1;
	} else {
		return 0;
	}
}

int IS_IDENTIFICADOR() {
	if ( token == tok_id )
	{
		return 1;
	} else {
		return 0;
	}
}

int IS_OPERACION_NUM() {
	if ( token == tok_sum ||
	        token == tok_resta)
	{
		return 1;
	} else {
		return 0;
	}
}

int IS_FUN_SUBSTRING() {
	if (token == tok_substring)
		return 1;
	else
		return 0;
}

int IS_FUN_CONCAT() {
	if (token == tok_concat)
		return 1;
	else
		return 0;
}

int IS_FUN_REPLACE() {
	if (token == tok_replace)
		return 1;
	else
		return 0;
}

int IS_DATA_CAD() {	
	if ( token == tok_cadena ||
	        token == tok_caracter ||
	        IS_IDENTIFICADOR())
	{
		return 1;
	} else {
		return 0;
	}
}

int IS_EXPRESION_BOOL() {
	if (token == tok_id) {
		int i = posicion();
		if (i > 0) {
			if (regEncontrado->tipoDato == TIPO_BOOLEAN ) {
				return 1;
			} else {
				return 0;
			}
		} else {
			//Identificador no declarado
			error(49);
		}
	}
	if (token == tok_true || token == tok_false ||
	        token == tok_equal)
		return 1;
	else
		return 0;
}

int IS_EXPRESION_ARR() {
	if (token == tok_split || IS_ARREGLO())
		return 1;
	else
		return 0;
}

int IS_EXPRESION_CAD() {
	if (token == tok_id) {
		int i = posicion();
		if (i > 0) {
			if (regEncontrado->tipoDato == TIPO_CADENA || regEncontrado->tipoDato == TIPO_CARACTER) {
				return 1;
			} else {
				return 0;
			}
		} else {
			//Identificador no declarado
			error(49);
		}
	}
	if (IS_FUN_CONCAT() || IS_FUN_SUBSTRING() ||
	        IS_FUN_REPLACE() || IS_DATA_CAD() || token == tok_fscanf || token == tok_fgets)
		return 1;
	else
		return 0;
}



int IS_EXPRESION_NUM() {
	if (token == tok_id) {
		int i = posicion();
		if (i > 0) {
			if (regEncontrado->tipoDato == TIPO_ENTERO || regEncontrado->tipoDato == TIPO_FLOAT ) {
				return 1;
			} else {
				return 0;
			}
		} else {
			//Identificador no declarado
			error(49);
		}
	}
	if (token == tok_round || token == tok_sin ||
	        token == tok_cos || token == tok_arcsin ||
	        token == tok_arccos || token == tok_tan ||
	        token == tok_arctan || token == tok_sqrt ||
	        token == tok_length || token == tok_pow || token == tok_log ||
	        IS_DATA_NUM() || IS_OPERACION_NUM() || token == tok_parena ||
	        token == tok_id)
		return 1;
	else
		return 0;
}

int IS_COMPANUM() {
	if (token == tok_menor || token == tok_menorigual ||
	        token == tok_mayor || token == tok_mayorigual)
		return 1;
	else
		return 0;
}

int IS_COMPAGENERAL() {
	if (token == tok_igual || token == tok_negacion)
		return 1;
	else
		return 0;
}

int IS_DECLARACION() {
	if (IS_VARIABLE())
		return 1;
	else
		return 0;
}


int GET_TIPO_DATO() {
	if (token == tok_varint || token == tok_funcint) return TIPO_ENTERO;
	if (token == tok_varfloat || token == tok_funcfloat) return TIPO_FLOAT;
	if (token == tok_varstr || token == tok_funcstr) return TIPO_CADENA;
	if (token == tok_varchar || token == tok_funcchar) return TIPO_CARACTER;
	if (token == tok_varbool || token == tok_bool) return TIPO_BOOLEAN;
	if (token == tok_funcvoid) return TIPO_VOID;
}

int IS_FUNCION() {
	if (token == tok_funcint || token == tok_funcfloat
	        || token ==  tok_funcchar || token ==  tok_funcstr
	        || token == tok_funcvoid || token == tok_bool)
		return 1;
	else
		return 0;
}

int IS_FUNCION_INSTRUCCION() {
	if (IS_FUNCION())
		return 1;
	else
		return 0;
}

int IS_ARREGLO() {
	if (token == tok_corcha)
		return 1;
	else
		return 0;
}
//
