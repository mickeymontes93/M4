//funciones auxiliares del compilador
#include <stdio.h>
#include <stdlib.h>
#include "pl0.h"
#include "auxiliares.h"
#include "lexico.h"

//error: por el momento todo error es fatal          
 int error(int no)
{ 
 printf ("\n^ Error %d: Este numero es demasiado grande",no);
 fclose(fp);//cerrar el programa fuente
 exit(1); //estoy en modo de pánico...cualquier error es fatal
}

//inicializar_espec
//construcción de la tabla de tokens para operadores y símboles especiales de pl0
void inicializar_espec()
{
  int i;
  for (i=0;i<=254;++i) 
/*
  for (i=0;i<=254;++i) 
	espec[i]=nulo;

  espec[43]=mas;
  espec[45]=menos;
  espec[42]=por;
  espec[47]=barra;
  espec[40]=parena;
  espec[41]=parenc;
  espec[61]=igl;
  espec[46]=punto;
  espec[44]=coma;
  espec[59]=puntoycoma;

*/    
	espec[i]=tok_null;

  espec[38]=tok_amp;
  espec[91]=tok_corcha;
  espec[92]=tok_corchc;
  espec[123]=tok_llavea;
  espec[125]=tok_llavec;
  espec[43]=tok_sum;
  espec[45]=tok_resta;
  espec[42]=tok_multi;
  espec[47]=tok_divi;
  espec[40]=tok_parena;
  espec[41]=tok_parenc;
  //espec[61]=igl;
//  espec[46]=tok_punto;
  espec[44]=tok_coma;
  espec[59]=tok_finlinea;
}

/*
	char *token_string[]={"nulo","ident","numero","mas","menos","por","barra","oddtok","igl","nig","mnr","mei","myr",
	                   "mai","parena","parenc","coma","puntoycoma","punto","asignacion","begintok","endtok","iftok",
					   "thentok","whiletok","dotok","calltok","consttok","vartok","proctok"};
*/

//imprime_token: transforma de enumerado a string. no aparecerá más en el compilador
void imprime_token()
{

 /*char *token_string[]={"tok_varint","tok_varfloat","tok_varchar","tok_varstr","tok_varbool","tok_if","tok_else","tok_switch","tok_case",
 "tok_default","tok_break","tok_while","tok_do","tok_for","tok_write","tok_read","tok_funcint","tok_funcfloat","tok_funcchar",
 "tok_funcstr","tok_bool","tok_funcvoid","tok_arrEntero","tok_arrFlotante","tok_arrCaracter","tok_arrCadena","tok_arrBooleano",
 "tok_main","tok_file","tok_fileopen","tok_fscanf","tok_fgets","tok_fclose","tok_split","tok_trim","tok_round","tok_sqrt","tok_substring",
 "tok_pow","tok_length","tok_concat","tok_replace","tok_equal","tok_sin","tok_cos","tok_tan","tok_arcsin","tok_arccos","tok_arctan","tok_log"};*/
 char *token_string[]={ "tok_null","tok_id","tok_numero","tok_flotante","tok_caracter","tok_cadena","tok_boolean","tok_amp","tok_llavea","tok_llavec",
 						"tok_parena","tok_parenc","tok_corcha","tok_corchc","tok_finlinea","tok_punto","tok_coma","tok_sum","tok_resta","tok_multi",
 						"tok_divi","tok_asignar","tok_sumasign","tok_mayor","tok_menor","tok_mayorigual","tok_menorigual","tok_igual","tok_negacion",
 						"tok_and","tok_or","tok_varint","tok_varfloat","tok_varchar","tok_varstr","tok_varbool","tok_if","tok_else","tok_switch",
 						"tok_case","tok_default","tok_break","tok_while","tok_do","tok_for","tok_write","tok_read","tok_funcint","tok_funcfloat",
 						"tok_funcchar","tok_funcstr","tok_bool","tok_funcvoid","tok_arrEntero","tok_arrFlotante","tok_arrCaracter","tok_arrCadena",
 						"tok_arrBooleano","tok_main","tok_file","tok_fileopen","tok_fscanf","tok_fgets","tok_fclose","tok_split","tok_trim","tok_round",
 						"tok_sqrt","tok_substring","tok_pow","tok_length","tok_concat","tok_replace","tok_equal","tok_sin","tok_cos","tok_tan","tok_arcsin",
 						"tok_arccos","tok_arctan","tok_log","tok_true","tok_false"};
 
 printf("\t => %10s",token_string[token]);
  
}
