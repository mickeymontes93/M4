#include "lexico.h"

//se define e inicializa la tabla de lexemes correspondientes a las palabras reservadas
//char *lexpal[MAXPAL]={"BEGIN","CALL","CONST","DO","END","IF","ODD","PROCEDURE","THEN","VAR","WHILE"};
char *lexpal[MAXPAL]={ "var.int","var.float","var.char","var.str","var.bool","if","else","switch","case","default","break","while","do","for","console.write",
						"console.read","func.int","func.float","func.char","func.str","func.bool","func.void","arr.int","arr.float","arr.char","arr.str",
						"arr.bool","MAIN","var.file","var.fopen","var.fscanf","file.fgets","file.fclose","split","trim","round","sqrt","substring","pow",
						"length","concat","replace","equal","sin","cos","tan","arcsin","arccos","arctan","log","true","false","cadena"};

//el token
enum simbolo token;

//se define e inicializa la tabla de tokens de palabras reservadas
//enum simbolo tokpal [MAXPAL]={begintok,calltok,consttok,dotok,endtok,iftok,oddtok,proctok,thentok,vartok,whiletok};

enum simbolo tokpal [MAXPAL]={tok_varint,tok_varfloat,tok_varchar,tok_varstr,tok_varbool,tok_if,tok_else,tok_switch,tok_case,tok_default,tok_break,tok_while,tok_do,
						tok_for,tok_write,tok_read,tok_funcint,tok_funcfloat,tok_funcchar,tok_funcstr,tok_bool,tok_funcvoid,tok_arrEntero,tok_arrFlotante,
						tok_arrCaracter,tok_arrCadena,tok_arrBooleano,tok_main,tok_file,tok_fileopen,tok_fscanf,tok_fgets,tok_fclose,tok_split,tok_trim,tok_round,
						tok_sqrt,tok_substring,tok_pow,tok_length,tok_concat,tok_replace,tok_equal,tok_sin,tok_cos,tok_tan,tok_arcsin,tok_arccos,tok_arctan,tok_log,
						tok_true,tok_false,tok_cadena};

//tabla de tokens correspondientes a operadores y símbolos especiales
enum simbolo espec[255] ;
