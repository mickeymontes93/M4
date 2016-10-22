//un parser descendente determinístico para pl0
#include <stdlib.h>
#include "parser.h"
#include "tds.h"
#include "auxiliares.h"
#include "lexico.h"
#include "scanner.h"

//funciones auxiliares del parser
int CHECK_VARIABLE();

//funciones internas al parser
void FUNCION_INSTRUCCION(), CONJUNVAR(), DECLARACIÓN(), OPERACION_NUM(), TERMINO(), ASIGNACIÓN(), FUN_SUBSTRING(), FUN_CONCAT();
void FUN_REPLACE(), INS_DO(), INS_CONSOLEWRITE(), INS_CONSOLEREAD(), INS_SWITCH(), CASE(), ABRIR_ARCHIVO(), INS_IF(), INS_WHILE();
void INS_FOR(), INSTRUCCION(), PAREN_CONDICIÓN(), LLAVE_INSTRUCCION(), DATA_NUM(), DATA_CAD(), ARREGLO(), IDENTIFICADOR();
void EXPRESION_NUM(), EXPRESIÓN_CAD(), EXPRESION_BOOL(), CONDICION(), COMPAGENERAL(), COMPANUM(), CAD_VAR(), FUNCION(), VARIABLE();
void TIPO_FILE(), BLOQUE();

int CHECK_VARIABLE()
{
	if( token == tok_arrEntero ||
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
	}
	else
	{
		return 0;
	}
		   
}

void FUNCION_INSTRUCCION() 
{
	FUNCION();
	if(token == tok_id)
	{
		obtok();
		CONJUNVAR();
		if(token == tok_llavea)
		{
			obtok();
			DECLARACIÓN();
			INSTRUCCION();
			if(token == tok_return)
			{
				// TODO: Evaluar ExpresionCad, ExpresionNum, ExpresionBoll
				if(token == tok_finlinea)
				{
					obtoken();
				}
				else
				{
					// TODO: modificar error
					error(1);
				}
			}
			if(token == tok_llavec)
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

void CONJUNVAR() 
{
	if(token == tok_parena)
	{
		obtoken();
		while(token != tok_parenc)
		{
			VARIABLE();
			if(token == tok_id)
			{
				obtoken();
				if(token != tok_coma)
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
		
		if(token == tok_parenc)
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

void DECLARACIÓN() 
{
	int isVariable = CHECK_VARIABLE();
	while(isVariable == 1)
	{
		VARIABLE();
		if(token == tok_id)
		{
			obtoken();
			if(token == tok_finlinea)
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

void OPERACION_NUM() 
{ 
	while (token == tok_sum || token == tok_resta)
	{
		obtoken();
		TERMINO();
	}
}

void TERMINO()
{
	do
	{
		EXPRESION_NUM();
	}while(token == tok_multi || toke == tok_divi)

}
void ASIGNACIÓN()
{
	if(token == tok_id)
	{
		obtoken();
		if(token == tok_asignar)
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

void FUN_SUBSTRING()
{
	if(token == tok_substring)
	{
		obtoken();
		if(token == tok_parena)
		{
			obtoken();
			EXPRESION_NUM();
			if(token == tok_coma)
			{
				obtoken();
				EXPRESION_NUM();
				if(token == tok_coma)
				{
					obtoken();
					EXPRESION_NUM();
					if(token == tok_parenc)
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

void FUN_CONCAT()
{
	if(token == tok_concat)
	{
		obtoken();
		if(token == tok_parena)
		{
			obtoken();
			EXPRESION_CAD();
			if(token == tok_coma)
			{
				obtoken();
				EXPRESION_CAD();
				if(token == tok_parenc)
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

void FUN_REPLACE() 
{
	if(token == tok_replace)
	{
		obtoken();
		if(token == tok_parena)
		{
			obtoken();
			EXPRESION_CAD();
			if(token == tok_coma)
			{
				obtoken();
				EXPRESION_CAD();
				if(token == tok_coma)
				{
					obtoken();
					EXPRESION_CAD();
					if(token == tok_parenc)
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

void INS_DO()
{
	if(token == tok_do)
	{
		obtoken();
		LLAVE_INSTRUCCION();
		if(token == tok_while)
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

void INS_CONSOLEWRITE()
{
	if(token == tok_write)
	{
		obtoken();
		if(toke == tok_parena)
		{
			obtoken();
			EXPRESION_CAD();
			if(toke == tok_coma)
			{
				obtoken();
				do
				{
					if(token == tok_id)
					{
						obtoken();
					}
					else
					{
						// TODO: modificar error
						error(1);
					}
				}while(token == tok_coma);
				
				if(token == tok_parenc)
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

void INS_CONSOLEREAD()
{ 
	if(token == tok_read)
	{
		obtoken();
		if(token == tok_parena)
		{
			obtoken();
			CAD_VAR();
			if(token == tok_coma)
			{
				obtoken();
				if(token == tok_amp)
				{
					obtoken();
					if(token == tok_id)
					{
						obtoken();
						if(token == tok_parenc)
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
void INS_SWITCH()
{
	if(token == tok_switch)
	{
		obtoken();
		if(token == tok_id)
		{
			obtoken();
			if(token == tok_llavea)
			{
				obtoken();
				CASE();
				if(token == tok_llaveC)
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

void CASE() 
{
	while(token == tok_case)
	{
		obtoken();
		PAREN_CONDICIÓN();
		if(token == tok_llavea)
		{
			obtoken();
			INSTRUCCION();
			if(token == tok_break)
			{
				obtoken();
				if(token == tok_finlinea)
				{
					obtoken();
					if(token == tok_llavec)
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
	if(token == tok_default)
	{
		obtoken();
		if(token == tok_llavea)
		{
			obtoken();
			INSTRUCCION();
			if(token == tok_break)
			{
				obtoken();
				if(token == tok_finlinea)
				{
					obtoken();
					if(token == tok_llavec)
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

