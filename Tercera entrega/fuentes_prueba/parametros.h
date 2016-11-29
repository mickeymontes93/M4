//parámetros del compilador.
//también se podrían leer de un archivo de configuración

#define MAXPAL     	55  //numero de palabras reservadas              
#define MAXSTRING 	200 //máxima longitud a guardar de strings
#define NOTOKENS   	85  //número de tokens en el lenguaje
#define MAXIT     	100  //máximo tamaño de la tabla de símbolos 

extern int MAXLINEA;  //tamaño máximo de una línea del programa fuente
extern int MAXDIGIT;  //máximo número de dígitos en los enteros
extern int MAXID;  //máxima longitud de los identificadores


/*PARAMETROS DE CODIGO P*/
#define MAXIC     200  //tamaño del array de código: instrucciones de codigo-p         
#define MAXD    32767  //máximo entero (16bits)           
#define MAXNIV      1  //máxima profundidad de anidamiento de bloques (NO HAY ANIDAMIENTO)    
#define TAMANO_MAX_NOMBRE_DEL_ARCHIVO_CODIGO_P 20 //tamaño máximo del nombre del archivo objeto que se creará(extensión .p)
