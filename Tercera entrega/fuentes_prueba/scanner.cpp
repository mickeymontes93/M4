//el analizador lexicogr�fico de pl0
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "scanner.h"
#include "pl0.h"
#include "lexico.h"
#include "auxiliares.h"


int ll;                   //contador de caracteres
int offset;               //corrimiento en la lectura de los caracteres del programa fuente
int fin_de_archivo;       //bandera de fin de archivo (obtch)
int ch;                   //�ltimo caracter le�do

//union valor de una lexeme correspondiene a un tipo de dato
valorPorTipo valor;

int ln = 0;				  //contador de lineas;

int obtch(), getline(char s[], int lim); //funciones internas a scanner.cpp

char charTemp[2];
int k = 0;

//tabla que guarda las cadena y sus identificadores de variables en cada fila.
char palabras[MAXSTRING][MAXSTRING];
char palabrasVar[MAXSTRING][MAXSTRING];
char strTemp[MAXSTRING];

void obtokenHelper()
{
	char lexid[MAXID + 1]; //+1 para colocar el marcador "\0"
	int i, j;
	int ok = 0;
//quitar blancos, caracter de cambio de l�nea y tabuladores
	while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t' || (int) ch < 0) ch = obtch() ;


	//Si comienza con comillas dobles (34 en ascii), es String hasta donde terminen las comillas
	if ((int) ch == 34) {
		lexid[0] = ch;
		i = 1;
		do {
			ch = obtch();
			if (ch == '%') {
				charTemp[0] = ch;
				ch = obtch();
				if (ch == 'd' || ch == 'f' || ch == 's' || ch == 'c') {
					charTemp[1] = ch;
					lexid[i] = charTemp[0];
					i++;
					lexid[i] = charTemp[1];
					i++;
					strcat(strTemp, charTemp);
					strcat(strTemp, ",");
				}
				else {
					lexid[i] = ch;
					i++;
				}
			} else {
				lexid[i] = ch;
				i++;
			}
		} while (ch != 34);
		ch = obtch();
		lexid[i] = '\0';
		//printf("\n%s", lexid);
		if (i == 3) {
			token = tok_caracter;
			valor.caracter = lexid[1];
		} else {
			token = tok_cadena;
			strncpy(valor.cadena, lexid + 1, strlen(lexid) - 1);
		}

		strTemp[strlen(strTemp) - 1] = '\0';
		strcpy(palabrasVar[k], strTemp);
		strcpy(palabras[k], lexid);
		k++;
		strcpy(strTemp, "");
	}
	//Si comienza con comillas simples (39 en ascii), es String hasta donde terminen las comillas
	else if ((int) ch == 39) {
		lexid[0] = ch;
		i = 1;
		do {
			ch = obtch();
			if (ch == '%') {
				charTemp[0] = ch;
				ch = obtch();
				if (ch == 'd' || ch == 'f' || ch == 's' || ch == 'c') {
					charTemp[1] = ch;
					lexid[i] = charTemp[0];
					i++;
					lexid[i] = charTemp[1];
					i++;
					strcat(strTemp, charTemp);
					strcat(strTemp, ",");
				}
				else {
					lexid[i] = ch;
					i++;
				}
			} else {
				lexid[i] = ch;
				i++;
			}
			if (strlen(strTemp) == 17)
				break;
		} while (ch != 39);
		ch = obtch();
		lexid[i] = '\0';
		//printf("\n%s", lexid);
		if (i == 3) {
			token = tok_caracter;
			valor.caracter = lexid[1];
		} else {
			token = tok_cadena;
			strncpy(valor.cadena, lexid + 1, strlen(lexid) - 1);
		}

		strTemp[strlen(strTemp) - 1] = '\0';
		strcpy(palabrasVar[k], strTemp);
		strcpy(palabras[k], lexid);
		k++;
		strcpy(strTemp, "");
	}
	//Si comienza con # (35 en ascii), es un comentario.
	else if ((int) ch == 35) {
		do {
			ch = obtch();
		} while (ch != '\n');
		ch = obtch();
		token = COMENTARIO;
	}

	else
//si la lexeme comienza con una letra, es identificador o palabra reservada
		if (isalpha(ch) ) {
			lexid[0] = ch;

			i = 1;
			int j;
			while ( isalpha( (ch = obtch()) ) ||  isdigit(ch) || ch == '.' || ch == '_') {
				//printf("%d",MAXID);
				if (i < MAXID) lexid[i++] = ch;
			}
			lexid[i] = '\0';

			//---------------BUSQUEDA BINARIA -----------------------
			ok = busqueda_binaria_palabras_reservadas((int) MAXPAL / 2, lexid);
			/*for (j=0;j<MAXPAL;++j)
				if (strcmp(lexid,lexpal[j])==0) {
					ok=1;
					break;
				}*/

			//Este descomentar para busqueda binaria
			if (ok != -1) {
				token = tokpal[ok]; //es palabra reservada
				if (token == tok_false) {
					valor.booleano = 0;
				} else if (token == tok_true) {
					valor.booleano = 1;
				}
			}
			/*if (ok == 1)
				token= tokpal[j];*/
			else // Es identificador
				token = tok_id;

			//printf("\n%s", lexid);

			strcpy(lex, lexid); //copiar en lex
		}
		else //si comienza con un d�gito...

			if (isdigit(ch)) {
				int es_flotante = 0; // ****** cambia a 1 si encuentra un .
				lexid[0] = ch;
				i = j = 1;
				while ( isdigit( (ch = obtch())) || (es_flotante == 0 and ch == '.')) {
					if (ch == '.') es_flotante = 1;
					if (i < MAXDIGIT) lexid[i++] = ch;
					j++;
				}
				lexid[i] = '\0';
				if (j > MAXDIGIT) {

					error(30); //este n�mero es demasiado grande
				}

				if (es_flotante == 1) {
					token = tok_flotante;
					valor.flotante = atof(lexid);//valor num�rico de una lexeme correspondiene a un flotante
				}
				else {
					token = tok_numero;
					valor.entero = atol(lexid); //valor num�rico de una lexeme correspondiene a un n�mero
				}
				//printf("\n%s", lexid);
			}
			else //reconocimiento de s�mbolos especiales, incluyendo pares de simbolos (aplicamos "lookahead symbol technique")
				if (ch == '<') {
					ch = obtch();
					if (ch == '=') {
						//printf("\n<=");
						token = tok_menorigual;
						ch = obtch();
					}
					else {
						//printf("\n<");
						token = tok_menor;
					}
					/*if (ch=='>') {
						//printf("\n<>");
					   token=tok_null;
					   ch=obtch();
					}
					else{
						//printf("\n<");
					   token=tok_menor;
					}*/
				}
				else if (ch == '>') {
					ch = obtch();
					if (ch == '=') {
						//printf("\n>=");
						token = tok_mayorigual;
						ch = obtch();
					}
					else {
						//printf("\n>");
						token = tok_mayor;
					}
				}
				else if (ch == ':') {
					ch = obtch();
					if (ch == '=') {
						//printf("\n:=");
						token = tok_asignar;
						ch = obtch();
					}
					else {
						//printf("\n%c", ch);
						token = tok_null;
					}
				} else {
					if (ch == '+') {
						ch = obtch();
						if (ch == '=') {
							//printf("\n+=");
							token = tok_sumasign;
							ch = obtch();
						} else {
							//printf("\n%c", ch);
							token = tok_sum;
						}
					} else {
						if (ch == '=') {
							ch = obtch();
							if (ch == '=') {
								//printf("\n==");
								token = tok_igual;
								ch = obtch();
							} else {
								//printf("\n%c", ch);
								token = tok_null;
							}

						} else {
							if (ch == '!') {
								ch = obtch();
								if (ch == '=') {
									//printf("\n!=");
									token = tok_negacion;
									ch = obtch();
								} else {
									//printf("\n%c", ch);
									token = tok_null;
								}
							} else {
								if (ch == '&') {
									ch = obtch();
									if (ch == '&') {
										//printf("\n&&");
										token = tok_and;
										ch = obtch();
									} else {
										//printf("\n%c", ch);
										token = tok_null;
									}
								} else {
									if (ch == '|') {
										ch = obtch();
										if (ch == '|') {
											//printf("\n||");
											token = tok_or;
											ch = obtch();
										} else {
											//printf("\n%c", ch);
											token = tok_null;
										}
									} else {
										//printf("\n%c", ch);
										token = espec[ch]; //hashing directo en la tabla de tokens de s�mbolos especiales del lenguaje
										ch = obtch();
									}
								}
							}
						}
					}
				}
	/* else {
	 	//printf("\n%c",ch);
	    token=espec[ch]; //hashing directo en la tabla de tokens de s�mbolos especiales del lenguaje
	    ch=obtch();
	 }*/
}

//obtoken: obtiene el siguiente token del programa fuente
void obtoken()
{
	do
	{
		obtokenHelper();
		imprime_token();
	} while (token == COMENTARIO);
}

//obtch: obtiene el siguiente caracter del programa fuente
int obtch()
{

	if (fin_de_archivo == 1) {
		//fclose(fp);//cerrar el programa fuente
		printf("\n\nFin del analisis sintactico, Sintaxis correcta.\n");
		//exit(1); //salir...
		return 0;
	}

	if (offset == ll - 1) {
		ll = getline(linea, MAXLINEA); //trae una l�nea del programa fuente al buffer de l�neas
		if (ll == 0)
			fin_de_archivo = 1; //se retrasa en un blanco la deteccion de EOF, porque obtoken lleva un caracter adelantado. si no, en
		//algunos casos tendr�amos problemas, por ejemplo: no se reconoceria el punto final del programa (...end.)

		////printf("\n%s",linea);
		offset = -1;
	}
	++offset;

	if ( (linea[offset] == '\0') || (fin_de_archivo == 1) )
		return (' ');
	else {
		return (tolower(linea[offset])); //de esto depende si el lenguaje es sensitivo de may�sculas o no.
	}
	//return(linea[offset]);

}

//getline: lee la siguiente l�nea del fuente y regresa su tama�o (incluyendo '\n') o 0 si EOF. (por ejemplo: para VAR regresa 4)
//es probablemente la rutina de m�s bajo nivel del compilador
//tomada de "El Lenguaje de programaci�n C" - Kernighan & Ritchie - pag 28
int getline(char s[], int lim)
{
	int c, i;

	for (i = 0; i < lim - 1 && (c = getc(fp)) != EOF && c != '\n'; ++i)
		s[i] = c;

	if (c == '\n') {
		s[i] = c;
		++i;
	}

	s[i] = '\0';
	ln++;
	return (i);
}

int busqueda_binaria_palabras_reservadas(int medio, char *palabra) {
	int comparacion, medio_anterior = 0;
	int salir = 0;
	do {

		comparacion = strcmp(palabra, lexpal[medio]);
		/*//printf("\n\n\n-----------------------------------");
		//printf("\n PALABRA ACTUAL A BUSCAR : %s",palabra);
		//printf("\n MEDIO ANTERIOR: %d",medio_anterior );
		//printf("\nMEDIO ACTUAL: %d",medio);
		//printf("\nPALABRA RESERVADA lexpal[%d]: %s\n",medio, lexpal[medio]);
		//printf("\nRESULTADO DE LA COMPARACION: %d",comparacion);*/

		if (comparacion < 0) {
			medio = medio - 1;
			medio_anterior++;
		}
		if (comparacion > 0) {
			medio = medio + 1;
			medio_anterior++;
			//break;
		}
		if (comparacion == 0) {
			////printf("\nentroooo\n");
			salir = medio;
			return medio;
			//medio= MAXPAL+1;
		}


		//return medio;
		////printf("\n-----------------------------------\n\n");
	} while (medio >= 0 && medio < MAXPAL && medio_anterior < MAXPAL);
	return -1;
}
