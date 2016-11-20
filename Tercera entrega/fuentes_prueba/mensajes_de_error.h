//mensajes de error para el compilador de pl0
//parser1 no está diseñado para mostrar todos los errores de la lista
//ésta es la lista de errores del compilador completo


char *mensaje_de_error[]={"",
/*Error 1*/ "Se esperaba parentesis de apertura",
/*Error 2*/ "Se esperaba parentesis de cierre",
/*Error 3*/ "Se esperaba una coma",
/*Error 4*/ "Se esperaba una expresion numerica o una variable",
/*Error 5*/ "Se esperaba un identificador",
/*Error 6*/ "Se esperaba una cadena",
/*Error 7*/ "Se esperaba un valor booleano",
/*Error 8*/ "Se esperaba una condicion",
/*Error 9*/ "Se esperaba un operador logico || o &&",
/*Error 10*/ "Se esperaba un comparador !=, ==, >, >=, < o <=",
/*Error 11*/ "Se esperaba un comparador == o !=",
/*Error 12*/ "Se esperaba un comparador numerico >, >=, < o <=",
/*Error 13*/ "Se esperaba un especificador de formato %d, %c, %f o %s",
/*Error 14*/ "Se esperaba un tipo de dato de retorno para la funcion",
/*Error 15*/ "Se esperaba un tipo de dato para la variable",
/*Error 16*/ "Se esperaba punto y coma",
/*Error 17*/ "Se esperaba funcion main",
/*Error 18*/ "Se esperaba llave de apertura",
/*Error 19*/ "Se esperaba llave de cierre",
/*Error 20*/ "Se esperaba una expresion numerica, booleana, cadena o arreglo",
/*Error 21*/ "Se esperaba una asignacion con :=",
/*Error 22*/ "Se esperaba un arreglo",
/*Error 23*/ "Se esperaba la instruccion substring",
/*Error 24*/ "Se esperaba la instruccion para abrir un archivo file.fopen",
/*Error 25*/ "Se esperaba la instruccion para cerrar un archivo file.fclose",
/*Error 26*/ "Se esperaba corchete de apertura",
/*Error 27*/ "Se esperaba corchete de cierre",
/*Error 28*/ "Se esperaba un flotante",
/*Error 29*/ "Se esperaba un entero.",
/*Error 30*/ "Se esperaba una cadena",
/*Error 31*/ "Se esperaba un caracter",
/*Error 32*/ "Se esperaba un valor booleano",
/*Error 33*/ "Se esperaba un break",
/*Error 34*/ "Se esperaba una instruccion case",
/*Error 35*/ "Se esperaba una instruccion default",
/*Error 36*/ "Se esperaba un expresion que genere un valor booleano",
/*Error 37*/ "Se esperaba una cadena, un caracter o un identificador.",
/*Error 38*/ "Se esperaba una numero entero, numero flotante o un identificador",
/*Error 39*/ "Se esperaba una instruccion concat",
/*Error 40*/ "Se esperaba una instruccion replace",
/*Error 41*/ "Se esperaba una instruccion console.read",
/*Error 42*/ "Se esperaba una instruccion console.write",
/*Error 43*/ "Se esperaba una instruccion do",
/*Error 44*/ "Se esperaba una instruccion while",
/*Error 45*/ "Se esperaba una instruccion for",
/*Error 46*/ "Se esperaba un if",
/*Error 47*/ "Se esperaba una instruccion switch",
/*Error 48*/ "Se esperaba una intruccion if, while, for, write, read, switch, do o fileopen",
/*Error 49*/ "Identificador no declarado",
/*Error 50*/ "Se esperaba un ampersand",
/*Error 51*/ "Simbolo incorrecto detras de las instrucciones de un bloque",
/*Error 52*/ "un simbolo incorrecto sigue a una instruccion",
/*Error 53*/ "un simbolo incorrecto sigue a un swtich-case",
/*Error 54*/ "El simbolo no corresponde a un conjunto de variables",
/*Error 55*/ "Se esperaba un simbolo de operacion numerica",
/*Error 56*/ "El simbolo no corresponde a un termino valido",
};

/*Errores sintacticos : 1,2,3,4,5,6,7,8,9,10,13,16,17,18,19,20,22,23,24,32 */

/*Errores semanticos  : 11,12,14,15,35*/

/*Limitaciones especificas del compilador : 30,31,33,34*/



    
