#include "lexico.h"

//se define e inicializa la tabla de lexemes correspondientes a las palabras reservadas
char *lexpal[MAXPAL]={ 	"arccos","arcsin","arctan","arr.bool","arr.char","arr.float","arr.int","arr.str","break","cadena","case","concat","console.read",
						"console.write","cos","default","do","else","equal","false","file.fclose","file.fgets","for","func.bool","func.char","func.float",
						"func.int","func.str","func.void","if","length","log","main","pow","replace","round","sin","split","sqrt","substring","switch","tan",
						"trim","true","var.bool","var.char","var.file","var.float","var.fopen","var.fscanf","var.int","var.str","while"};

//el token
enum simbolo token;

//se define e inicializa la tabla de tokens de palabras reservadas

enum simbolo tokpal [MAXPAL]={	tok_arccos,tok_arcsin,tok_arctan,tok_arrBooleano,tok_arrCaracter,tok_arrFlotante,tok_arrEntero,tok_arrCadena,
								tok_break,tok_cadena,tok_case,tok_concat,tok_read,tok_write,tok_cos,tok_default,tok_do,tok_else,tok_equal,
								tok_false,tok_fclose,tok_fgets,tok_for,tok_bool,tok_funcchar,tok_funcfloat,tok_funcint,tok_funcstr,tok_funcvoid,
								tok_if,tok_length,tok_log,tok_main,tok_pow,tok_replace,tok_round,tok_sin,tok_split,tok_sqrt,tok_substring,
								tok_switch,tok_tan,tok_trim,tok_true,tok_varbool,tok_varchar,tok_file,tok_varfloat,tok_fileopen,tok_fscanf,
								tok_varint,tok_varstr,tok_while};

//tabla de tokens correspondientes a operadores y símbolos especiales
enum simbolo espec[255] ;
