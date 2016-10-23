//un parser descendente deterministico para pl0
#include <stdlib.h>
#include "parser.h"
#include "tds.h"
#include "auxiliares.h"
#include "lexico.h"
#include "scanner.h"

//funciones auxiliares del parser
int CHECK_VARIABLE();

//funciones internas al parser

void FUNCION_INSTRUCCION(), CONJUNVAR(), DECLARACION(), OPERACION_NUM(), TERMINO(), ASIGNACION(), FUN_SUBSTRING(), FUN_CONCAT();
void FUN_REPLACE(), INS_DO(), INS_CONSOLEWRITE(), INS_CONSOLEREAD(), INS_SWITCH(), CASE(), ABRIR_ARCHIVO(), INS_IF(), INS_WHILE();
void INS_FOR(), INSTRUCCION(), PAREN_CONDICION(), LLAVE_INSTRUCCION(), DATA_NUM(), DATA_CAD(), ARREGLO(), IDENTIFICADOR();
int IDENTIFICADOR();
void EXPRESION_NUM(), EXPRESION_CAD(), EXPRESION_BOOL(), CONDICION(), COMPAGENERAL(), COMPANUM(), CAD_VAR(), FUNCION(), VARIABLE();
void TIPO_FILE(), BLOQUE();



void FUNCION_INSTRUCCION() {
	FUNCION();
	if (token == tok_id)
	{
		obtok();
		CONJUNVAR();
		if (token == tok_llavea)
		{
			obtok();
			DECLARACION();
			INSTRUCCION();
			if (token == tok_return)
			{
				// TODO: Evaluar ExpresionCad, ExpresionNum, ExpresionBoll
				if (token == tok_finlinea)
				{
					obtoken();
				}
				else
				{
					// TODO: modificar error
					error(1);
				}
			}
			if (token == tok_llavec)
			{
				obtoken();
			}
			else
			{
				// TODO: modificar error
				error(1);
			}
		}
		else
		{
			// TODO: modificar error
			error(1);
		}
	}
	else
	{
		// TODO: modificar error
		error(1);
	}
}

void CONJUNVAR() {
	if (token == tok_parena)
	{
		obtoken();
		while (token != tok_parenc)
		{
			VARIABLE();
			if (token == tok_id)
			{
				obtoken();
				if (token != tok_coma)
				{
					break;
				}
			}
			else
			{
				// TODO: modificar error
				error(1);
			}
		}

		if (token == tok_parenc)
		{
			obtoken();
		}
		else
		{
			// TODO: modificar error
			error(1);
		}
	}
	else
	{
		// TODO: modificar error
		error(1);
	}
}

void DECLARACION() {
	int isVariable = CHECK_VARIABLE();
	while (isVariable == 1)
	{
		VARIABLE();
		if (token == tok_id)
		{
			obtoken();
			if (token == tok_finlinea)
			{
				obtoken();
				isVariable = CHECK_VARIABLE();
			}
			else
			{
				// TODO: modificar error
				error(1);
			}
		}
		else
		{
			// TODO: modificar error
			error(1);
		}
	}
	//TODO: Revisar si en el vacio hay que obtener token
}

void OPERACION_NUM() {
	while (token == tok_sum || token == tok_resta)
	{
		obtoken();
		TERMINO();
	}
}

void TERMINO() {
	do
	{
		EXPRESION_NUM();
	} while (token == tok_multi || toke == tok_divi)

	}
void ASIGNACION()
{
	if (token == tok_id)
	{
		obtoken();
		if (token == tok_asignar)
		{
			obtoken();
			// TODO: evaluar expresionBool, expresionNum, expresionNum, expresionCad, expresionArr
		}
		else
		{
			// TODO: modificar error
			error(1);
		}
	}
	else
	{
		// TODO: modificar error
		error(1);
	}
}

void FUN_SUBSTRING() {
	if (token == tok_substring)
	{
		obtoken();
		if (token == tok_parena)
		{
			obtoken();
			EXPRESION_NUM();
			if (token == tok_coma)
			{
				obtoken();
				EXPRESION_NUM();
				if (token == tok_coma)
				{
					obtoken();
					EXPRESION_NUM();
					if (token == tok_parenc)
					{
						obtoken();
					}
					else
					{
						// TODO: modificar error
						error(1);
					}
				}
				else
				{
					// TODO: modificar error
					error(1);
				}
			}
			else
			{
				// TODO: modificar error
				error(1);
			}
		}
		else
		{
			// TODO: modificar error
			error(1);
		}
	}
	else
	{
		// TODO: modificar error
		error(1);
	}
}

void FUN_CONCAT() {
	if (token == tok_concat)
	{
		obtoken();
		if (token == tok_parena)
		{
			obtoken();
			EXPRESION_CAD();
			if (token == tok_coma)
			{
				obtoken();
				EXPRESION_CAD();
				if (token == tok_parenc)
				{
					obtoken();
				}
				else
				{
					// TODO: modificar error
					error(1);
				}
			}
			else
			{
				// TODO: modificar error
				error(1);
			}
		}
		else
		{
			// TODO: modificar error
			error(1);
		}
	}
	else
	{
		// TODO: modificar error
		error(1);
	}
}

void FUN_REPLACE() {
	if (token == tok_replace)
	{
		obtoken();
		if (token == tok_parena)
		{
			obtoken();
			EXPRESION_CAD();
			if (token == tok_coma)
			{
				obtoken();
				EXPRESION_CAD();
				if (token == tok_coma)
				{
					obtoken();
					EXPRESION_CAD();
					if (token == tok_parenc)
					{
						obtoken();
					}
					else
					{
						// TODO: modificar error
						error(1);
					}
				}
				else
				{
					// TODO: modificar error
					error(1);
				}
			}
			else
			{
				// TODO: modificar error
				error(1);
			}
		}
		else
		{
			// TODO: modificar error
			error(1);
		}
	}
	else
	{
		// TODO: modificar error
		error(1);
	}
}

void INS_DO() {
	if (token == tok_do)
	{
		obtoken();
		LLAVE_INSTRUCCION();
		if (token == tok_while)
		{
			obtoken();
			PAREN_INSTRUCCION();
		}
		else
		{
			// TODO: modificar error
			error(1);
		}
	}
	else
	{
		// TODO: modificar error
		error(1);
	}
}

void INS_CONSOLEWRITE() {
	if (token == tok_write)
	{
		obtoken();
		if (toke == tok_parena)
		{
			obtoken();
			EXPRESION_CAD();
			if (toke == tok_coma)
			{
				obtoken();
				do
				{
					if (token == tok_id)
					{
						obtoken();
					}
					else
					{
						// TODO: modificar error
						error(1);
					}
				} while (token == tok_coma);

				if (token == tok_parenc)
				{
					obtoken();
				}
				else
				{
					// TODO: modificar error
					error(1);
				}
			}
			else
			{
				// TODO: modificar error
				error(1);
			}
		}
		else
		{
			// TODO: modificar error
			error(1);
		}
	}
	else
	{
		// TODO: modificar error
		error(1);
	}
}

void INS_CONSOLEREAD() {
	if (token == tok_read)
	{
		obtoken();
		if (token == tok_parena)
		{
			obtoken();
			CAD_VAR();
			if (token == tok_coma)
			{
				obtoken();
				if (token == tok_amp)
				{
					obtoken();
					if (token == tok_id)
					{
						obtoken();
						if (token == tok_parenc)
						{
							obtoken();
						}
						else
						{
							// TODO: modificar error
							error(1);
						}
					}
					else
					{
						// TODO: modificar error
						error(1);
					}
				}
				else
				{
					// TODO: modificar error
					error(1);
				}
			}
			else
			{
				// TODO: modificar error
				error(1);
			}
		}
		else
		{
			// TODO: modificar error
			error(1);
		}
	}
	else
	{
		// TODO: modificar error
		error(1);
	}
}
void INS_SWITCH() {
	if (token == tok_switch)
	{
		obtoken();
		if (token == tok_id)
		{
			obtoken();
			if (token == tok_llavea)
			{
				obtoken();
				CASE();
				if (token == tok_llavec)
				{
					obtoken();
				}
				else
				{
					// TODO: modificar error
					error(1);
				}
			}
			else
			{
				// TODO: modificar error
				error(1);
			}
		}
		else
		{
			// TODO: modificar error
			error(1);
		}
	}
	else
	{
		// TODO: modificar error
		error(1);
	}
}

void CASE() {
	while (token == tok_case)
	{
		obtoken();
		PAREN_CONDICION();
		if (token == tok_llavea)
		{
			obtoken();
			INSTRUCCION();
			if (token == tok_break)
			{
				obtoken();
				if (token == tok_finlinea)
				{
					obtoken();
					if (token == tok_llavec)
					{
						obtoken();
					}
					else
					{
						// TODO: modificar error
						error(1);
					}
				}
				else
				{
					// TODO: modificar error
					error(1);
				}
			}
			else
			{
				// TODO: modificar error
				error(1);
			}
		}
		else
		{
			// TODO: modificar error
			error(1);
		}
	}
	if (token == tok_default)
	{
		obtoken();
		if (token == tok_llavea)
		{
			obtoken();
			INSTRUCCION();
			if (token == tok_break)
			{
				obtoken();
				if (token == tok_finlinea)
				{
					obtoken();
					if (token == tok_llavec)
					{
						obtoken();
					}
					else
					{
						// TODO: modificar error
						error(1);
					}
				}
				else
				{
					// TODO: modificar error
					error(1);
				}
			}
			else
			{
				// TODO: modificar error
				error(1);
			}
		}
		else
		{
			// TODO: modificar error
			error(1);
		}
	}
	else
	{
		// TODO: modificar error
		error(1);
	}
}


//******** PARTE FER ********//


void ABRIR_ARCHIVO() {
	if (token == tok_fileopen)
	{
		obtoken();
		if (token == tok_parena)
		{
			obtoken();
			DATA_CAD();
			if (token == tok_coma)
			{
				obtoken();
				TIPO_FILE();
				if (token == tok_parenc)
				{
					//Se completo exitosamente esta parte.
					obtoken();
				}
				else
				{
					//err: Se esperaba el cierre de un parentesis.
					error(1);
				}
			}
			else
			{
				// err: se esperaba una coma.
				error(1);
			}
		}
		else
		{
			// err: se esperaba un parentesis de apertura.
			error(1);
		}
	}
	else
	{
		// err: se esperaba la instruccion para abrir un archivo var.fopen
		error(1);
	}

}

void DATA_CAD() {
	if (IDENTIFICADOR()) {
		//Se cumplio este camino.
		obtoken();
	}
	else if (token == tok_cadena || token == tok_caracter)
	{
		//Se completo exitosamente esta parte.
		obtoken();
	}
	else
	{
		// err: se esperaba una cadena, un caracter o un identificador.
		error(1);
	}

}

//Tuve que cambiarla a int por el caso de DATA_CAD(), donde necesitaba que identificador me regresara algo para determinar si se fue por ese camino o no, en este caso un entero.
int IDENTIFICADOR() {
	if (token == tok_id)
	{
		obtoken();
		if (token == tok_llavea)
		{
			obtoken();
			EXPRESION_NUM();
			if (token == tok_llavec)
			{
				//Se completo exitosamente esta parte.
				obtoken();
				return 1;
			}
			else
			{
				//err: se esperaba una llave de cierre.
				error(1);
			}
		}
		else
		{
			//err: Se esperaba una llave de apertura.
		}
	}
	else
	{
		//err: se esperaba un identificador.
		return 0;
	}

}

void DATA_NUM() {
	if (IDENTIFICADOR()) {
		//Se cumplio este camino.
		obtoken();
	}
	else if (token == tok_numero || token == tok_flotante)
	{
		//Se completo exitosamente esta parte.
		obtoken();
	}
	else
	{
		// err: se esperaba una numero entero, numero flotante o un identificador.
		error(1);
	}
}
void INS_IF() {
	if (token == tok_if)
	{
		obtoken();
		PAREN_CONDICION();
		LLAVE_INSTRUCCION();
	}
	else
	{
		//err: Se esperaba un if.
		error(1);
	}
}
void INS_WHILE() {
	if (token == tok_while)
	{
		obtoken();
		PAREN_CONDICION();
		LLAVE_INSTRUCCION();
	}
	else
	{
		//err: Se esperaba un while.
		error(1);
	}
}
void INS_FOR() {
	if (token == tok_for)
	{
		obtoken();
		if (token == tok_parena)
		{
			obtoken();
			ASIGNACION();
			if (token == tok_finlinea)
			{
				obtoken();
				CONDICION();
				if (token == tok_finlinea)
				{
					obtoken();
					OPERACION_NUM();
					if (token == parenc)
					{
						obtoken();
						LLAVE_INSTRUCCION();
						//Se completo
					}
					else
					{
						//err: Se esperaba un parentesis de cierre
						error(1);
					}
				}
				else
				{
					//err: Se esperaba un ;
					error(1);
				}
			}
			else
			{
				//err: Se esperaba un ;
				error(1);
			}
		}
		else
		{
			//err: Se esperaba un parentesis de apertura
			error(1);
		}
	}
	else
	{
		//err: Se esperaba la palabra reservada "For".
		error(1);
	}
}


void INSTRUCCION() {
	//TODO: Evaluar instrucciones ASIGNACION(), INS_IF(), INS_WHILE(), INS_FOR(), INS_SWITCH(), INS_DO(), INS_CONSOLEWRITE(), INS_CONSOLEREAD(), ABRIR_ARCHIVO(), tok_id


	if (token == tok_id)
	{
		ASIGNACION();
		if (token == tok_finlinea)
		{
			//Se completo
			obtoken();
		}
		else
		{
			//err: Se esperaba un ;
			error(1);
		}
	}
	else if (token == tok_if)
	{
		INS_IF();
		if (token == tok_finlinea)
		{
			//Se completo
			obtoken();
		}
		else
		{
			//err: Se esperaba un ;
			error(1);
		}
	}
	else if (token == tok_while)
	{
		INS_WHILE();
		if (token == tok_finlinea)
		{
			//Se completo
			obtoken();
		}
		else
		{
			//err: Se esperaba un ;
			error(1);
		}
	}
	else if (token == tok_for)
	{
		INS_FOR();
		if (token == tok_finlinea)
		{
			//Se completo
			obtoken();
		}
		else
		{
			//err: Se esperaba un ;
			error(1);
		}
	}
	else if (token == tok_write)
	{
		INS_CONSOLEWRITE();
		if (token == tok_finlinea)
		{
			//Se completo
			obtoken();
		}
		else
		{
			//err: Se esperaba un ;
			error(1);
		}
	}
	else if (token == tok_read)
	{
		INS_CONSOLEREAD();
		if (token == tok_finlinea)
		{
			//Se completo
			obtoken();
		}
		else
		{
			//err: Se esperaba un ;
			error(1);
		}
	}
	else if (token == tok_switch)
	{
		INS_SWITCH();
		if (token == tok_finlinea)
		{
			//Se completo
			obtoken();
		}
		else
		{
			//err: Se esperaba un ;
			error(1);
		}
	}
	else if (token == tok_do)
	{
		INS_DO();
		if (token == tok_finlinea)
		{
			//Se completo
			obtoken();
		}
		else
		{
			//err: Se esperaba un ;
			error(1);
		}
	}
	else if (token == tok_fileopen)
	{
		ABRIR_ARCHIVO();
		if (token == tok_finlinea)
		{
			//Se completo
			obtoken();
		}
		else
		{
			//err: Se esperaba un ;
			error(1);
		}
	}
	else
	{
		//err: Se esperaba un Identificador, if, while, for, write, read, switch, do o fileopen.
		error(1);
	}


}
void PAREN_CONDICION() {
	if (token == tok_parena)
	{
		obtoken();
		CONDICION();
		if (token == tok_parenc)
		{
			//EXITOOOO
			obtoken();
		}
		else
		{
			//err: Se esperaba un parentesis de cierre.
			exit(1);
		}
	}
	else
	{
		//err: Se esperaba un parentesis de apertura.
		exit(1);
	}
}
void LLAVE_INSTRUCCION() {
	if (token == tok_llavea)
	{
		obtoken();
		INSTRUCCION();
		if (token == tok_llavec)
		{
			//EXITOOOO
			obtoken();
		}
		else
		{
			//err: Se esperaba un parentesis de cierre.
			exit(1);
		}
	}
	else
	{
		//err: Se esperaba un parentesis de apertura.
		exit(1);
	}
}
void ARREGLO() {
	if (token == tok_llavea)
	{
		obtoken();
		if (token == tok_flotante || token == tok_numero || token == tok_cadena || token == tok_boolean || token == tok_caracter) {
			if (token == tok_flotante)
			{
				obtoken();
				do
				{
					if (token == tok_coma)
					{
						obtoken();
						if (token == tok_flotante)
						{
							obtoken();
						}
						else
						{
							//Err: Se esperaba un flotante.
							exit(1);
						}
					}
					else
					{
						//err: Se esperaba una coma.
						exit(1);
					}
				} while (token != tok_finlinea);
			}

			if (token == tok_numero)
			{
				obtoken();
				do
				{
					if (token == tok_coma)
					{
						obtoken();
						if (token == tok_numero)
						{
							obtoken();
						}
						else
						{
							//Err: Se esperaba un entero.
							exit(1);
						}
					}
					else
					{
						//err: Se esperaba una coma.
						exit(1);
					}
				} while (token != tok_finlinea);
			}

			if (token == tok_cadena)
			{
				obtoken();
				do
				{
					if (token == tok_coma)
					{
						obtoken();
						if (token == tok_cadena)
						{
							obtoken();
						}
						else
						{
							//Err: Se esperaba una cadena.
							exit(1);
						}
					}
					else
					{
						//err: Se esperaba una coma.
						exit(1);
					}
				} while (token != tok_finlinea);
			}

			if (token == tok_boolean)
			{
				obtoken();
				do
				{
					if (token == tok_coma)
					{
						obtoken();
						if (token == tok_boolean)
						{
							obtoken();
						}
						else
						{
							//Err: Se esperaba un booleano.
							exit(1);
						}
					}
					else
					{
						//err: Se esperaba una coma.
						exit(1);
					}
				} while (token != tok_finlinea);
			}

			if (token == tok_caracter)
			{
				obtoken();
				do
				{
					if (token == tok_coma)
					{
						obtoken();
						if (token == tok_caracter)
						{
							obtoken();
						}
						else
						{
							//Err: Se esperaba un caracter.
							exit(1);
						}
					}
					else
					{
						//err: Se esperaba una coma.
						exit(1);
					}
				} while (token != tok_finlinea);
			}
		}
		else
		{
			//err: Se esperaba un flotante, entero, cadena, booleano o caracter.
			exit(1);
		}
	}
	else
	{
		//err: Se esperaba una llave de apertura
		exit(1);
	}
}

void EXPRESION_NUM() {
	if (token == tok_round ||
	        token == tok_sin ||
	        token == tok_cos ||
	        token == tok_arcsin ||
	        token == tok_arccos ||
	        token == tok_tan ||
	        token == tok_arctan ||
	        token == tok_sqrt) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			EXPRESION_NUM();

			if (token == tok_parenc) {
				obtoken();
			} else {
				//err: Se esperaba parentesis de cierre
				exit(2);
			}
		} else {
			//err: Se esperaba parentesis de apertura
			exit(1);
		}
	} else {
		if ( token == tok_length ) {
			obtoken();
			if (token == tok_parena) {
				obtoken();
				EXPRESION_CAD();
				if (token == tok_parenc) {
					obtoken();
				} else {
					//err: Se esperaba parentesis de cierre
					exit(2);
				}
			} else {
				//err: Se esperaba parentesis de apertura
				exit(1);
			}
		} else {
			if (token == tok_pow || token == tok_log ) {
				obtoken();
				if (token == tok_parena) {
					obtoken();
					EXPRESION_NUM();
					if (token == tok_coma) {
						obtoken();
						EXPRESION_NUM();
						if (token == tok_parenc) {
							obtoken();
						} else {
							//err: Se esperaba parentesis de cierre
							exit(2);
						}

					} else {
						//err: Se esperaba una coma
						exit(3);
					}
				} else {
					//err: Se esperaba parentesis de apertura
					exit(1);
				}
			} else {
				if (IS_DATA_NUM()) {
					DATA_NUM();
				} else if (IS_OPERACION_NUM()) {
					OPERACION_NUM();
				} else {
					if (token == tok_parena) {
						obtoken();
						EXPRESIÓN_CAD();
						if (token == tok_parenc) {
							obtoken();
						} else {
							//err: Se esperaba parentesis de cierre
							exit(2);
						}
					} else {
						if (token == tok_id ) {
							obtoken();
						} else {
							//err: Se esperaba una expresion numerica o una variable
							exit(4);
						}
					}
				}
			}
		}
	}
}

void EXPRESION_CAD() {
	if (IS_FUN_SUBSTRING()) {
		FUN_SUBSTRING();
	} else if (IS_FUN_CONCAT()) {
		FUN_CONCAT();
	} else if (IS_FUN_REPLACE()) {
		FUN_REPLACE();
	} else if (IS_DATA_CAD()) {
		DATA_CAD();
	} else {
		if (token == tok_id) {
			obtoken();
		} else {
			if (token == tok_fscanf) {
				obtoken();
				if (token == tok_parena) {
					obtoken();
					if (token == tok_id) {
						obtoken();
						if (token == tok_parenc) {
							obtoken();
						} else {
							//err: Se esperaba parentesis de cierre
							exit(2);
						}
					} else {
						//err: Se esperaba un identificador
						exit(5);
					}
				} else {
					//err: Se esperaba parentesis de apertura
					exit(1);
				}
			} else {
				if (token == tok_fgets) {
					obtoken();
					if (token == tok_parena) {
						obtoken();
						if (token == tok_id) {
							obtoken();
							if (token == tok_parenc) {
								obtoken();
							} else {
								//err: Se esperaba parentesis de cierre
								exit(2);
							}
						} else {
							//err: Se esperaba un identificador
							exit(5);
						}
					} else {
						//err: Se esperaba parentesis de apertura
						exit(1);
					}
				} else {
					//err: Se esperaba una cadena
					exit(6);
				}
			}
		}

	}
}
void EXPRESION_BOOL() {
	if (token == tok_true || token == tok_false ||
	        token == tok_id) {
		obtoken();
	} else {
		if (token == tok_equal) {
			obtoken();
			if (token == tok_parena) {
				obtoken();
				EXPRESIÓN_CAD();
				if (token == tok_coma) {
					obtoken();
					EXPRESIÓN_CAD();
					if (token == tok_parenc) {
						obtoken();
					} else {
						//err: Se esperaba parentesis de cierre
						exit(2);
					}
				} else {
					//err: Se esperaba una coma
					exit(3);
				}
			} else {
				//err: Se esperaba parentesis de apertura
				exit(1);
			}
		} else {
			//err: Se esperaba un valor booleano
			exit(7);
		}
	}
}

void CONDICION() {
	if (IS_EXPRESION_BOOL()) {
		EXPRESION_BOOL();
	} else if (IS_EXPRESION_NUM()) {
		EXPRESION_NUM();
		if (IS_COMPANUM()) {
			COMPANUM();
		} else if (IS_COMPAGENERAL()) {
			COMPAGENERAL();
		} else {
			//err: Se esperaba un comparador !=, ==, >, >=, < o <=
			exit(10);
		}

		EXPRESION_NUM();
		if (token == tok_or || token == tok_and) {
			obtoken();
			CONDICION();
		} else {
			//err: Se esperaba un operador logico || o &&
			exit(9);
		}
	} else if (IS_EXPRESION_CAD()) {
		COMPAGENERAL();
		EXPRESIÓN_CAD();
		if (token == tok_or || token == tok_and) {
			obtoken();
			CONDICION();
		} else {
			//err: Se esperaba un operador logico || o &&
			exit(9);
		}
	} else {
		//err: Se esperaba una condicion
		exit(8);
	}
}

void COMPAGENERAL() {
	if (token == tok_igual || token == tok_negacion) {
		obtoken();
	} else {
		//err: Se esperaba un comparador == o !=
		exit(11);
	}
}

void COMPANUM() {
	if (token == tok_menor || token == tok_menorigual ||
	        token == tok_mayor || token == tok_mayorigual) {
		obtoken();
	} else {
		//err: Se esperaba un comparador numerico >, >=, < o <=
		exit(12);
	}
}

void CAD_VAR() {}

void FUNCION() {
	if ( token == tok_funcint || token == tok_funcfloat
	        || token ==  tok_funcchar || token ==  tok_funcstr
	        || token == tok_funcvoid || token == tok_bool) {
		obtoken();
	} else {
		//err: Se esperaba un tipo de dato de retorno para la funcion
		exit(14);
	}
}

void VARIABLE() {
	if ( token == tok_arrEntero || token == tok_arrFlotante
	        || token ==  tok_arrCaracter || token ==  tok_arrBooleano
	        || token == tok_arrCadena || token == tok_varint
	        || token == tok_varfloat || token == tok_varchar
	        || token == tok_varstr || token == tok_varbool
	        || token == tok_file) {
		obtoken();
	} else {
		//err: Se esperaba un tipo de dato para la variable
		exit(15);
	}
}
void TIPO_FILE() {
	if ( token == tok_cadena) {
		obtoken();
	} else {
		//err: Se esperaba una cadena
		exit(6);
	}
}

void BLOQUE() {
	while (IS_DECLARACION()) {
		DECLARACION()
	}
	while (IS_FUNCION()) {
		FUNCION();
		if (token == tok_id) {
			obtoken();
			CONJUNVAR();
			if (token == tok_finlinea) {
				obtoken();
				INSTRUCCION();
			} else {
				//err: Se esperaba punto y coma
				exit(16);
			}
		} else {
			//err: Se esperaba un identificador
			exit(5);
		}
	}

}
void PROGRAMA() {
	if (token == tok_main) {
		obtoken();
		if (token == tok_llavea) {
			obtoken();
			BLOQUE();
			if (token == tok_llavec) {
				obtoken();
				while (IS_FUNCION_INSTRUCCION()) {
					FUNCION_INSTRUCCION();
				}
			} else {
				//err: Se esperaba llave de cierre
				exit(19);
			}
		} else {
			//err: Se esperaba llave de apertura
			exit(18);
		}
	} else {
		//err: Se esperaba funcion main
		exit(17);
	}
}
