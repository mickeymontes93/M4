//par�metros del compilador.
//tambi�n se podr�an leer de un archivo de configuraci�n

#define MAXPAL     	55  //numero de palabras reservadas              
#define MAXSTRING 	200 //m�xima longitud a guardar de strings
#define NOTOKENS   	85  //n�mero de tokens en el lenguaje
#define MAXIT     	100  //m�ximo tama�o de la tabla de s�mbolos 

extern int MAXLINEA;  //tama�o m�ximo de una l�nea del programa fuente
extern int MAXDIGIT;  //m�ximo n�mero de d�gitos en los enteros
extern int MAXID;  //m�xima longitud de los identificadores


/*PARAMETROS DE CODIGO P*/
#define MAXIC     200  //tama�o del array de c�digo: instrucciones de codigo-p         
#define MAXD    32767  //m�ximo entero (16bits)           
#define MAXNIV      1  //m�xima profundidad de anidamiento de bloques (NO HAY ANIDAMIENTO)    
#define TAMANO_MAX_NOMBRE_DEL_ARCHIVO_CODIGO_P 20 //tama�o m�ximo del nombre del archivo objeto que se crear�(extensi�n .p)
