//un parser descendente deterministico para pl0
#include <stdlib.h>
#include "parser.h"
#include "tds.h"
#include "auxiliares.h"
#include "lexico.h"
#include "scanner.h"


void ABRIR_ARCHIVO() {
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
						exit(28);
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
						exit(29);
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
						exit(30);
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
						exit(32);
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
						exit(31);
					}
				}
			}

			if (token == tok_corchc) {
				obtoken();
			} else {
				//err: Se esperaba corchete de cierre
				exit(27);
			}
		} else {
			//err: Se esperaba un flotante, entero, cadena, booleano o caracter.
			exit(1);
		}
	} else {
		//err: Se esperaba corchete de apertura
		exit(26);
	}
}

void ASIGNACION() {
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
				exit(16);
			}
		} else {
			//err: Se esperaba un identificador
			exit(5);
		}
	}
	INSTRUCCION();
}

void CAD_VAR() {}

void CASE() {
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
		exit(36);
	}
}

void CONJUNVAR() {
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
						EXPRESION_CAD();
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

void FUNCION_INSTRUCCION() {
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
	if (token == tok_llavea) {
		obtoken();
		INSTRUCCION();
		if (token == tok_llavec) {
			//EXITOOOO
			obtoken();
		} else {
			//err: Se esperaba llave de cierre
			exit(19);
		}
	} else {
		//err: Se esperaba llave de apertura
		exit(18);
	}
}

void OPERACION_NUM() {
	while (token == tok_sum || token == tok_resta) {
		obtoken();
		TERMINO();
	}
}

void PAREN_CONDICION() {
	if (token == tok_parena) {
		obtoken();
		CONDICION();
		if (token == tok_parenc) {
			//EXITOOOO
			obtoken();
		} else {
			//err: Se esperaba un parentesis de cierre.
			exit(2);
		}
	} else {
		//err: Se esperaba un parentesis de apertura.
		exit(1);
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

void TERMINO() {
	EXPRESION_NUM();
	while (token == tok_multi || token == tok_divi) {
		obtoken();
		EXPRESION_NUM();
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



