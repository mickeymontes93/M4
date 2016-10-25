//un parser descendente deterministico para pl0
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "tds.h"
#include "auxiliares.h"
#include "lexico.h"
#include "scanner.h"


void ABRIR_ARCHIVO() {
	printf("*****************ABRIR_ARCHIVO\n");
	if (token == tok_fileopen) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			DATA_CAD();
			if (token == tok_coma) {
				obtoken();
				TIPO_FILE();
				if (token == tok_parenc) {
					obtoken();
					INSTRUCCION();
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

	printf("*****************ARREGLO\n");
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
						error(30);
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

void ASIGNACION() {

	printf("*****************ASIGNACION\n");
	if (token == tok_id) {
		obtoken();
		if (token == tok_asignar) {
			obtoken();
			if (IS_EXPRESION_CAD())
				EXPRESION_CAD();
			else if (IS_EXPRESION_NUM())
				EXPRESION_NUM();
			else if (IS_EXPRESION_BOOL())
				EXPRESION_BOOL();
			else if (IS_EXPRESION_ARR())
				EXPRESION_ARR();
			else {
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

void BLOQUE() {
	printf("*****************BLOQUE\n");
	if (IS_DECLARACION()) {
		DECLARACION();
	}
	while (IS_FUNCION()) {
		FUNCION();
		if (token == tok_id) {
			obtoken();
			CONJUNVAR();
			if (token == tok_finlinea) {
				obtoken();
			} else {
				//err: Se esperaba punto y coma
				error(16);
			}
		} else {
			//err: Se esperaba un identificador
			error(5);
		}
	}
	INSTRUCCION();
}

void CAD_VAR() {}

void CASE() {
	printf("*****************CASE\n");
	if ( token == tok_case) {
		while (token == tok_case) {
			obtoken();
			PAREN_CONDICION();
			if (token == tok_llavea) {
				obtoken();
				INSTRUCCION();
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
				}
			} else {
				// Se esperaba llave de apertura
				error(18);
			}
		}
	} else {
		// Se esperaba una instruccion case
		error(34);
	}

	if (token == tok_default) {
		obtoken();
		if (token == tok_llavea) {
			obtoken();
			INSTRUCCION();
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
			}
		} else {
			// Se esperaba llave de apertura
			error(18);
		}
	} else {
		// Se esperaba una instruccion default
		error(35);
	}
}

void COMPAGENERAL() {
	printf("*****************COMPAGENERAL\n");
	if (token == tok_igual || token == tok_negacion) {
		obtoken();
	} else {
		//err: Se esperaba un comparador == o !=
		error(11);
	}
}

void COMPANUM() {
	printf("*****************COMPANUM\n");
	if (token == tok_menor || token == tok_menorigual ||
	        token == tok_mayor || token == tok_mayorigual) {
		obtoken();
	} else {
		//err: Se esperaba un comparador numerico >, >=, < o <=
		error(12);
	}
}


void CONDICION() {
	printf("*****************CONDICION\n");
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
			error(10);
		}

		EXPRESION_NUM();
		if (token == tok_or || token == tok_and) {
			obtoken();
			CONDICION();
		}
	} else if (IS_EXPRESION_CAD()) {
		COMPAGENERAL();
		EXPRESION_CAD();

		if (token == tok_or || token == tok_and) {
			obtoken();
			CONDICION();
		}
	} else {
		//err: Se esperaba un expresion que genere un valor booleano
		error(36);
	}
}

void CONJUNVAR() {
	printf("*****************CONJUNVAR\n");
	if (token == tok_parena) {
		obtoken();
		while (IS_VARIABLE()) {
			VARIABLE();
			if (token == tok_id) {
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
		error(1);
	}
}

void DATA_CAD() {
	printf("*****************DATA_CAD\n");
	if (IS_IDENTIFICADOR()) {
		IDENTIFICADOR();
	} else if (token == tok_cadena || token == tok_caracter) {
		obtoken();
	} else {
		// err: se esperaba una cadena, un caracter o un identificador.
		error(37);
	}
}

void DATA_NUM() {
	printf("*****************DATA_NUM\n");
	if (IS_IDENTIFICADOR()) {
		IDENTIFICADOR();
		if (token == tok_sumasign) {
			obtoken();
			EXPRESION_NUM();
		}
	} else if (token == tok_numero || token == tok_flotante) {
		//Se completo exitosamente esta parte.
		obtoken();
	} else {
		// err: se esperaba una numero entero, numero flotante o un identificador.
		error(38);
	}
}

void DECLARACION() {
	printf("*****************DECLARACION\n");
	while (IS_VARIABLE()) {
		VARIABLE();
		if (token == tok_id) {
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
		}
	}
}

void EXPRESION_ARR() {
	printf("*****************EXPRESION_ARR\n");
	if (token == tok_split) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			EXPRESION_CAD();
			if (token == tok_coma) {
				obtoken();
				EXPRESION_CAD();
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

void EXPRESION_BOOL() {
	printf("*****************EXPRESION_BOOL\n");
	if (token == tok_true || token == tok_false ||
	        token == tok_id) {
		obtoken();
	} else {
		if (token == tok_equal) {
			obtoken();
			if (token == tok_parena) {
				obtoken();
				EXPRESION_CAD();
				if (token == tok_coma) {
					obtoken();
					EXPRESION_CAD();
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
			//err: Se esperaba un valor booleano
			error(7);
		}
	}
}

void EXPRESION_CAD() {
	printf("*****************EXPRESION_CAD\n");
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
					error(6);
				}
			}
		}

	}
}

void EXPRESION_NUM() {
	printf("*****************EXPRESION_NUM\n");
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
				EXPRESION_CAD();
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
					EXPRESION_NUM();
					if (token == tok_coma) {
						obtoken();
						EXPRESION_NUM();
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
				if (IS_DATA_NUM()) {
					DATA_NUM();
				} else if (IS_OPERACION_NUM()) {
					OPERACION_NUM();
				} else {
					if (token == tok_parena) {
						obtoken();
						EXPRESION_CAD();
						if (token == tok_parenc) {
							obtoken();
						} else {
							//err: Se esperaba parentesis de cierre
							error(2);
						}
					} else {
						if (token == tok_id ) {
							obtoken();
						} else {
							//err: Se esperaba una expresion numerica o una variable
							error(4);
						}
					}
				}
			}
		}
	}
}

void FUNCION() {
	printf("*****************FUNCION\n");
	if ( token == tok_funcint || token == tok_funcfloat
	        || token ==  tok_funcchar || token ==  tok_funcstr
	        || token == tok_funcvoid || token == tok_bool) {
		obtoken();
	} else {
		//err: Se esperaba un tipo de dato de retorno para la funcion
		error(14);
	}
}

void FUNCION_INSTRUCCION() {
	printf("*****************FUNCION_INSTRUCCION\n");
	FUNCION();
	if (token == tok_id) {
		obtoken();
		CONJUNVAR();
		if (token == tok_llavea) {
			obtoken();
			DECLARACION();
			INSTRUCCION();
			if (token == tok_return) {
				obtoken();
				if (IS_EXPRESION_CAD())
					EXPRESION_CAD();
				else if (IS_EXPRESION_NUM())
					EXPRESION_NUM();
				else if (IS_EXPRESION_BOOL())
					EXPRESION_BOOL();
				else {
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

void FUN_CONCAT() {
	printf("*****************FUN_CONCAT\n");
	if (token == tok_concat) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			EXPRESION_CAD();
			if (token == tok_coma) {
				obtoken();
				EXPRESION_CAD();
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

void FUN_REPLACE() {
	printf("*****************FUN_REPLACE\n");
	if (token == tok_replace) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			EXPRESION_CAD();
			if (token == tok_coma) {
				obtoken();
				EXPRESION_CAD();
				if (token == tok_coma) {
					obtoken();
					EXPRESION_CAD();
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

void FUN_SUBSTRING() {
	printf("*****************FUN_SUBSTRING\n");
	if (token == tok_substring) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			EXPRESION_CAD();
			if (token == tok_coma) {
				obtoken();
				EXPRESION_NUM();
				if (token == tok_coma) {
					obtoken();
					EXPRESION_NUM();
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

void IDENTIFICADOR() {
	printf("*****************IDENTIFICADOR\n");
	if (token == tok_id) {
		obtoken();
		if (token == tok_corcha) {
			obtoken();
			EXPRESION_NUM();
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
		error(5);
	}
}

void INS_CONSOLEREAD() {
	printf("*****************INS_CONSOLEREAD\n");
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
					error(43);
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

void INS_CONSOLEWRITE() {
	printf("*****************INS_CONSOLEWRITE\n");
	if (token == tok_write) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			EXPRESION_CAD();
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
				// Se esperaba una coma
				error(3);
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

void INS_DO() {
	printf("*****************INS_DO\n");
	if (token == tok_do) {
		obtoken();
		LLAVE_INSTRUCCION();
		if (token == tok_while) {
			obtoken();
			PAREN_CONDICION();
		} else {
			// Se esperaba una instruccion while
			error(44);
		}
	} else {
		// Se esperaba una instruccion do
		error(43);
	}
}

void INS_FOR() {
	printf("*****************INS_FOR\n");
	if (token == tok_for) {
		obtoken();
		if (token == tok_parena) {
			obtoken();
			ASIGNACION();
			if (token == tok_finlinea) {
				obtoken();
				CONDICION();
				if (token == tok_finlinea) {
					obtoken();
					OPERACION_NUM();
					if (token == tok_parenc) {
						obtoken();
						LLAVE_INSTRUCCION();
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

void INS_IF() {
	printf("*****************INS_IF\n");
	if (token == tok_if) {
		obtoken();
		PAREN_CONDICION();
		LLAVE_INSTRUCCION();
	} else {
		//err: Se esperaba un if.
		error(46);
	}
}

void INS_SWITCH() {
	printf("*****************INS_SWITCH\n");
	if (token == tok_switch) {
		obtoken();
		if (token == tok_id) {
			obtoken();
			if (token == tok_llavea) {
				obtoken();
				CASE();
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

void INS_WHILE() {
	printf("*****************INS_WHILE\n");
	if (token == tok_while) {
		obtoken();
		PAREN_CONDICION();
		LLAVE_INSTRUCCION();
	} else {
		//err: Se esperaba una instruccion while
		error(44);
	}
}

void INSTRUCCION() {
	printf("*****************INSTRUCCION\n");
	//TODO: Evaluar instrucciones ASIGNACION(), INS_IF(), INS_WHILE(), INS_FOR(), INS_SWITCH(), INS_DO(), INS_CONSOLEWRITE(), INS_CONSOLEREAD(), ABRIR_ARCHIVO(), tok_id

	if (token == tok_id) {
		ASIGNACION();
		if (token == tok_finlinea) {
			//Se completo
			obtoken();
		} else {
			//err: Se esperaba punto y coma
			error(16);
		}
	} else if (token == tok_if) {
		INS_IF();
	} else if (token == tok_while) {
		INS_WHILE();
	} else if (token == tok_for) {
		INS_FOR();
	} else if (token == tok_switch) {
		INS_SWITCH();
	} else if (token == tok_do) {
		INS_DO();
	} else if (token == tok_write) {
		INS_CONSOLEWRITE();
		if (token == tok_finlinea) {
			obtoken();
		} else {
			//err: Se esperaba punto y coma
			error(16);
		}
	} else if (token == tok_read) {
		INS_CONSOLEREAD();
		if (token == tok_finlinea) {
			//Se completo
			obtoken();
		} else {
			//err: Se esperaba punto y coma
			error(16);
		}
	} else if (token == tok_fileopen) {
		ABRIR_ARCHIVO();
		if (token == tok_finlinea) {
			//Se completo
			obtoken();
		} else {
			//err: Se esperaba punto y coma
			error(16);
		}
	} else {
		//err: Se esperaba una intruccion if, while, for, write, read, switch, do o fileopen.
		error(48);
	}
}

void LLAVE_INSTRUCCION() {
	printf("*****************LLAVE_INSTRUCCION\n");
	if (token == tok_llavea) {
		obtoken();
		INSTRUCCION();
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

void OPERACION_NUM() {
	printf("*****************OPERACION_NUM\n");
	while (token == tok_sum || token == tok_resta) {
		obtoken();
		TERMINO();
	}
}

void PAREN_CONDICION() {
	printf("*****************PAREN_CONDICION\n");
	if (token == tok_parena) {
		obtoken();
		CONDICION();
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
	printf("*****************PROGRAMA\n");
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

void TERMINO() {
	printf("*****************TERMINO\n");
	EXPRESION_NUM();
	while (token == tok_multi || token == tok_divi) {
		obtoken();
		EXPRESION_NUM();
	}
}

void TIPO_FILE() {
	printf("*****************TIPO_FILE\n");
	if ( token == tok_cadena) {
		obtoken();
	} else {
		//err: Se esperaba una cadena
		error(6);
	}
}

void VARIABLE() {
	printf("*****************VARIABLE\n");
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



