//un parser descendente deterministico para pl0
#include <stdlib.h>
#include <stdio.h>
#include "pl0.h"
#include "parser.h"
#include "tds.h"
#include "auxiliares.h"
#include "lexico.h"
#include "scanner.h"
#include "conjuntos.h"
#include "codigo_p.h"

//elementos a utilizar en tratamiento de errores
int temp;
int vacio[NOTOKENS];   //conjunto vacío
//int setpaso[NOTOKENS]; //conjunto de paso por valor

void ABRIR_ARCHIVO(int toksig[]) {
	int setpaso[NOTOKENS];
	//printf("*****************ABRIR_ARCHIVO\n");
	if (token == tok_fileopen) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			union_set(setpaso, toksig, sig_data_cad);
			DATA_CAD(setpaso);
			if (token == tok_coma) {
				obtoken();
				TIPO_FILE();
				if (token == tok_parenc) {
					obtoken();
					union_set(setpaso, toksig, sig_instruccion);
					INSTRUCCION(setpaso);
					if (token == tok_fclose) {
						obtoken();
						if (token == tok_parena) {
							obtoken();
							if (token == tok_id) {
								obtoken();
								if (token == tok_parenc) {
									//Se completo exitosamente
									obtoken();
								} else {
									//err: Se esperaba parentesis de cierre
									error(2);
								}
							} else {
								// err: Se esperaba un identificador
								error(5);
							}
						} else {
							// err: Se esperaba parentesis de apertura
							error(1);
						}
					} else {
						// err: Se esperaba la instruccion para cerrar un archivo file.fclose
						error(25);
					}
				} else {
					//err: Se esperaba parentesis de cierre
					error(2);
				}
			} else {
				// err: Se esperaba una coma
				error(3);
			}
		} else {
			// err: Se esperaba parentesis de apertura
			error(1);
		}
	} else {
		// err: Se esperaba la instruccion para abrir un archivo file.fopen
		error(24);
	}

}

void ARREGLO() {

	//printf("*****************ARREGLO\n");
	if (token == tok_corcha) {
		obtoken();
		if (token == tok_flotante || token == tok_numero || token == tok_cadena || token == tok_boolean || token == tok_caracter) {
			if (token == tok_flotante) {
				obtoken();
				while (token == tok_coma) {
					obtoken();
					if (token == tok_flotante) {
						obtoken();
					} else {
						//Err: Se esperaba un flotante.
						error(28);
					}
				}
			} else if (token == tok_numero) {
				obtoken();
				while (token == tok_coma) {
					obtoken();
					if (token == tok_numero) {
						obtoken();
					} else {
						//Err: Se esperaba un entero.
						error(29);
					}
				}
			} else if (token == tok_cadena) {
				obtoken();
				while (token == tok_coma) {
					obtoken();
					if (token == tok_cadena) {
						obtoken();
					} else {
						//Err: Se esperaba una cadena.

						error(6);
					}
				}
			} else if (token == tok_true || token == tok_false) {
				obtoken();
				while (token == tok_coma) {
					obtoken();
					if (token == tok_true || token == tok_false) {
						obtoken();
					} else {
						//Err: Se esperaba un valor booleano.
						error(32);
					}
				}
			} else if (token == tok_caracter) {
				obtoken();
				while (token == tok_coma) {
					obtoken();
					if (token == tok_caracter) {
						obtoken();
					} else {
						//Err: Se esperaba un caracter.
						error(31);
					}
				}
			}

			if (token == tok_corchc) {
				obtoken();
			} else {
				//err: Se esperaba corchete de cierre
				error(27);
			}
		} else {
			//err: Se esperaba un flotante, entero, cadena, booleano o caracter.
			error(1);
		}
	} else {
		//err: Se esperaba corchete de apertura
		error(26);
	}
}

void ASIGNACION(int toksig[]) {
	int setpaso[NOTOKENS];

	//printf("*****************ASIGNACION\n");
	if (token == tok_id) {
		obtoken();
		// Se perdona que se use "==" o ":="
		if ((token == tok_asignar) || (token == tok_igual)) {
			obtoken();
			if (IS_EXPRESION_CAD()) {
				union_set(setpaso, toksig, sig_auxfunc);
				EXPRESION_CAD(setpaso);
			} else if (IS_EXPRESION_BOOL()) {
				union_set(setpaso, toksig, sig_exprbool);
				EXPRESION_BOOL(setpaso);
			} else if (IS_EXPRESION_NUM()) {
				union_set(setpaso, toksig, sig_exprnum);
				EXPRESION_NUM(setpaso);
			} else if (IS_EXPRESION_ARR()) {
				EXPRESION_ARR(toksig);
			} else {
				// Se esperaba una expresion numerica, booleana, cadena o arreglo
				error(20);
			}
		} else {
			// Se esperaba una asignacion con :=
			error(21);
		}
	} else {
		// Se esperaba un identificador
		error(5);
	}
}

void BLOQUE(int toksig[]) {
	int idat; //índice de asignación de memoria, comienza con 3 por ED, DR y EE
	int it0 ; //índice que "recuerda" en donde comienzan las instrucciones de este bloque
	idat = 3;
	it0 = it; //recordamos en donde comienzan en la TDS las declaraciones de este bloque

	//detalle técnico
	registro* elto = getElemento(it);
	elto->tipoDato = TIPO_FUNCION;
	elto->variante.dir = ic;

	gen(SAL, 0, 0); //un procedimiento significa un salto en el código. luego cambiaremos nivel y direccion,
	//los mostrados arriba (0 y 0) son 'paja', lo que pasa es que todavia no sabemos
	//todavía hacia donde saltar

	//printf("*****************BLOQUE\n");
	int setpaso[NOTOKENS]; //conjunto de paso por valor
	init_set(setpaso);
	init_set(vacio);
	do {
		if (IS_DECLARACION()) {
			union_set(setpaso, toksig, sig_declaracion);
			DECLARACION(setpaso);
		}
		while (IS_FUNCION()) {
			FUNCION();
			if (token == tok_id) {
				poner(TIPO_FUNCION);
				obtoken();
				union_set(setpaso, toksig, sig_conjunvar);
				CONJUNVAR(setpaso, 0);
				if (token == tok_finlinea) {
					obtoken();
				} else {
					//err: Se esperaba punto y coma
					error(16);
				}
			} else {
				//err: Se esperaba un identificador
				error(5);

				if (token == tok_finlinea) {
					obtoken();
				} else {
					//err: Se esperaba punto y coma
					//error(16);
					//tok_id, if, while, for, switch, do, console.write, console.read, file.fopen
					setpaso[tok_id] = setpaso[tok_if] = setpaso[tok_while] = setpaso[tok_for] = 1;
					setpaso[tok_switch] = setpaso[tok_do] = setpaso[tok_write] = setpaso[tok_read] = 1;
					setpaso[tok_fileopen] = setpaso[tok_llavec];
					test(setpaso, toksig, 16);
				}
			}
		}

		//parchamos a la TDS y el código intermedio
		//backpatching sobre TDS y código
		registro* elto = getElemento(it0);
		codigo[elto->variante.dir].di = ic;
		elto->variante.dir = ic; //aquí en ic es donde comienza el código para este procedure

		//se abre espacio en la memoria para un mínimo de 3 direcciones
		gen(INS, 0, idat);

		//se copia los tokens siguientes de instruccion
		//copia_set(setpaso,sig_auxllavec); //token siguiente de bloque
		union_set(setpaso, setpaso, sig_instruccion); //token siguiente de instruccion
		INSTRUCCION(setpaso);

		//Este do-while garantiza que lea el bloque de declaración-función-instrucción sin importar el orden.
	} while (tokinidecl[token] == 1);

	//aquí viene el chequeo explícito de que el token que viene a continuación
	//está en el conjunto de sucesores correctos (los sucesores de bloque)
	copia_set(setpaso, sig_auxllavec); //token siguiente de bloque
	test(setpaso, vacio, 51); //símbolo incorrecto detrás de las instrucciones de un bloque
}

void CAD_VAR() {
	if (token == tok_cadena) {
		obtoken();
	} else {
		error(6);
	}
}

void CASE(int toksig[]) {
	//printf("*****************CASE\n");
	int setpaso[NOTOKENS];

	if ( token == tok_case) {
		while (token == tok_case) {
			obtoken();
			union_set(setpaso, toksig, sig_parencondicion);
			PAREN_CONDICION(setpaso);
			if (token == tok_llavea) {
				obtoken();
				union_set(setpaso, toksig, sig_instruccion);
				INSTRUCCION(setpaso);

				if (token == tok_break) {
					obtoken();
					if (token == tok_finlinea) {
						obtoken();
						if (token == tok_llavec) {
							obtoken();
						} else {
							// Se esperaba llave de cierre
							error(19);
							CASE(setpaso);
						}
					} else {
						// Se esperaba punto y coma
						error(16);
					}
				} else {
					// Se esperaba un break
					error(33);
					if (token == tok_llavec) {
						obtoken();
					} else {
						// Se esperaba llave de cierre
						error(19);
						CASE(setpaso);
					}
				}
			} else {
				// Se esperaba llave de apertura
				error(18);
				union_set(setpaso, toksig, sig_instruccion);
				INSTRUCCION(setpaso);

				if (token == tok_break) {
					obtoken();
					if (token == tok_finlinea) {
						obtoken();
						if (token == tok_llavec) {
							obtoken();
						} else {
							// Se esperaba llave de cierre
							error(19);
							CASE(setpaso);
						}
					} else {
						// Se esperaba punto y coma
						error(16);
					}
				} else {
					// Se esperaba un break
					error(33);
					if (token == tok_llavec) {
						obtoken();
					} else {
						// Se esperaba llave de cierre
						error(19);
						CASE(setpaso);
					}
				}
			}
		}
	} else {
		copia_set(setpaso, toksig);
		setpaso[tok_case] = setpaso[tok_default] = 1;
		test(sig_auxllavec, setpaso, 34);
		// Se esperaba una instruccion case
		//error(34);
		if (token == tok_llavec) {
			obtoken();
		} else {
			// Se esperaba llave de cierre
			error(19);
		}
		while (token == tok_case) {
			obtoken();
			union_set(setpaso, toksig, sig_parencondicion);
			PAREN_CONDICION(setpaso);
			if (token == tok_llavea) {
				obtoken();
				union_set(setpaso, toksig, sig_instruccion);
				INSTRUCCION(setpaso);

				if (token == tok_break) {
					obtoken();
					if (token == tok_finlinea) {
						obtoken();
						if (token == tok_llavec) {
							obtoken();
						} else {
							// Se esperaba llave de cierre
							error(19);
						}
					} else {
						// Se esperaba punto y coma
						error(16);
					}
				} else {
					// Se esperaba un break
					error(33);
					if (token == tok_llavec) {
						obtoken();
					} else {
						// Se esperaba llave de cierre
						error(19);
					}
				}
			} else {
				// Se esperaba llave de apertura
				error(18);
			}
		}
	}

	if (token == tok_default) {
		obtoken();
		if (token == tok_llavea) {
			obtoken();
			union_set(setpaso, toksig, sig_instruccion);
			INSTRUCCION(setpaso);
			if (token == tok_break) {
				obtoken();
				if (token == tok_finlinea) {
					obtoken();
					if (token == tok_llavec) {
						obtoken();
					} else {
						// Se esperaba llave de cierre
						error(19);
					}
				} else {
					// Se esperaba punto y coma
					error(16);
				}
			} else {
				// Se esperaba un break
				//error(33);
				if (token == tok_llavec) {
					obtoken();
				} else {
					// Se esperaba llave de cierre
					error(19);
				}
			}
		} else {
			// Se esperaba llave de apertura
			error(18);
		}
	} else {
		// Se esperaba una instruccion default
		error(35);
	}
	copia_set(setpaso, toksig);
	test(sig_auxllavec, setpaso, 18);
}

void COMPAGENERAL() {
	//printf("*****************COMPAGENERAL\n");
	if (token == tok_igual || token == tok_negacion) {
		obtoken();
	} else {
		//err: Se esperaba un comparador == o !=
		error(11);
	}
}

void COMPANUM() {
	//printf("*****************COMPANUM\n");
	if (token == tok_menor || token == tok_menorigual ||
	        token == tok_mayor || token == tok_mayorigual) {
		obtoken();
	} else {
		//err: Se esperaba un comparador numerico >, >=, < o <=
		error(12);
	}
}


void CONDICION(int toksig[]) {
	int setpaso[NOTOKENS]; //conjunto de paso por valor
	init_set(vacio);
	printf("*****************CONDICION\n");
	if (IS_EXPRESION_BOOL()) {
		union_set(setpaso, toksig, sig_exprbool);
		EXPRESION_BOOL(setpaso);
	} else if (IS_EXPRESION_NUM()) {
		union_set(setpaso, toksig, sig_exprnum);
		EXPRESION_NUM(setpaso);
		if (IS_COMPANUM()) {
			COMPANUM();
		} else if (IS_COMPAGENERAL()) {
			COMPAGENERAL();
		} else {
			//err: Se esperaba un comparador !=, ==, >, >=, < o <=
			error(10);
		}
		union_set(setpaso, toksig, sig_exprnum);
		EXPRESION_NUM(setpaso);
		if (token == tok_or || token == tok_and) {
			obtoken();
			CONDICION(toksig);
		}
	} else if (IS_EXPRESION_CAD()) {
		COMPAGENERAL();
		union_set(setpaso, toksig, sig_auxfunc);
		EXPRESION_CAD(setpaso);

		if (token == tok_or || token == tok_and) {
			obtoken();
			CONDICION(toksig);
		}
	} else {
		//err: Se esperaba un expresion que genere un valor booleano
		test(toksig, vacio, 36);
		//error(36);
	}
}

void CONJUNVAR(int toksig[], int declaracion) {
	//printf("*****************CONJUNVAR\n");
	int prim_conjunvar[NOTOKENS]; //para hacer un analisis previo
	init_set(prim_conjunvar);
	prim_conjunvar[tok_parena];

	//test(prim_conjunvar,toksig,54); //El simbolo no corresponde a un conjunto de variables


	if (token == tok_parena) {
		obtoken();
		while (IS_VARIABLE()) {
			VARIABLE();
			if (token == tok_id) {
				if (declaracion)
					poner(TIPO_VARIABLE);
				obtoken();
				if (token == tok_coma) {
					obtoken();
				} else {
					break;
				}
			} else {
				// Se esperaba un identificador
				error(5);
			}
		}
		if (token == tok_parenc) {
			obtoken();
		} else {
			// Se esperaba parentesis de cierre
			error(2);
		}
	} else {
		// Se esperaba parentesis de apertura
		test(toksig, vacio, 1);
		//error(1);
	}
}

void DATA_CAD(int toksig[]) {
	//printf("*****************DATA_CAD\n");
	int setpaso[NOTOKENS]; //conjunto de paso por valor
	init_set(vacio);

	int prim_datacad[NOTOKENS];
	init_set(prim_datacad);
	prim_datacad[tok_id] = prim_datacad[tok_cadena] = prim_datacad[tok_caracter] = 1;
	test(prim_datacad, toksig, 37);

	if (IS_IDENTIFICADOR()) {
		union_set(setpaso, toksig, sig_identificador);
		IDENTIFICADOR(setpaso);
	} else if (token == tok_cadena || token == tok_caracter) {
		obtoken();
	} else {
		// err: se esperaba una cadena, un caracter o un identificador.
		//error(37);
		test(toksig, vacio, 37);
	}
}

void DATA_NUM(int toksig[]) {
	//printf("*****************DATA_NUM\n");
	int setpaso[NOTOKENS];
	init_set(vacio);

	//tok_id, entero, flotante
	int prim_datanum[NOTOKENS];
	init_set(prim_datanum);
	prim_datanum[tok_id] = prim_datanum[tok_numero] = prim_datanum[tok_flotante] = 1;

	test(prim_datanum, toksig, 38);

	if (IS_IDENTIFICADOR()) {
		union_set(setpaso, toksig, sig_identificador);
		IDENTIFICADOR(setpaso);
		if (token == tok_sumasign) {
			obtoken();
			union_set(setpaso, toksig, sig_exprnum);
			EXPRESION_NUM(setpaso);
		}
	} else if (token == tok_numero || token == tok_flotante) {
		//Se completo exitosamente esta parte.
		obtoken();
	} else {
		// err: se esperaba una numero entero, numero flotante o un identificador.
		//error(38);
		test(prim_datanum, toksig, 38);
	}
}

void DECLARACION(int toksig[]) {
	//printf("*****************DECLARACION\n");
	int setpaso[NOTOKENS];
	init_set(setpaso);
	while (IS_VARIABLE()) {

		VARIABLE();

		if (token == tok_id) {
			poner(TIPO_VARIABLE);
			obtoken();
			if (token == tok_finlinea) {
				obtoken();
			} else {
				// Se esperaba punto y coma
				error(16);
			}
		} else {
			// Se esperaba un identificador
			error(5);

			//while(token!= tok_finlinea && IS_VARIABLE()==0){
			while (1) {

				if (token == tok_finlinea) {
					obtoken();
					break;
				}
				if (IS_VARIABLE()) {
					break;
				}
				obtoken();
			}
		}
	}
	//copia_set(setpaso,sig_declaracion);
	//test(setpaso,toksig,16);
}

void EXPRESION_ARR(int toksig[]) {
	int setpaso[NOTOKENS];
	//printf("*****************EXPRESION_ARR\n");
	if (token == tok_split) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			union_set(setpaso, toksig, sig_auxfunc);
			EXPRESION_CAD(setpaso);
			if (token == tok_coma) {
				obtoken();
				union_set(setpaso, toksig, sig_auxfunc);
				EXPRESION_CAD(setpaso);
				if (token == tok_parenc) {
					obtoken();
				} else {
					// Se esperaba parentesis de cierre
					error(2);
				}
			} else {
				// Se esperaba una coma
				error(3);
			}
		} else {
			// Se esperaba parentesis de apertura
			error(1);
		}
	} else if (IS_ARREGLO()) {
		ARREGLO();
	} else {
		// Se esperaba un arreglo
		error(22);
	}
}

void EXPRESION_BOOL(int toksig[]) {
	//printf("*****************EXPRESION_BOOL\n");
	int setpaso[NOTOKENS]; //conjunto de paso por valor
	init_set(vacio);
	if (token == tok_true || token == tok_false ||
	        token == tok_id) {
		obtoken();
	} else {
		if (token == tok_equal) {
			obtoken();
			if (token == tok_parena) {
				obtoken();
				union_set(setpaso, toksig, sig_auxfunc);
				EXPRESION_CAD(setpaso);
				if (token == tok_coma) {
					obtoken();
					union_set(setpaso, toksig, sig_auxfunc);
					EXPRESION_CAD(setpaso);
					if (token == tok_parenc) {
						obtoken();
					} else {
						//err: Se esperaba parentesis de cierre
						error(2);
					}
				} else {
					//err: Se esperaba una coma
					error(3);
				}
			} else {
				//err: Se esperaba parentesis de apertura
				error(1);
			}
		} else {
			copia_set(setpaso, toksig);
			test(setpaso, vacio, 7);
			//err: Se esperaba un valor booleano
			//error(7);
		}
	}
}

void EXPRESION_CAD(int toksig[]) {
	//printf("*****************EXPRESION_CAD\n");
	int setpaso[NOTOKENS]; //conjunto de paso por valor
	init_set(vacio);

	if (token == tok_id) {
		int i = posicion();
		if (i > 0) {
			if ((regEncontrado->tipoDato == TIPO_CADENA || regEncontrado->tipoDato == TIPO_CARACTER) && regEncontrado->tipo == TIPO_FUNCION ) {
				obtoken();
				union_set(setpaso, toksig, sig_conjunvar);
				CONJUNVAR(setpaso, 0);
			} else if ((regEncontrado->tipoDato == TIPO_CADENA || regEncontrado->tipoDato == TIPO_CARACTER ) && regEncontrado->tipo == TIPO_VARIABLE ) {
				union_set(setpaso, toksig, sig_data_cad);
				DATA_CAD(setpaso);
			}
		} else {
			//Identificador no declarado
			error(49);
		}
	}
	if (IS_FUN_SUBSTRING()) {
		union_set(setpaso, toksig, sig_auxfunc);
		FUN_SUBSTRING(setpaso);
	} else if (IS_FUN_CONCAT()) {
		union_set(setpaso, toksig, sig_auxfunc);
		FUN_CONCAT(setpaso);
	} else if (IS_FUN_REPLACE()) {
		union_set(setpaso, toksig, sig_auxfunc);
		FUN_REPLACE(setpaso);
	} else if (IS_DATA_CAD()) {
		union_set(setpaso, toksig, sig_data_cad);
		DATA_CAD(setpaso);
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
							error(2);
						}
					} else {
						//err: Se esperaba un identificador
						error(5);
					}
				} else {
					//err: Se esperaba parentesis de apertura
					error(1);
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
								error(2);
							}
						} else {
							//err: Se esperaba un identificador
							error(5);
						}
					} else {
						//err: Se esperaba parentesis de apertura
						error(1);
					}
				} else {
					//err: Se esperaba una cadena

					test(toksig, vacio, 6);
					//error(6);
				}
			}
		}

	}
}

void EXPRESION_NUM(int toksig[]) {
	int setpaso[NOTOKENS]; //conjunto de paso por valor
	//printf("*****************EXPRESION_NUM\n");
	init_set(vacio);
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
			EXPRESION_NUM(toksig);

			if (token == tok_parenc) {
				obtoken();
			} else {
				//err: Se esperaba parentesis de cierre
				error(2);
			}
		} else {
			//err: Se esperaba parentesis de apertura
			error(1);
		}
	} else {
		if ( token == tok_length ) {
			obtoken();
			if (token == tok_parena) {
				obtoken();
				union_set(setpaso, toksig, sig_auxfunc);
				EXPRESION_CAD(setpaso);
				if (token == tok_parenc) {
					obtoken();
				} else {
					//err: Se esperaba parentesis de cierre
					error(2);
				}
			} else {
				//err: Se esperaba parentesis de apertura
				error(1);
			}
		} else {
			if (token == tok_pow || token == tok_log ) {
				obtoken();
				if (token == tok_parena) {
					obtoken();
					EXPRESION_NUM(toksig);
					if (token == tok_coma) {
						obtoken();
						EXPRESION_NUM(toksig);
						if (token == tok_parenc) {
							obtoken();
						} else {
							//err: Se esperaba parentesis de cierre
							error(2);
						}

					} else {
						//err: Se esperaba una coma
						error(3);
					}
				} else {
					//err: Se esperaba parentesis de apertura
					error(1);
				}
			} else {

				if (IS_OPERACION_NUM()) {
					union_set(setpaso, toksig, sig_operacionnum);
					OPERACION_NUM(setpaso);
				} else if (IS_DATA_NUM()) {
					union_set(setpaso, toksig, sig_data_num);
					DATA_NUM(setpaso);
				} else {
					if (token == tok_parena) {
						obtoken();
						union_set(setpaso, toksig, sig_auxfunc);
						EXPRESION_CAD(setpaso);
						if (token == tok_parenc) {
							obtoken();
						} else {
							//err: Se esperaba parentesis de cierre
							error(2);
						}
					} else {
						if (token == tok_id ) {
							obtoken();
							union_set(setpaso, toksig, sig_conjunvar);
							CONJUNVAR(setpaso, 0);
						} else {
							//err: Se esperaba una expresion numerica o una variable
							test(toksig, vacio, 4);
							//error(4);
						}
					}
				}
			}
		}
	}
}

void FUNCION() {
	//printf("*****************FUNCION\n");
	if ( token == tok_funcint || token == tok_funcfloat
	        || token ==  tok_funcchar || token ==  tok_funcstr
	        || token == tok_funcvoid || token == tok_bool) {
		tipoDato = GET_TIPO_DATO();
		obtoken();
	} else {
		//err: Se esperaba un tipo de dato de retorno para la funcion
		error(14);
	}
}

void FUNCION_INSTRUCCION(int toksig[]) {
	int setpaso[NOTOKENS];
	//printf("*****************FUNCION_INSTRUCCION\n");
	init_set(vacio);
	FUNCION();
	if (token == tok_id) {
		obtoken();
		union_set(setpaso, toksig, sig_conjunvar);
		CONJUNVAR(setpaso, 1);
		if (token == tok_llavea) {
			obtoken();
			union_set(setpaso, toksig, sig_declaracion);
			DECLARACION(setpaso);
			//se copia los tokens siguientes de instruccion
			//copia_set(setpaso,sig_auxllavec); //token siguiente de bloque
			union_set(setpaso, toksig, sig_instruccion); //token siguiente de instruccion
			INSTRUCCION(setpaso);
			if (token == tok_return) {
				obtoken();
				if (IS_EXPRESION_CAD()) {
					union_set(setpaso, toksig, sig_auxfunc);
					EXPRESION_CAD(setpaso);
				} else if (IS_EXPRESION_NUM()) {
					union_set(setpaso, toksig, sig_exprnum);
					EXPRESION_NUM(setpaso);
				} else if (IS_EXPRESION_BOOL()) {
					union_set(setpaso, toksig, sig_exprbool);
					EXPRESION_BOOL(setpaso);
				} else {
					// Se esperaba una expresion numerica, booleana o de cadena
					error(20);
				}
				if (token == tok_finlinea) {
					obtoken();
				} else {
					// Se esperaba punto y coma
					error(16);
				}
			}
			if (token == tok_llavec) {
				obtoken();
			} else {
				// Se esperaba llave de cierre
				error(19);
			}
		} else {
			// Se esperaba llave de apertura
			error(18);
		}
	} else {
		// Se esperaba un identificador
		error(5);
	}


}

void FUN_CONCAT(int toksig[]) {
	//printf("*****************FUN_CONCAT\n");
	int setpaso[NOTOKENS];

	if (token == tok_concat) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			union_set(setpaso, toksig, sig_auxfunc);
			EXPRESION_CAD(setpaso);
			if (token == tok_coma) {
				obtoken();
				union_set(setpaso, toksig, sig_auxfunc);
				EXPRESION_CAD(setpaso);
				if (token == tok_parenc) {
					obtoken();
				} else {
					// Se esperaba parentesis de cierre
					error(2);
				}
			} else {
				// Se esperaba una coma
				error(3);
			}
		} else {
			// Se esperaba parentesis de apertura
			error(1);
		}
	} else {
		// Se esperaba una instruccion concat
		error(39);
	}
}

void FUN_REPLACE(int toksig[]) {
	//printf("*****************FUN_REPLACE\n");
	if (token == tok_replace) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			EXPRESION_CAD(toksig);
			if (token == tok_coma) {
				obtoken();
				EXPRESION_CAD(toksig);
				if (token == tok_coma) {
					obtoken();
					EXPRESION_CAD(toksig);
					if (token == tok_parenc) {
						obtoken();
					} else {
						// Se esperaba parentesis de cierre
						error(2);
					}
				} else {
					// Se esperaba una coma
					error(3);
				}
			} else {
				// Se esperaba una coma
				error(3);
			}
		} else {
			// Se esperaba parentesis de apertura
			error(1);
		}
	} else {
		// Se esperaba una instruccion replace
		error(41);
	}
}

void FUN_SUBSTRING(int toksig[]) {
	int setpaso[NOTOKENS];

	//printf("*****************FUN_SUBSTRING\n");
	if (token == tok_substring) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			union_set(setpaso, toksig, sig_auxfunc);
			EXPRESION_CAD(setpaso);
			if (token == tok_coma) {
				obtoken();
				union_set(setpaso, toksig, sig_exprnum);
				EXPRESION_NUM(setpaso);
				if (token == tok_coma) {
					obtoken();
					union_set(setpaso, toksig, sig_exprnum);
					EXPRESION_NUM(setpaso);
					if (token == tok_parenc) {
						obtoken();
					} else {
						// Se esperaba parentesis de cierre
						error(2);
					}
				} else {
					// Se esperaba una coma
					error(3);
				}
			} else {
				// Se esperaba una coma
				error(3);
			}
		} else {
			// Se esperaba parentesis de apertura
			error(1);
		}
	} else {
		// Se esperaba la instruccion substring
		error(23);
	}
}

void IDENTIFICADOR(int toksig[]) {
	//printf("*****************IDENTIFICADOR\n");
	int setpaso[NOTOKENS]; //conjunto de paso por valor
	int prim_identificador[NOTOKENS];
	init_set(prim_identificador);
	prim_identificador[tok_id] = 1;

	test(prim_identificador, toksig, 5);
	if (token == tok_id) {
		obtoken();
		if (token == tok_corcha) {
			obtoken();
			union_set(setpaso, toksig, sig_exprnum);
			EXPRESION_NUM(setpaso);
			if (token == tok_corchc) {
				//Se completo exitosamente esta parte.
				obtoken();
			} else {
				//err: Se esperaba corchete de cierre
				error(28);
			}
		}
	} else {
		//err: Se esperaba un identificador
		test(prim_identificador, toksig, 5);
		//error(5);
	}
}

void INS_CONSOLEREAD() {

	if (token == tok_read) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			CAD_VAR();
			if (token == tok_coma) {
				obtoken();

				if (token == tok_amp) {
					obtoken();
					if (token == tok_id) {
						obtoken();
						if (token == tok_parenc) {
							obtoken();
						} else {
							// Se esperaba parentesis de cierre
							error(2);
						}
					} else {
						// Se esperaba un identificador
						error(5);
					}
				} else {
					// Se esperaba un ampersand
					error(50);
				}
			} else {
				// Se esperaba una coma
				error(3);
			}
		} else {
			// Se esperaba parentesis de apertura
			error(1);
		}
	} else {
		// Se esperaba una instruccion console.read
		error(41);
	}
}

void INS_CONSOLEWRITE(int toksig[]) {
	int setpaso[NOTOKENS];
	//printf("*****************INS_CONSOLEWRITE\n");
	if (token == tok_write) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			union_set(setpaso, toksig, sig_auxfunc);
			EXPRESION_CAD(setpaso);
			if (token == tok_coma) {
				obtoken();
				if (token == tok_id) {
					obtoken();
					while (token == tok_coma) {
						obtoken();
						if (token == tok_id) {
							obtoken();
						} else {
							// Se esperaba un identificador
							error(5);
						}
					}
				} else {
					// Se esperaba un identificador
					error(5);
				}
			}
			if (token == tok_parenc) {
				obtoken();
			} else {
				// Se esperaba parentesis de cierre
				error(2);
			}
		} else {
			// Se esperaba parentesis de apertura
			error(1);
		}
	} else {
		// Se esperaba una instruccion console.write
		error(42);
	}
}

void INS_DO(int toksig[]) {
	int setpaso[NOTOKENS];
	//printf("*****************INS_DO\n");
	if (token == tok_do) {
		obtoken();
		union_set(setpaso, toksig, sig_llaveinstr);
		LLAVE_INSTRUCCION(setpaso);
		if (token == tok_while) {
			obtoken();
			union_set(setpaso, toksig, sig_parencondicion);
			PAREN_CONDICION(setpaso);
		} else {
			// Se esperaba una instruccion while
			error(44);
		}
	} else {
		// Se esperaba una instruccion do
		error(43);
	}
}

void INS_FOR(int toksig[]) {
	int setpaso[NOTOKENS];
	//printf("*****************INS_FOR\n");
	if (token == tok_for) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			union_set(setpaso, toksig, sig_auxpuntoycoma);
			ASIGNACION(setpaso);
			if (token == tok_finlinea) {
				obtoken();
				union_set(setpaso, toksig, sig_condicion);
				CONDICION(setpaso);
				if (token == tok_finlinea) {
					obtoken();
					union_set(setpaso, toksig, sig_operacionnum);
					OPERACION_NUM(setpaso);
					if (token == tok_parenc) {
						obtoken();
						union_set(setpaso, toksig, sig_llaveinstr);
						LLAVE_INSTRUCCION(setpaso);
						//Se completo
					} else {
						//err: Se esperaba parentesis de cierre
						error(2);
					}
				} else {
					//err: Se esperaba punto y coma
					error(16);
				}
			} else {
				//err: Se esperaba punto y coma
				error(16);
			}
		} else {
			//err: Se esperaba un parentesis de apertura
			error(1);

		}
	} else {
		//err: Se esperaba una instruccion for
		error(45);
	}
}

void INS_IF(int toksig[]) {
	int setpaso[NOTOKENS]; //conjunto de paso por valor
	//printf("*****************INS_IF\n");
	if (token == tok_if) {
		obtoken();
		union_set(setpaso, toksig, sig_parencondicion);
		PAREN_CONDICION(setpaso);
		union_set(setpaso, toksig, sig_llaveinstr);
		LLAVE_INSTRUCCION(setpaso);
	} else {
		//err: Se esperaba un if.
		error(46);
	}
}

void INS_SWITCH(int toksig[]) {
	int setpaso[NOTOKENS];
	//printf("*****************INS_SWITCH\n");
	if (token == tok_switch) {
		obtoken();
		if (token == tok_id) {
			obtoken();
			if (token == tok_llavea) {
				obtoken();
				union_set(setpaso, setpaso, sig_auxllavec);
				CASE(setpaso);
				if (token == tok_llavec) {
					obtoken();
				} else {
					// Se esperaba llave de cierre
					error(19);
				}
			} else {
				// Se esperaba llave de apertura
				error(18);
			}
		} else {
			// Se esperaba un identificador
			error(5);
		}
	} else {
		// Se esperaba una instruccion switch
		error(47);
	}
}

void INS_WHILE(int toksig[]) {
	int setpaso[NOTOKENS];
	//printf("*****************INS_WHILE\n");
	if (token == tok_while) {
		obtoken();
		union_set(setpaso, toksig, sig_parencondicion);
		PAREN_CONDICION(setpaso);
		union_set(setpaso, toksig, sig_llaveinstr);
		LLAVE_INSTRUCCION(setpaso);
	} else {
		//err: Se esperaba una instruccion while
		error(44);
	}
}

void INSTRUCCION(int toksig[]) {
	//printf("*****************INSTRUCCION\n");
	//TODO: Evaluar instrucciones ASIGNACION(), INS_IF(), INS_WHILE(), INS_FOR(), INS_SWITCH(), INS_DO(), INS_CONSOLEWRITE(), INS_CONSOLEREAD(), ABRIR_ARCHIVO(), tok_id
	int setpaso[NOTOKENS]; //conjunto de paso por valor
	init_set(vacio);

	if (token == tok_id) {
		int r = posicion();
		//printf("***************** POSICION %d \n", r);
		if (r > 0) {
			if (regEncontrado->tipoDato != TIPO_VOID) {
				union_set(setpaso, toksig, sig_auxpuntoycoma);
				ASIGNACION(setpaso);
				if (token == tok_finlinea) {
					//Se completo
					obtoken();
					copia_set(setpaso, toksig);
					INSTRUCCION(setpaso);

				} else {
					//err: Se esperaba punto y coma
					error(16);
				}
			} else {
				obtoken();
				copia_set(setpaso, toksig);
				INSTRUCCION(setpaso);

			}
		} else {
			//Identificador no declarado
			//error(49);
			test(toksig, vacio, 49);
		}
		copia_set(setpaso, toksig);
		INSTRUCCION(setpaso);

	} else if (token == tok_if) {
		union_set(setpaso, toksig, sig_auxinstr);
		INS_IF(setpaso);
		copia_set(setpaso, toksig);
		INSTRUCCION(setpaso);

	} else if (token == tok_while) {
		union_set(setpaso, toksig, sig_auxinstr);
		INS_WHILE(setpaso);
		copia_set(setpaso, toksig);
		INSTRUCCION(setpaso);

	} else if (token == tok_for) {
		union_set(setpaso, toksig, sig_auxinstr);
		INS_FOR(setpaso);
		copia_set(setpaso, toksig);
		INSTRUCCION(setpaso);

	} else if (token == tok_switch) {
		union_set(setpaso, toksig, sig_auxinstr);
		INS_SWITCH(setpaso);
		copia_set(setpaso, toksig);
		INSTRUCCION(setpaso);

	} else if (token == tok_do) {
		union_set(setpaso, toksig, sig_auxinstr);
		INS_DO(setpaso);
		copia_set(setpaso, toksig);
		INSTRUCCION(setpaso);

	} else if (token == tok_write) {
		union_set(setpaso, toksig, sig_auxpuntoycoma);
		INS_CONSOLEWRITE(setpaso);
		if (token == tok_finlinea) {
			obtoken();
			copia_set(setpaso, toksig);
			INSTRUCCION(setpaso);

		} else {
			//err: Se esperaba punto y coma
			error(16);
		}
	} else if (token == tok_read) {
		INS_CONSOLEREAD();
		if (token == tok_finlinea) {
			//Se completo
			obtoken();
			copia_set(setpaso, toksig);
			INSTRUCCION(setpaso);

		} else {
			//err: Se esperaba punto y coma
			error(16);
		}
	} else if (token == tok_fileopen) {
		union_set(setpaso, toksig, sig_auxpuntoycoma);
		ABRIR_ARCHIVO(setpaso);
		if (token == tok_finlinea) {
			//Se completo
			obtoken();
			copia_set(setpaso, toksig);
			INSTRUCCION(setpaso);

		} else {
			//err: Se esperaba punto y coma
			error(16);
		}
	}

	union_set(setpaso, setpaso, sig_instruccion);
	test(setpaso, vacio, 52); //error(52): Un simbolo incorrecto sigue a una instrucción
}

void LLAVE_INSTRUCCION(int toksig[]) {
	//printf("*****************LLAVE_INSTRUCCION\n");
	int setpaso[NOTOKENS];

	if (token == tok_llavea) {
		obtoken();
		union_set(setpaso, toksig, sig_instruccion);
		INSTRUCCION(setpaso);
		if (token == tok_llavec) {
			//EXITOOOO
			obtoken();
		} else {
			//err: Se esperaba llave de cierre
			error(19);
		}
	} else {
		//err: Se esperaba llave de apertura
		error(18);
	}
}

void OPERACION_NUM(int toksig[]) {
	//printf("*****************OPERACION_NUM\n");
	int setpaso[NOTOKENS]; //conjunto de paso por valor
	init_set(vacio);
	if (token != tok_sum && token != tok_resta)
	{
		error(55);
		union_set(setpaso, toksig, sig_termino);
		TERMINO(setpaso);
	}

	while (token == tok_sum || token == tok_resta) {
		obtoken();
		union_set(setpaso, toksig, sig_termino);
		TERMINO(setpaso);
	}

	test(toksig, vacio, 55); //se esperaba un simbolo de operacion numerica
}

void PAREN_CONDICION(int toksig[]) {
	int setpaso[NOTOKENS]; //conjunto de paso por valor
	//printf("*****************PAREN_CONDICION\n");
	if (token == tok_parena) {
		obtoken();
		union_set(setpaso, toksig, sig_condicion);
		CONDICION(setpaso);
		if (token == tok_parenc) {
			//EXITOOOO
			obtoken();
		} else {
			//err: Se esperaba un parentesis de cierre.
			error(2);
		}
	} else {
		//err: Se esperaba un parentesis de apertura.
		error(1);
	}
}

void PROGRAMA() {
	//printf("*****************PROGRAMA\n");
	if (token == tok_main) {
		obtoken();
		if (token == tok_llavea) {
			obtoken();
			BLOQUE(sig_auxllavec);
			if (token == tok_llavec) {
				obtoken();
				while (IS_FUNCION_INSTRUCCION()) {
					FUNCION_INSTRUCCION(sig_funcinst);
				}
			} else {
				//err: Se esperaba llave de cierre
				error(19);
			}
		} else {
			//err: Se esperaba llave de apertura
			error(18);
		}
	} else {
		//err: Se esperaba funcion main
		error(17);
	}
}

void TERMINO(int toksig[]) {
	int setpaso[NOTOKENS];
	init_set(vacio);
	//printf("*****************TERMINO\n");
	//={ round, sin, cos, arcsin, arccos, arctan, tan, sqrt, length, pow, log, tok_id, entero, flotante, +, - , (,  }
	int prim_termino[NOTOKENS];
	init_set(prim_termino);
	prim_termino[tok_round] = prim_termino[tok_sin] = prim_termino[tok_cos] = prim_termino[tok_arcsin] = 1;
	prim_termino[tok_arccos] = prim_termino[tok_arctan] = prim_termino[tok_tan] = prim_termino[tok_sqrt] = 1;
	prim_termino[tok_length] = prim_termino[tok_pow] = prim_termino[tok_log] = prim_termino[tok_id] = 1;
	prim_termino[tok_numero] = prim_termino[tok_flotante] = prim_termino[tok_sum] = prim_termino[tok_resta] = 1;
	prim_termino[tok_parena] = 1;

	test(prim_termino, toksig, 56); //El simbolo no corresponde a un termino valido

	union_set(setpaso, toksig, sig_exprnum);
	EXPRESION_NUM(setpaso);
	while (token == tok_multi || token == tok_divi) {
		obtoken();
		EXPRESION_NUM(setpaso);
	}
}

void TIPO_FILE() {
	//printf("*****************TIPO_FILE\n");
	if ( token == tok_cadena) {
		obtoken();
	} else {
		//err: Se esperaba una cadena

		error(6);
	}
}

void VARIABLE() {
	//printf("*****************VARIABLE\n");
	tipoDato = GET_TIPO_DATO();
	if ( token == tok_arrEntero || token == tok_arrFlotante
	        || token ==  tok_arrCaracter || token ==  tok_arrBooleano
	        || token == tok_arrCadena || token == tok_varint
	        || token == tok_varfloat || token == tok_varchar
	        || token == tok_varstr || token == tok_varbool
	        || token == tok_file) {
		obtoken();
	} else {
		//err: Se esperaba un tipo de dato para la variable
		error(15);
	}
}



