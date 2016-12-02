//máquina virtual (intérprete-ejecutor) de código-p simplificado
//se usa una máquina de stack para realizar la ejecución del código
//la implementación del stack está desguarnecida: no se ha implementado 
//ningún nivel de abstracción ni de validación
//noviembre 2012,abril de 2011
//se usó Microsoft Visual C++ 2010 para escribir esta mv

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "parametros.h"
#define MAXIC     200  //tamaño máximo del array de código-p
#define LONGSTACK 700  //tamaño-longitud del stack de datos                    

//instrucciones(mnemónicos) del código-p
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC};
enum tipo_dato {NO_TIPO, ENTERO, FLOAT, CADENA, CARACTER, BOOLEAN}; 

typedef struct {
     enum fcn f; //mnemónico de la instrucción a utilizar
     int     ni; //nivel (0..MAXNIV), donde 0 es global y 1 es local
     int     di; //dirección o desplazamiento (0..32767)
                  /*EN LIT d es el número que se va a ingresar, este dato da igual */
     enum tipo_dato ti; // indica el tipo de dato con el que la instruccion va a trabajar:
                  /*
                      0----> no es un tipo de dato
                      1----> numero entero
                      2----> numero flotante
                      3----> Cadena
                      4----> Caracter
                      5----> Boolean
                  */
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

FILE *obj; //apuntador al archivo de código intermedio


//int p[LONGSTACK] ; //memoria de datos-stack
struct pila {
  enum tipo_dato ti;
  union{
    int entero;
    float flotante;
    int booleano;
    char caracter;
    char cadena[MAXSTRING]; 
  }Dato;
};

//arrays para mostrar como string el mnemónico de la instrucción
char *mnemonico[]={"LIT","OPR","CAR","ALM","LLA","INS","SAL","SAC"};

//comentarios para una instrucción de codigo-p
 char *comentario[]={";cargar una constante",";operacion aritmetica, relacional o retornar",";cargar una variable",
                     ";almacenamiento/instruccion de asignacion",";llamada a procedimiento",";asignacion de espacio de memoria",
                     ";salto incondicional",";salto condicional"};

//prototipos de funciones
void interpretar(void),listar_p(void);
int base(int ni,int b);
struct pila p[LONGSTACK]; 

//main: inicia la ejecución
int main(int argc,char *argv[]) {
 if (argc!=2) //analizar argumentos de main
    printf("\nuso: pl0mv nombre_de_archivo");
 else  
    if ( (obj=fopen(argv[1],"r+b"))==NULL ) 
       printf("\nerror al abrir archivo %s",argv[1]);
	else {
       //leer el código desde el archivo hacia un array en memoria
       ic=0;
       fread(&codigo[ic],sizeof(codigo_intermedio),1,obj);
	printf("\n----tam: %d  -  linea: %s ----\n",sizeof(codigo_intermedio),codigo[ic]);
       while ( !feof(obj) ) {
             ++ic;
             fread(&codigo[ic],sizeof(codigo_intermedio),1,obj);
       }
	   //cierra el archivo
       fclose(obj);

       listar_p(); //quita el comentario de esta línea si deseas verificar que el código se cargo al array correctamente

	   //interpreta el codigo-p 
	   interpretar();   	
    }
   return(0);
 }

//interpretar: interpreta código-p                
void interpretar(void) {

 register int d;       //registro de dirección del programa: apunta a la siguiente instrucción por ejecutar
 register int b;       //registro de dirección base
 register int s;       //apuntador al tope del stack
 codigo_intermedio i;  //registro de instrucción: contiene la siguiente instrucción a ejecutar		     
float op1,op2; //auxiliares
 
 s=-1;b=0;d=0;
 p[0].Dato.entero=p[1].Dato.entero=p[2].Dato.entero=0;   //ED, EE y DR para el programa principal
 p[0].ti=p[1].ti=p[2].ti=ENTERO;

 //bucle de ejecución
 do {
	//printf("\n %4d  %3s %5d %5d %s",i,mnemonico[codigo[i].f],codigo[i].ni,codigo[i].di,comentario[codigo[i].f])
	// 0(i)   mnemonico(codigo[i].f)   nivel(codigo[i].ni)   direccion(codigo[i].di)   comentario
  
    i=codigo[d++];
	//i=codigo[p[s+1].Dato.entero]   
    //printf("\n\nejecutando la instruccion %4d: %3s %5d %5d",d-1,mnemonico[i.f],i.ni,i.di); 
    printf("\n\nejecutando la instruccion %4d: %3s %5d %5d",d-1,mnemonico[i.f],i.ni,i.Dato.entero); 

    switch(i.f) {
          case LIT: // LIT nivel X tipo_dato
               //p[++s]=i.di;
		printf("\n\n--ENTRO AL LIT--\n\n");
                switch(i.ti){
                  case ENTERO:
                      ++s;
                      p[s].Dato.entero= i.Dato.entero;
                      p[s].ti=ENTERO;
                      //printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %d en direccion %d ",s,i.ni,i.di,i.ti,i.Dato.entero,s);
                      printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %d en direccion %d ",s,i.ni,i.Dato.entero,i.ti,i.Dato.entero,s);
                      break;
                  case FLOAT:
                      ++s;
                      p[s].Dato.flotante= i.Dato.flotante;
                      p[s].ti=FLOAT;
                      //printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %f en direccion %d ",s,i.ni,i.di,i.ti,i.Dato.flotante,s);
			printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %f en direccion %d ",s,i.ni,i.Dato.entero,i.ti,i.Dato.flotante,s);			
                      break;
                  case CARACTER:
                      ++s;
                      p[s].Dato.caracter= i.Dato.caracter;
                      p[s].ti=CARACTER;
                      //printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %c en direccion %d ",s,i.ni,i.di,i.ti,i.Dato.caracter,s);
                      printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %c en direccion %d ",s,i.ni,i.Dato.entero,i.ti,i.Dato.caracter,s);
                      break;
                  case CADENA:
                      ++s;
                      strcpy(p[s].Dato.cadena,i.Dato.cadena);
                      p[s].ti=CADENA;
                      //printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %s en direccion %d ",s,i.ni,i.di,i.ti,i.Dato.cadena,s);
                      printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %s en direccion %d ",s,i.ni,i.Dato.entero,i.ti,i.Dato.cadena,s);
                      break;
                  case BOOLEAN:
                      ++s;
                      p[s].Dato.booleano= i.Dato.booleano;
                      p[s].ti=BOOLEAN;
                      //printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %d en direccion %d ",s,i.ni,i.di,i.ti,i.Dato.booleano,s);
                      printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %d en direccion %d ",s,i.ni,i.Dato.entero,i.ti,i.Dato.booleano,s);
                      break;
                }

               break;

          case OPR: // OPR nivel tipo_opr tipo_dato
               //printf("\nOPR : ");
			   //determinar de que operador se trata
               switch(i.di) {  
			       case 0: //retornar o fin
                        s=--b;
                        d=p[s+3].Dato.entero;
			b=p[s+2].Dato.entero;
	                      //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] Retornar a la instruccion %d, base=%d ",s,i.ni,i.di,i.ti,d,b);
				printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] Retornar a la instruccion %d, base=%d ",s,i.ni,i.Dato.entero,i.ti,p[s+3].Dato.entero,b);
                        break;

                   case 1: //----------> MENOS UNARIO
                        switch(i.ti){
                          case ENTERO:
                            //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] Operador ( - )  unario para %d",s,i.ni,i.di,i.ti,p[s].Dato.entero);
                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] Operador ( - )  unario para %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero);
                            p[s].Dato.entero=-p[s].Dato.entero;
				p[s].ti = ENTERO;
                            break;
                          case FLOAT:
                            //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] Operador ( - )  unario para %f",s,i.ni,i.di,i.ti,p[s].Dato.flotante);
                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] Operador ( - )  unario para %f",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante);
                            p[s].Dato.flotante=-p[s].Dato.flotante;
				p[s].ti = FLOAT;
                            break;
                          default:
                            fclose(obj);
                            printf("\nError a tiempo de ejecucion\nError en tipos de datos");
                            printf("\n--Programa abortado--");
                            exit(1); //el error es fatal
                        }
                        break;

                   case 2: //----------> SUMA
                        --s;
                        if(p[s].ti==ENTERO && p[s+1].ti==ENTERO){
                          //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma de %d + %d",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma de %d + %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                          p[s].Dato.entero=p[s].Dato.entero+p[s+1].Dato.entero;
                          p[s].ti=ENTERO;
                        }else if(p[s].ti==ENTERO && p[s+1].ti==FLOAT){
                          //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma de %d + %f",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.flotante);
                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma de %d + %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                          p[s].Dato.flotante=p[s].Dato.entero+p[s+1].Dato.flotante;
                          p[s].ti=FLOAT;
                        }else if(p[s].ti==FLOAT && p[s+1].ti==ENTERO){
                          //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma de %f + %d",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.entero);
                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma de %d + %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                          p[s].Dato.flotante=p[s].Dato.flotante+p[s+1].Dato.entero;
                          p[s].ti=FLOAT;
                        }else if(p[s].ti==FLOAT && p[s+1].ti==FLOAT){
                          //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma de %f + %f",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.flotante);
                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma de %d + %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                          p[s].Dato.flotante=p[s].Dato.flotante+p[s+1].Dato.flotante;
                          p[s].ti=FLOAT;
                        }else{
                            fclose(obj);
                            printf("\nError a tiempo de ejecucion\nError en tipos de datos");
                            printf("\n--Programa abortado--");
                            exit(1); //el error es fatal
                        }  
	                      break;

                   case 3://----------> RESTA
	                      --s; 
                        //printf("resta de %d - %d (s en %d)",p[s],p[s+1],s);
                        //p[s]=p[s]-p[s+1];
                        if(p[s].ti==ENTERO && p[s+1].ti==ENTERO){
                          //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] resta de %d - %d",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] resta de %d - %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                          p[s].Dato.entero=p[s].Dato.entero-p[s+1].Dato.entero;
                          p[s].ti=ENTERO;
                        }else if(p[s].ti==ENTERO && p[s+1].ti==FLOAT){
                          //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] resta de %d - %f",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.flotante);
                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] resta de %d - %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                          p[s].Dato.flotante=p[s].Dato.entero-p[s+1].Dato.flotante;
                          p[s].ti=FLOAT;
                        }else if(p[s].ti==FLOAT && p[s+1].ti==ENTERO){
                          //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] resta de %f - %d",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.entero);
                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] resta de %d - %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                          p[s].Dato.flotante=p[s].Dato.flotante-p[s+1].Dato.entero;
                          p[s].ti=FLOAT;
                        }else if(p[s].ti==FLOAT && p[s+1].ti==FLOAT){
                          //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] resta de %f - %f",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.flotante);
                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] resta de %d - %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                          p[s].Dato.flotante=p[s].Dato.flotante-p[s+1].Dato.flotante;
                          p[s].ti=FLOAT;
                        }else{
                            fclose(obj);
                            printf("\nError a tiempo de ejecucion\nError en tipos de datos");
                            printf("\n--Programa abortado--");
                            exit(1); //el error es fatal
                        }
                        break;

                   case 4: //----------> MULTIPLICACION
	                      --s;
                       // printf("multiplicacion de %d * %d (s en %d)",p[s],p[s+1],s);
                       // p[s]=p[s]*p[s+1];
                        if(p[s].ti==ENTERO && p[s+1].ti==ENTERO){
                            //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] multiplicacion de %d * %d",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] multiplicacion de %d * %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                            p[s].Dato.entero=p[s].Dato.entero*p[s+1].Dato.entero;
                            p[s].ti=ENTERO;
                        }else if(p[s].ti==ENTERO && p[s+1].ti==FLOAT){
                            //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] multiplicacion de %d * %f",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.flotante);
                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] multiplicacion de %d * %f",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.flotante);
                            p[s].Dato.flotante=p[s].Dato.entero*p[s+1].Dato.flotante;
                            p[s].ti=FLOAT;
                        }else if(p[s].ti==FLOAT && p[s+1].ti==ENTERO){
                            //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] multiplicacion de %f * %d",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.entero);
                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] multiplicacion de %f * %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.entero);
                            p[s].Dato.flotante=p[s].Dato.flotante*p[s+1].Dato.entero;
                            p[s].ti=FLOAT;
                        }else if(p[s].ti==FLOAT && p[s+1].ti==FLOAT){
                            //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] multiplicacion de %f * %f",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.flotante);
                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] multiplicacion de %f * %f",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.flotante);
                            p[s].Dato.flotante=p[s].Dato.flotante*p[s+1].Dato.flotante;
                            p[s].ti=FLOAT;
                        }else{
                            fclose(obj);
                            printf("\nError a tiempo de ejecucion\nError en tipos de datos");
                            printf("\n--Programa abortado--");
                            exit(1); //el error es fatal
                        }
                        break;

                   case 5: //---------->  DIVISION
                        --s;
	                     //printf("division entera de %d / %d (s en %d)",p[s],p[s+1],s);
                        if((p[s+1].ti==ENTERO && p[s+1].Dato.entero==0) or (p[s+1].ti==FLOAT && p[s+1].Dato.flotante==0)) {
    						            fclose(obj);
    	                      printf("\nError a tiempo de ejecucion\nSe intenta dividir entre cero");
    	                      printf("\n--Programa abortado--");
    	                      exit(1); //el error es fatal
                        }
                        //p[s]=p[s]/p[s+1];
                        if(p[s].ti==ENTERO && p[s+1].ti==ENTERO){
                            //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] division de %d / %d",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
				printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] division de %d / %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                            p[s].Dato.flotante=p[s].Dato.entero/p[s+1].Dato.entero;
                            p[s].ti=FLOAT;
                        }else if(p[s].ti==ENTERO && p[s+1].ti==FLOAT){
                            //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] division de %d / %f",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.flotante);
                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] division de %d / %f",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.flotante);
                            p[s].Dato.flotante=p[s].Dato.entero/p[s+1].Dato.flotante;
                            p[s].ti=FLOAT;
                        }else if(p[s].ti==FLOAT && p[s+1].ti==ENTERO){
                            //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] division de %f / %d",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.entero);
                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] division de %f / %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.entero);
                            p[s].Dato.flotante=p[s].Dato.flotante/p[s+1].Dato.entero;
                            p[s].ti=FLOAT;
                        }else if(p[s].ti==FLOAT && p[s+1].ti==FLOAT){
                            //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] division de %f / %f",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.flotante);
                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] division de %f / %f",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.flotante);
                            p[s].Dato.flotante=p[s].Dato.flotante/p[s+1].Dato.flotante;
                            p[s].ti=FLOAT;
                        }else{
                            fclose(obj);
                            printf("\nError a tiempo de ejecucion\nError en tipos de datos");
                            printf("\n--Programa abortado--");
                            exit(1); //el error es fatal
                        }
                        break;

                   case 6: //----------> ODD
                        switch(p[s].ti){
                            case ENTERO:
                               //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] odd(%d)?",s,i.ni,i.di,i.ti,p[s].Dato.entero);
                               printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] odd(%d)?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero);
                               p[s].Dato.entero=(((p[s].Dato.entero)%2)!=0);
                               p[s].ti=ENTERO;
                               break;
                            /*case FLOAT:
                               printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] odd(%f)?",s,i.ni,i.di,i.ti,p[s].Dato.flotante);
                               p[s].Dato.entero=(((p[s].Dato.flotante)%2)!=0);
                               p[s].ti=ENTERO;
                               break;*/
                        }
                        //printf("odd(%d)? (s en %d)",p[s],s);
                        //p[s]=(p[s]%2!=0);
                        break;
       
                   case 7: //sin uso
                        break;

                   case 8: //----------> IGUAL (==)
	                      --s;
                        switch(p[s].ti){
                            case ENTERO:
                                switch(p[s+1].ti){
                                    case ENTERO:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%d?",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                                        p[s].Dato.entero=(p[s].Dato.entero==p[s+1].Dato.entero);
                                        p[s].ti=ENTERO;
                                        break;
                                    case BOOLEAN:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%d?",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.booleano);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CARACTER:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%c?",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.caracter);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%c?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CADENA:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%s?",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.cadena);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%s?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case FLOAT:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%f?",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.flotante);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%f?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=(p[s].Dato.entero==p[s+1].Dato.flotante);
                                        p[s].ti=ENTERO;
                                        break;
                                }

                                break;
                            case BOOLEAN:
                                switch(p[s+1].ti){
                                    case ENTERO:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%d?",s,i.ni,i.di,i.ti,p[s].Dato.booleano,p[s+1].Dato.entero);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.booleano,p[s+1].Dato.entero);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case BOOLEAN:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%d?",s,i.ni,i.di,i.ti,p[s].Dato.booleano,p[s+1].Dato.booleano);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.booleano,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=(p[s].Dato.booleano==p[s+1].Dato.booleano);
                                        p[s].ti=ENTERO;
                                        break;
                                    case CARACTER:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%c?",s,i.ni,i.di,i.ti,p[s].Dato.booleano,p[s+1].Dato.caracter);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%c?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.booleano,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CADENA:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%s?",s,i.ni,i.di,i.ti,p[s].Dato.booleano,p[s+1].Dato.cadena);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%s?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.booleano,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case FLOAT:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%f?",s,i.ni,i.di,i.ti,p[s].Dato.booleano,p[s+1].Dato.flotante);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%f?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.booleano,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                }

                                break;
                            case CARACTER:
                                switch(p[s+1].ti){
                                    case ENTERO:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%d?",s,i.ni,i.di,i.ti,p[s].Dato.caracter,p[s+1].Dato.entero);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.caracter,p[s+1].Dato.entero);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case BOOLEAN:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%d?",s,i.ni,i.di,i.ti,p[s].Dato.caracter,p[s+1].Dato.booleano);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.caracter,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CARACTER:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%c?",s,i.ni,i.di,i.ti,p[s].Dato.caracter,p[s+1].Dato.caracter);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%c?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.caracter,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=(p[s].Dato.caracter==p[s+1].Dato.caracter);
                                        p[s].ti=ENTERO;
                                        break;
                                    case CADENA:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%s?",s,i.ni,i.di,i.ti,p[s].Dato.caracter,p[s+1].Dato.cadena);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%s?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.caracter,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case FLOAT:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%f?",s,i.ni,i.di,i.ti,p[s].Dato.caracter,p[s+1].Dato.flotante);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%f?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.caracter,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                }

                                break;

                            case CADENA:
                                switch(p[s+1].ti){
                                    case ENTERO:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%d?",s,i.ni,i.di,i.ti,p[s].Dato.cadena,p[s+1].Dato.entero);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.cadena,p[s+1].Dato.entero);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case BOOLEAN:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%d?",s,i.ni,i.di,i.ti,p[s].Dato.cadena,p[s+1].Dato.booleano);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.cadena,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CARACTER:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%c?",s,i.ni,i.di,i.ti,p[s].Dato.cadena,p[s+1].Dato.caracter);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%c?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.cadena,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CADENA:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%s?",s,i.ni,i.di,i.ti,p[s].Dato.cadena,p[s+1].Dato.cadena);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%s?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.cadena,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=(p[s].Dato.cadena==p[s+1].Dato.cadena);
                                        p[s].ti=ENTERO;
                                        break;
                                    case FLOAT:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%f?",s,i.ni,i.di,i.ti,p[s].Dato.cadena,p[s+1].Dato.flotante);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%f?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.cadena,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                }

                                break;


                            case FLOAT:
                                switch(p[s+1].ti){
                                    case ENTERO:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%d?",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.entero);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.entero);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case BOOLEAN:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%d?",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.booleano);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CARACTER:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%c?",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.caracter);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%c?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CADENA:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%s?",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.cadena);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%s?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case FLOAT:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%f?",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.flotante);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%f?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=(p[s].Dato.flotante==p[s+1].Dato.flotante);
                                        p[s].ti=ENTERO;
                                        break;
                                }

                                break;
                        }
                        //printf("%d=%d? (s en %d)",p[s],p[s+1],s);
                        //p[s]=(p[s]==p[s+1]);
                        break;

                   case 9: //----------> DIFERENTE DE (!=)
                        --s;
                       /* printf("%d!=%d? (s en %d)",p[s],p[s+1],s);
                        p[s]=(p[s]!=p[s+1]);
                        break;*/
                        switch(p[s].ti){
                            case ENTERO:
                                switch(p[s+1].ti){
                                    case ENTERO:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%d?",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                                        p[s].Dato.entero=(p[s].Dato.entero!=p[s+1].Dato.entero);
                                        p[s].ti=ENTERO;
                                        break;
                                    case BOOLEAN:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%d?",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.booleano);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CARACTER:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%c?",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.caracter);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%c?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CADENA:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%s?",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.cadena);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%s?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                    case FLOAT:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%f?",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.flotante);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%f?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=(p[s].Dato.entero!=p[s+1].Dato.flotante);
                                        p[s].ti=ENTERO;
                                        break;
                                }

                                break;
                            case BOOLEAN:
                                switch(p[s+1].ti){
                                    case ENTERO:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%d?",s,i.ni,i.di,i.ti,p[s].Dato.booleano,p[s+1].Dato.entero);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.booleano,p[s+1].Dato.entero);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                    case BOOLEAN:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%d?",s,i.ni,i.di,i.ti,p[s].Dato.booleano,p[s+1].Dato.booleano);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.booleano,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=(p[s].Dato.booleano!=p[s+1].Dato.booleano);
                                        p[s].ti=ENTERO;
                                        break;
                                    case CARACTER:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%c?",s,i.ni,i.di,i.ti,p[s].Dato.booleano,p[s+1].Dato.caracter);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%c?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.booleano,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CADENA:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%s?",s,i.ni,i.di,i.ti,p[s].Dato.booleano,p[s+1].Dato.cadena);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%s?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.booleano,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                    case FLOAT:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%f?",s,i.ni,i.di,i.ti,p[s].Dato.booleano,p[s+1].Dato.flotante);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%f?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.booleano,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                }

                                break;
                            case CARACTER:
                                switch(p[s+1].ti){
                                    case ENTERO:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%d?",s,i.ni,i.di,i.ti,p[s].Dato.caracter,p[s+1].Dato.entero);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.caracter,p[s+1].Dato.entero);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                    case BOOLEAN:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%d?",s,i.ni,i.di,i.ti,p[s].Dato.caracter,p[s+1].Dato.booleano);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.caracter,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CARACTER:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%c?",s,i.ni,i.di,i.ti,p[s].Dato.caracter,p[s+1].Dato.caracter);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%c?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.caracter,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=(p[s].Dato.caracter!=p[s+1].Dato.caracter);
                                        p[s].ti=ENTERO;
                                        break;
                                    case CADENA:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%s?",s,i.ni,i.di,i.ti,p[s].Dato.caracter,p[s+1].Dato.cadena);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%s?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.caracter,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                    case FLOAT:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%f?",s,i.ni,i.di,i.ti,p[s].Dato.caracter,p[s+1].Dato.flotante);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%f?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.caracter,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                }

                                break;

                            case CADENA:
                                switch(p[s+1].ti){
                                    case ENTERO:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%d?",s,i.ni,i.di,i.ti,p[s].Dato.cadena,p[s+1].Dato.entero);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.cadena,p[s+1].Dato.entero);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                    case BOOLEAN:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%d?",s,i.ni,i.di,i.ti,p[s].Dato.cadena,p[s+1].Dato.booleano);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.cadena,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CARACTER:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%c?",s,i.ni,i.di,i.ti,p[s].Dato.cadena,p[s+1].Dato.caracter);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%c?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.cadena,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CADENA:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%s?",s,i.ni,i.di,i.ti,p[s].Dato.cadena,p[s+1].Dato.cadena);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%s?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.cadena,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=(p[s].Dato.cadena!=p[s+1].Dato.cadena);
                                        p[s].ti=ENTERO;
                                        break;
                                    case FLOAT:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%f?",s,i.ni,i.di,i.ti,p[s].Dato.cadena,p[s+1].Dato.flotante);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%f?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.cadena,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=1;
                                        p[s].ti=ENTERO;
                                        break;
                                }

                                break;


                            case FLOAT:
                                switch(p[s+1].ti){
                                    case ENTERO:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%d?",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.entero);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.entero);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case BOOLEAN:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%d?",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.booleano);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%d?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CARACTER:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%c?",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.caracter);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%c?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case CADENA:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%s?",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.cadena);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%s?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=0;
                                        p[s].ti=ENTERO;
                                        break;
                                    case FLOAT:
                                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%f?",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.flotante);
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%f?",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=(p[s].Dato.flotante==p[s+1].Dato.flotante);
                                        p[s].ti=ENTERO;
                                        break;
                                }

                                break;
                        }
                        //printf("%d=%d? (s en %d)",p[s],p[s+1],s);
                        //p[s]=(p[s]==p[s+1]);
                        break;

                   case 10: //----------> MENOR QUE
                        --s;
                        //printf("%d<%d? (s en %d)",p[s],p[s+1],s);
                        
                        if(p[s].ti==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

                        if(p[s+1].ti==ENTERO) op2=float(p[s+1].Dato.entero);
                        else op2=p[s+1].Dato.flotante;

                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f<%f?",s,i.ni,i.di,i.ti,op1,op2);
                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f<%f?",s,i.ni,i.Dato.entero,i.ti,op1,op2);
                        p[s].Dato.entero=(op1<op2);
                        p[s].ti=ENTERO;
                        break;

                   case 11: //----------> MAYOR O IGUAL QUE
                        --s;
                        //printf("%d>=%d? (s en %d)",p[s],p[s+1],s);
                        //p[s]=(p[s]>=p[s+1]);
                        
                        if(p[s].ti==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

                        if(p[s+1].ti==ENTERO) op2=float(p[s+1].Dato.entero);
                        else op2=p[s+1].Dato.flotante;

                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f>=%f?",s,i.ni,i.di,i.ti,op1,op2);
                        p[s].Dato.entero=(op1>=op2);
                        p[s].ti=ENTERO;
                        break;

                   case 12: //----------> MAYOR QUE
                        --s; 
                       // printf("%d>%d? (s en %d)",p[s],p[s+1],s);
                       // p[s]=(p[s]>p[s+1]);
                        
                        if(p[s].ti==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

                        if(p[s+1].ti==ENTERO) op2=float(p[s+1].Dato.entero);
                        else op2=p[s+1].Dato.flotante;

                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f>%f?",s,i.ni,i.di,i.ti,op1,op2);
                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f>%f?",s,i.ni,i.Dato.entero,i.ti,op1,op2);
                        p[s].Dato.entero=(op1>op2);
                        p[s].ti=ENTERO;
                        break;

                   case 13: //----------> MENOR O IGUAL QUE
	                    --s;
                        //printf("%d<=%d? (s en %d)",p[s],p[s+1],s);
                        //p[s]=(p[s]<=p[s+1]);
                        
                        if(p[s].ti==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

                        if(p[s+1].ti==ENTERO) op2=float(p[s+1].Dato.entero);
                        else op2=p[s+1].Dato.flotante;

                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f<=%f?",s,i.ni,i.di,i.ti,op1,op2);
                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f<=%f?",s,i.ni,i.Dato.entero,i.ti,op1,op2);
                        p[s].Dato.entero=(op1<=op2);
                        p[s].ti=ENTERO;
                        break;
                    case 14:
                        break;
                    case 15:
                        break;
                    case 16:
                        break;
                    case 17:
                        break;
                    case 18:
                        break;
                    case 19:
                        break;
                    //-------------------- FUNCIONES DE M4 -------------------------     
                    case 20: //----------> SQRT
                        //printf("squrt( %d )? (s en %d)",p[s],s);
                        if(p[s].ti==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;


                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] sqrt(%f)?",s,i.ni,i.di,i.ti,op1);
                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] sqrt(%f)?",s,i.ni,i.Dato.entero,i.ti,op1);
                        p[s].Dato.flotante=sqrt(op1);
                        p[s].ti=FLOAT;
                        break;
                    case 21: //----------> POW
                        --s;
                        //printf("pow( %d, %d )? (s en %d) ",p[s],p[s+1],s);
                        if(p[s].ti==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

                        if(p[s+1].ti==ENTERO) op2=float(p[s+1].Dato.entero);
                        else op2=p[s+1].Dato.flotante;
                        

                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] pow(%f,%f)?",s,i.ni,i.di,i.ti,op1,op2);
                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] pow(%f,%f)?",s,i.ni,i.Dato.entero,i.ti,op1,op2);
                        
                        p[s].ti=FLOAT;
                        p[s].Dato.flotante=pow(op1,op2);
                        break;
                    case 22: //----------> SIN
                        //printf("sin( %d )? (s en %d)", p[s],s);
                        // p[s]=sin(p[s]);
                        if(p[s].ti==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;


                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] sin(%f)?",s,i.ni,i.di,i.ti,op1);
                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] sin(%f)?",s,i.ni,i.Dato.entero,i.ti,op1);
                        p[s].Dato.flotante=sin(op1);
                        p[s].ti=FLOAT;
                        break;
                    case 23: //----------> COS
                       // printf("cos( %d )? (s en %d)", p[s],s);
                       // p[s]=cos(p[s]);
                        if(p[s].ti==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;


                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] cos(%f)?",s,i.ni,i.di,i.ti,op1);
                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] cos(%f)?",s,i.ni,i.Dato.entero,i.ti,op1);
                        p[s].Dato.flotante=cos(op1);
                        p[s].ti=FLOAT;
                        break;
                    case 24: //----------> TAN
                       // printf("tan( %d )? (s en %d)", p[s],s);
                        //p[s]=tan(p[s]);
                        if(p[s].ti==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;


                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] tan(%f)?",s,i.ni,i.di,i.ti,op1);
                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] tan(%f)?",s,i.ni,i.Dato.entero,i.ti,op1);
                        p[s].Dato.flotante=tan(op1);
                        p[s].ti=FLOAT;
                        break;
                    case 25: //----------> LOG 
                        //printf("log( %d )? (s en %d)", p[s],s);
                        //p[s]=log(p[s]);
                        if(p[s].ti==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;


                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] log(%f)?",s,i.ni,i.di,i.ti,op1);
                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] log(%f)?",s,i.ni,i.Dato.entero,i.ti,op1);
                        p[s].Dato.flotante=log(op1);
                        p[s].ti=FLOAT;
                        break;
                    case 26: //---------->ARCSIN
                        //printf("arctan( %d )? (s en %d)", p[s],s);
                        //p[s]=asin(p[s]);
                        if(p[s].ti==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;


                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] asin(%f)?",s,i.ni,i.di,i.ti,op1);
                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] asin(%f)?",s,i.ni,i.Dato.entero,i.ti,op1);
                        p[s].Dato.flotante=asin(op1);
                        p[s].ti=FLOAT;
                        break;
                    case 27: //---------->ARCCOS
                        //printf("arccos( %d )? (s en %d)", p[s],s);
                        //p[s]=acos(p[s]);
                        if(p[s].ti==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;


                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] acos(%f)?",s,i.ni,i.di,i.ti,op1);
                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] acos(%f)?",s,i.ni,i.Dato.entero,i.ti,op1);
                        p[s].Dato.flotante=acos(op1);
                        p[s].ti=FLOAT;
                        break;
                    case 28: //---------->ARCTAN
                        //printf("arctan( %d )? (s en %d)", p[s],s);
                        //p[s]=atan(p[s]);
                        if(p[s].ti==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;


                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] atan(%f)?",s,i.ni,i.di,i.ti,op1);
                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] atan(%f)?",s,i.ni,i.Dato.entero,i.ti,op1);
                        p[s].Dato.flotante=atan(op1);
                        p[s].ti=FLOAT;
                        break;
                    case 29: //---------->ROUND
                        //printf("round( %d )? (s en %d)", p[s],s);
                        //p[s]=round(p[s]);
                        if(p[s].ti==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;


                        //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] round(%f)?",s,i.ni,i.di,i.ti,op1);
                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] round(%f)?",s,i.ni,i.Dato.entero,i.ti,op1);
                        p[s].Dato.flotante=round(op1);
                        p[s].ti=FLOAT;
                        break;
                    case 30: //----------> += 
                        --s;
                        //printf("suma igual de %d + %d (s en %d)",p[s],p[s+1],s);
                        //p[s]=p[s]+p[s+1];
                         if(p[s].ti==ENTERO && p[s+1].ti==ENTERO){
                          //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma igual de %d + %d",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma igual de %d + %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.entero);
                          p[s].Dato.entero=p[s].Dato.entero+p[s+1].Dato.entero;
                          p[s].ti=ENTERO;
                        }else if(p[s].ti==ENTERO && p[s+1].ti==FLOAT){
                          //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma igual de %d + %f",s,i.ni,i.di,i.ti,p[s].Dato.entero,p[s+1].Dato.flotante);
                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma igual de %d + %f",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.entero,p[s+1].Dato.flotante);
                          p[s].Dato.flotante=p[s].Dato.entero+p[s+1].Dato.flotante;
                          p[s].ti=FLOAT;
                        }else if(p[s].ti==FLOAT && p[s+1].ti==ENTERO){
                          //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma igual de %f + %d",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.entero);
                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma igual de %f + %d",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.entero);
                          p[s].Dato.flotante=p[s].Dato.flotante+p[s+1].Dato.entero;
                          p[s].ti=FLOAT;
                        }else if(p[s].ti==FLOAT && p[s+1].ti==FLOAT){
                          //printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma igual de %f + %f",s,i.ni,i.di,i.ti,p[s].Dato.flotante,p[s+1].Dato.flotante);
                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma igual de %f + %f",s,i.ni,i.Dato.entero,i.ti,p[s].Dato.flotante,p[s+1].Dato.flotante);
                          p[s].Dato.flotante=p[s].Dato.flotante+p[s+1].Dato.flotante;
                          p[s].ti=FLOAT;
                        }else{
                            fclose(obj);
                            printf("\nError a tiempo de ejecucion\nError en tipos de datos");
                            printf("\n--Programa abortado--");
                            exit(1); //el error es fatal
                        }  
                        break;
                    case 31: //----------> CONSOLE.WRITE
                        break;
                    case 32: //----------> CONSOLE.READ
                        break;
                    case 33: //----------> FCLOSE
                        break;
                    case 34: //----------> FOPEN
                        break;
                    case 35: //----------> FSCANF
                        break;
                    case 36: //----------> FGETS
                        break;
                    case 37: //----------> SPLIT
                        break;
                    case 38: //----------> TRIM
                        break;
                    case 39: //----------> LENGTH
                        break;
                    case 40: //----------> CONCAT
                        break;
                    case 41: //----------> REPLACE
                        break;
                    case 42: //----------> EQUAL
                        break;
               };
               break;
    
          case CAR:
			s++;
		switch(i.ti){
			case ENTERO:
              			 //p[++s]=p[base(i.ni,b)+i.di];
	         		 //printf("\nCAR : cargando %d en la direccion %d (s en %d)",p[base(i.ni,b)+i.di].Dato.entero,s,s);
				p[s].Dato.entero=p[base(i.ni,b)+i.Dato.entero].Dato.entero;
				p[s].ti = ENTERO;
	           		printf("\nCAR : cargando %d en la direccion %d (s en %d)",p[base(i.ni,b)+i.Dato.entero].Dato.entero,s,s);
				break;

			case FLOAT:
				p[s].Dato.flotante=p[base(i.ni,b)+i.Dato.entero].Dato.flotante;
				p[s].ti = FLOAT;
	           		printf("\nCAR : cargando %f en la direccion %d (s en %d)",p[base(i.ni,b)+i.Dato.entero].Dato.flotante,s,s);
				break;

			case CARACTER:
				p[s].Dato.caracter=p[base(i.ni,b)+i.Dato.entero].Dato.caracter;
				p[s].ti = CARACTER;
	           		printf("\nCAR : cargando %c en la direccion %d (s en %d)",p[base(i.ni,b)+i.Dato.entero].Dato.caracter,s,s);
				break;

			case CADENA:
				//p[s].Dato.cadena=p[base(i.ni,b)+i.Dato.entero].Dato.cadena;
				strcpy(p[s].Dato.cadena,p[base(i.ni,b)+i.Dato.entero].Dato.cadena);				
				p[s].ti = CADENA;
	           		printf("\nCAR : cargando %s en la direccion %d (s en %d)",p[base(i.ni,b)+i.Dato.entero].Dato.cadena,s,s);
				break;

			case BOOLEAN:
				p[s].Dato.booleano=p[base(i.ni,b)+i.Dato.entero].Dato.booleano;
				p[s].ti = BOOLEAN;
	           		printf("\nCAR : cargando %d en la direccion %d (s en %d)",p[base(i.ni,b)+i.Dato.entero].Dato.booleano,s,s);
				break;
		}
		break;

          case ALM:
               //printf("\nALM : almacenando %d en la direccion %d (s en %d)",p[s].Dato.entero,base(i.ni,b)+i.di,s-1);
               printf("\nALM : almacenando %d en la direccion %d (s en %d)",p[s].Dato.entero,base(i.ni,b)+i.Dato.entero,s-1);
               //p[base(i.ni,b)+i.di]=p[s];
               p[base(i.ni,b)+i.Dato.entero].Dato.entero=p[s].Dato.entero;
               --s;
               break;

          case LLA:
               //generar un nuevo bloque
               p[s+1].Dato.entero=base(i.ni,b);
               p[s+2].Dato.entero=b;
               p[s+3].Dato.entero=d;
               printf("\nLLA : activando subrutina, enlaces y DR: %d %d %d",p[s+1].Dato.entero,p[s+2].Dato.entero,p[s+3].Dato.entero);

               //b=s+1;d=i.di;
		b=s+1; d=i.Dato.entero;
               printf("\n    : nueva base %d, instruccion %d (s en %d)",b,d,s);
               break;

          case INS:
               //printf("\nINS : asignando %d espacios en el stack (s en %d)",i.di,s+i.di);
		printf("\nINS : asignando %d espacios en el stack (s en %d)",i.di,s+i.di);
               //s+=i.di;
		s+=i.Dato.entero;
               break;

          case SAL:
               //printf("\nSAL : saltando incondicionalmente a la instruccion %d (s en %d)",i.di,s);
		printf("\nSAL : saltando incondicionalmente a la instruccion %d (s en %d)",i.Dato.entero,s);
               //d=i.di;
		d=i.Dato.entero;
               break;

          case SAC:
               printf("\nSAC : ");
               if (p[s].Dato.entero==0) {
                  //d=i.di;
			d=i.Dato.entero;
                  printf("la condicion es falsa. saltando condicionalmente a la instruccion %d.",d);
               }
               else
                  printf("la condicion es verdadera. prosigo en la instruccion %d",d);

              --s;
	          printf("(s en %d)",s);
              break;
    };

 } while (d!=0);
}
 
//base: encuentra la base ni niveles más abajo en el stack para la operación actual (CAR,ALM,LLA)
int base(int ni,int b)
{
 int b1;
 b1=b;
 while (ni>0) {
       b1=p[b1].Dato.entero;
       --ni;
 }
 return (b1);
}

//listar_p: emitir listado de código-p
void listar_p()
{
 int i;
   
 printf("\n\n --Listado de codigo-p simplificado generado por el compilador--\n\n");
 printf("\n---ic: %d---\n",ic);
 printf("\n---mnemonico: %d---\n",sizeof(codigo[i]));


 for(i=0;i<ic;++i) {
     printf("\n %4d  %3s %5d %5d %s",i,mnemonico[codigo[i].f],codigo[i].ni,codigo[i].Dato.entero,comentario[codigo[i].f]);
 }
}


