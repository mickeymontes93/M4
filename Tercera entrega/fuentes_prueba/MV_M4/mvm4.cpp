/*ESTE ES LA MAQUINA VIRTUAL QUE SIRVE, SOLO FALTA IMPLEMENTAR INTERPRETAR*/
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
//#include <conio.h>


#define MAXSTRING 300 
#define MAXID     100  /*Maxima longitud de los identificadores        */
#define MAXIC     1000  /*Tamaño del array codigo: instrucciones codigo-p*/
#define LONGSTACK 500  /*Longitud del stack de datos                    */
 

/*    Instrucciones del ensamblador (codigo-p)    */
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC,RET,AA};
//Tipos de datos utilizados en M4
enum tipo_dato {NO_TIPO, ENTERO, FLOAT, CADENA, CARACTER, BOOLEAN}; 
//arrays para mostrar como string el mnemónico de la instrucción
char *mnemonico[]={"LIT","OPR","CAR","ALM","LLA","INS","SAL","SAC","AA"};

//comentarios para una instrucción de codigo-p
char *comentario[]={";cargar una constante",";operacion aritmetica, relacional o retornar",";cargar una variable",
                 ";almacenamiento/instruccion de asignacion",";llamada a procedimiento",";asignacion de espacio de memoria",
                 ";salto incondicional",";salto condicional","borraaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaar"};
                     
typedef struct {
 enum fcn f; // mnemónico
 int      n; //nivel
 tipo_dato t;
 union{
	int entero;
	float flotante;
	int booleano;
	char caracter;
	char cadena[MAXSTRING];
 }Dato;
} codigo_intermedio;

codigo_intermedio codigo[MAXIC]; //array con las instrucciones de codigo-p
int ic;                          //índice sobre el array de código-p

//---- CREACION DE STACK 
typedef struct {
 tipo_dato tipo;
 union{
	int entero;
	float flotante;
	int booleano;
	char caracter;
	char cadena[MAXSTRING];
 }Dato;
} pila;

pila p[MAXIC];
int ip; 

FILE *obj; //apuntador al archivo de código intermedio

//----> DECLARACION DE FUNCIONES.
void listar_p(void);
void interpretar(); 
int base(int ni,int b);
void leer(char *);

int main(int argc,char *argv[]) {
	 //if (argc!=2) //analizar argumentos de main
	//	printf("\nuso: pl0mv nombre_de_archivo");
	 //else  
	//	if ( (obj=fopen(argv[1],"r+b"))==NULL ) {
	    if ( (obj=fopen("ejemplo1.ob","r+b"))==NULL ) {
		   printf("\nerror al abrir archivo %s",argv[1]);
		}else {
		   //leer el código desde el archivo hacia un array en memoria
		   ic=0;
		  // printf("\n IC----->%d",ic);
		   fread(&codigo[ic],sizeof(codigo_intermedio),1,obj);
		   /*printf("\tF----->%d",codigo[ic].f);
		   printf("\tN----->%d",codigo[ic].n);
		   printf("\tD----->%d",codigo[ic].d);
		   printf("\n");
		   printf("\tDato entero ----->%d",codigo[ic].Dato.entero);
		   printf("\tDato float ----->%f",codigo[ic].Dato.flotante);
		   printf("\tDato char ----->%c",codigo[ic].Dato.caracter);
		   printf("\tDato cad ----->%s",codigo[ic].Dato.cadena);
		   printf("\n");*/
		   while ( !feof(obj) ) {
				 ++ic;
				 fread(&codigo[ic],sizeof(codigo_intermedio),1,obj);
				 /* printf("\n IC----->%d",ic);
				  printf("\tF----->%d",codigo[ic].t);
				  printf("\tN----->%d",codigo[ic].n);
				  printf("\tD----->%d",codigo[ic].d);
			      printf("\n");
			      printf("\tDato entero ----->%d",codigo[ic].Dato.entero);
			      printf("\tDato float ----->%f",codigo[ic].Dato.flotante);
			      printf("\tDato char ----->%c",codigo[ic].Dato.caracter);
			      printf("\tDato cad ----->%s",codigo[ic].Dato.cadena);
			      printf("\n");*/
				 
		   }
		   //cierra el archivo
		   fclose(obj);

		   listar_p(); //quita el comentario de esta línea si deseas verificar que el código se cargo al array correctamente
		

		   //interpreta el codigo-p 
		  // interpretar();   	
		}
		printf("\n\nFinalizo la ejecucion\n");
		//system("PAUSE");
		return(0);
 }


//base: encuentra la base ni niveles más abajo en el stack para la operación actual (CAR,ALM,LLA)
int base(int ni,int b){
 int b1;
 b1=b;
 while (ni>0) {
       b1=p[b1].Dato.entero;
       --ni;
 }
 return (b1);
}

//FUNCION QUE SE ENCARGA DE LISTAR TODOS LAS LINEAS DE CODIGO INTERMEDIO DENTRO DEL ARCHIVO
void listar_p(){
 int i;
   
 printf("\n\n --Listado de codigo-p simplificado generado por el compilador--\n\n");
 printf("\n---ic: %d---\n",ic);

 //printf("\n---mnemonico: %f---\n",sizeof(codigo[i]));

 for(i=0;i<ic;++i) {
     printf("\ni==== %d",i);
     printf("\tcodigo[i].f====%d  (%s) ",codigo[i].f, mnemonico[codigo[i].f]);
     printf("\ncodigo[i].n====%d",codigo[i].n);
     printf("\tcodigo[i].d====%d",codigo[i].d);
     printf("\n %4d  %3s %5d %5d %s",i,mnemonico[codigo[i].f],codigo[i].n,codigo[i].d,comentario[codigo[i].f]);
 }
}
