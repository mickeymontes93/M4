#include "parametros.h"

//lexemes de las palabras reservadas
extern char *lexpal[MAXPAL];

//lista de tokens de pl0
//enum simbolo {nulo,ident,numero,mas,menos,por,barra,oddtok,igl,nig,mnr,mei,myr,mai,parena,parenc,coma,puntoycoma,
//	          punto,asignacion,begintok,endtok,iftok,thentok,whiletok,dotok,calltok,consttok,vartok,proctok}; //definido aquí en el encabezado

enum simbolo {tok_null, tok_id, tok_numero, tok_flotante, tok_caracter, tok_cadena, tok_boolean, tok_amp, tok_llavea, tok_llavec, tok_parena,
              tok_parenc, tok_corcha, tok_corchc, tok_finlinea, tok_punto, tok_coma, tok_sum, tok_resta, tok_multi, tok_divi, tok_asignar, tok_sumasign,
              tok_mayor, tok_menor, tok_mayorigual, tok_menorigual, tok_igual, tok_negacion, tok_and, tok_or, tok_varint, tok_varfloat, tok_varchar,
              tok_varstr, tok_varbool, tok_if, tok_else, tok_switch, tok_case, tok_default, tok_break, tok_while, tok_do, tok_for, tok_write, tok_read, tok_funcint,
              tok_funcfloat, tok_funcchar, tok_funcstr, tok_bool, tok_funcvoid, tok_arrEntero, tok_arrFlotante, tok_arrCaracter, tok_arrCadena, tok_arrBooleano, tok_main,
              tok_file, tok_fileopen, tok_fscanf, tok_fgets, tok_fclose, tok_split, tok_trim, tok_round, tok_sqrt, tok_substring, tok_pow, tok_length, tok_concat,
              tok_replace, tok_equal, tok_sin, tok_cos, tok_tan, tok_arcsin, tok_arccos, tok_arctan, tok_log, tok_true, tok_false, COMENTARIO, tok_return
             };

extern enum simbolo token;

//tabla de tokens de palabras reservadas
extern enum simbolo tokpal [MAXPAL];

//tabla de tokens correspondientes a operadores y símbolos especiales
extern enum simbolo espec[255];

