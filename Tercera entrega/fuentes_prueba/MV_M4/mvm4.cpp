/* ESTE ES LA MAQUINA VIRTUAL QUE SIRVE, SOLO FALTA IMPLEMENTAR INTERPRETAR */
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
 int      ni; //nivel
 //int      di; //este valor depende del valor de f.
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
 tipo_dato t;
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

FILE *obj, *file_aux; //apuntador al archivo de código intermedio

//----> DECLARACION DE FUNCIONES.
void listar_p(void);
void interpretar(); 
int base(int ni,int b);
void leer(char *);
void replace_cad(int s);

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
		   interpretar();   	
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
     printf("\ncodigo[i].n====%d",codigo[i].ni);
     printf("\tcodigo[i].d====%d",codigo[i].Dato.entero);
     printf("\n %4d  %3s %5d %5d %s",i,mnemonico[codigo[i].f],codigo[i].ni,codigo[i].Dato.entero,comentario[codigo[i].f]);
 }
}


//----------------> EJECUCION DE MNEMONICOS.
void interpretar(void) {
	register int d;       //registro de dirección del programa: apunta a la siguiente instrucción por ejecutar
	register int b;       //registro de dirección base
	register int s;       //apuntador al tope del stack
	codigo_intermedio i;  //registro de instrucción: contiene la siguiente instrucción a ejecutar         
	float op1,op2; 		  // variables auxiliares

	s=-1;
	b=0;
	d=0;
	p[0].Dato.entero=p[1].Dato.entero=p[2].Dato.entero=0;   //ED, EE y DR para el programa principal
	p[0].t=p[1].t=p[2].t=ENTERO;
	
	//bucle de ejecución
	do {
		i=codigo[d++];
		printf("\n\nejecutando la instruccion %4d: %3s %5d %5d",d-1,mnemonico[i.f],i.ni,i.Dato.entero); 
		switch(i.f) {
        	case LIT: // LIT nivel X tipo_dato
        		printf("\n\n--ENTRO AL LIT--\n\n");
        		 ++s;
        		switch(i.t){
                  case ENTERO: 
                      p[s].Dato.entero= i.Dato.entero;
                      p[s].t=ENTERO;
                      printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %d en direccion %d ",s,i.ni,i.Dato.entero,i.t,i.Dato.entero,s);
                      break;
                  case FLOAT:
                      p[s].Dato.flotante= i.Dato.flotante;
                      p[s].t=FLOAT;
                      printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %f en direccion %d ",s,i.ni,i.Dato.entero,i.t,i.Dato.flotante,s);      
                      break;
                  case CARACTER:
                      p[s].Dato.caracter= i.Dato.caracter;
                      p[s].t=CARACTER;
                      printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %c en direccion %d ",s,i.ni,i.Dato.entero,i.t,i.Dato.caracter,s);
                      break;
                  case CADENA:
                      strcpy(p[s].Dato.cadena,i.Dato.cadena);
                      p[s].t=CADENA;
                      printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %s en direccion %d ",s,i.ni,i.Dato.entero,i.t,i.Dato.cadena,s);
                      break;
                  case BOOLEAN:
                      p[s].Dato.booleano= i.Dato.booleano;
                      p[s].t=BOOLEAN;
                      printf("\n[ s = %d ] [  LIT ---> N = %d  D = %d  T = %d  ] Cargado dato %d en direccion %d ",s,i.ni,i.Dato.entero,i.t,i.Dato.booleano,s);
                      break;
                }

               break;

            case OPR: // OPR nivel tipo_opr tipo_dato
				switch(i.Dato.entero) { 

					case 0: //retornar o fin
                        s=--b;
                        d=p[s+3].Dato.entero;
					    b=p[s+2].Dato.entero;
					    printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] Retornar a la instruccion %d, base=%d ",s,i.ni,i.Dato.entero,i.t,p[s+3].Dato.entero,b);
					    break;

					case 1: //----------> MENOS UNARIO
                        switch(p[s].t){
                          case ENTERO:
	                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] Operador ( - )  unario para %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero);
	                            p[s].Dato.entero=-p[s].Dato.entero;	        					
	                            break;

                          case FLOAT:
	                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] Operador ( - )  unario para %f",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante);
	                            p[s].Dato.flotante=-p[s].Dato.flotante;
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
                        if(p[s].t==ENTERO && p[s+1].t==ENTERO){
	                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma de %d + %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.entero);
	                          p[s].Dato.entero=p[s].Dato.entero+p[s+1].Dato.entero;
	                          p[s].t=ENTERO;
                        }else if(p[s].t==ENTERO && p[s+1].t==FLOAT){
	                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma de %d + %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.entero);
	                          p[s].Dato.flotante=p[s].Dato.entero+p[s+1].Dato.flotante;
	                          p[s].t=FLOAT;
                        }else if(p[s].t==FLOAT && p[s+1].t==ENTERO){
	                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma de %d + %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.entero);
	                          p[s].Dato.flotante=p[s].Dato.flotante+p[s+1].Dato.entero;
	                          p[s].t=FLOAT;
                        }else if(p[s].t==FLOAT && p[s+1].t==FLOAT){
	                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma de %d + %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.entero);
	                          p[s].Dato.flotante=p[s].Dato.flotante+p[s+1].Dato.flotante;
	                          p[s].t=FLOAT;
                        }else{
	                            fclose(obj);
	                            printf("\nError a tiempo de ejecucion\nError en tipos de datos");
	                            printf("\n--Programa abortado--");
	                            exit(1); //el error es fatal
                        }  
                        break;


                    case 3://----------> RESTA
                        --s; 
                        if(p[s].t==ENTERO && p[s+1].t==ENTERO){
		                      printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] resta de %d - %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.entero);
		                      p[s].Dato.entero=p[s].Dato.entero-p[s+1].Dato.entero;
		                      p[s].t=ENTERO;
                        }else if(p[s].t==ENTERO && p[s+1].t==FLOAT){
	                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] resta de %d - %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.entero);
	                          p[s].Dato.flotante=p[s].Dato.entero-p[s+1].Dato.flotante;
	                          p[s].t=FLOAT;
                        }else if(p[s].t==FLOAT && p[s+1].t==ENTERO){
	                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] resta de %d - %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.entero);
	                          p[s].Dato.flotante=p[s].Dato.flotante-p[s+1].Dato.entero;
	                          p[s].t=FLOAT;
                        }else if(p[s].t==FLOAT && p[s+1].t==FLOAT){
	                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] resta de %d - %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.entero);
	                          p[s].Dato.flotante=p[s].Dato.flotante-p[s+1].Dato.flotante;
	                          p[s].t=FLOAT;
                        }else{
                              fclose(obj);
                              printf("\nError a tiempo de ejecucion\nError en tipos de datos");
                              printf("\n--Programa abortado--");
                              exit(1); //el error es fatal
                        }
                        break;


                    case 4: //----------> MULTIPLICACION
                        --s;
                       if(p[s].t==ENTERO && p[s+1].t==ENTERO){
                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] multiplicacion de %d * %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.entero);
                            p[s].Dato.entero=p[s].Dato.entero*p[s+1].Dato.entero;
                            p[s].t=ENTERO;

                        }else if(p[s].t==ENTERO && p[s+1].t==FLOAT){
                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] multiplicacion de %d * %f",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.flotante);
                            p[s].Dato.flotante=p[s].Dato.entero*p[s+1].Dato.flotante;
                            p[s].t=FLOAT;

                        }else if(p[s].t==FLOAT && p[s+1].t==ENTERO){
                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] multiplicacion de %f * %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.entero);
                            p[s].Dato.flotante=p[s].Dato.flotante*p[s+1].Dato.entero;
                            p[s].t=FLOAT;

                        }else if(p[s].t==FLOAT && p[s+1].t==FLOAT){
                            printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] multiplicacion de %f * %f",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.flotante);
                            p[s].Dato.flotante=p[s].Dato.flotante*p[s+1].Dato.flotante;
                            p[s].t=FLOAT;

                        }else{
                            fclose(obj);
                            printf("\nError a tiempo de ejecucion\nError en tipos de datos");
                            printf("\n--Programa abortado--");
                            exit(1); //el error es fatal
                        }
                        break;


                    case 5: //---------->  DIVISION
						--s;
						if((p[s+1].t==ENTERO && p[s+1].Dato.entero==0) or (p[s+1].t==FLOAT && p[s+1].Dato.flotante==0)) {
						    fclose(obj);
						    printf("\nError a tiempo de ejecucion\nSe intenta dividir entre cero");
						    printf("\n--Programa abortado--");
						    exit(1); //el error es fatal
						}
						
						if(p[s].t==ENTERO && p[s+1].t==ENTERO){
						    printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] division de %d / %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.entero);
						    p[s].Dato.flotante=p[s].Dato.entero/p[s+1].Dato.entero;
						}else if(p[s].t==ENTERO && p[s+1].t==FLOAT){
						    printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] division de %d / %f",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.flotante);
						    p[s].Dato.flotante=p[s].Dato.entero/p[s+1].Dato.flotante;
						}else if(p[s].t==FLOAT && p[s+1].t==ENTERO){
						    printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] division de %f / %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.entero);
						    p[s].Dato.flotante=p[s].Dato.flotante/p[s+1].Dato.entero;
						}else if(p[s].t==FLOAT && p[s+1].t==FLOAT){
						    printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] division de %f / %f",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.flotante);
						    p[s].Dato.flotante=p[s].Dato.flotante/p[s+1].Dato.flotante;
						    
						}else{
						    fclose(obj);
						    printf("\nError a tiempo de ejecucion\nError en tipos de datos");
						    printf("\n--Programa abortado--");
						    exit(1); //el error es fatal
						}
						p[s].t=FLOAT;
						break;

                    case 6: //----------> ODD
                        switch(p[s].t){
                            case ENTERO:
                               printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] odd(%d)?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero);
                               p[s].Dato.entero=(((p[s].Dato.entero)%2)!=0);
                               p[s].t=ENTERO;
                               break;
                        }
                        break;
       
                    case 7: //sin uso
                        break;
                    
                    case 8: //----------> IGUAL (==)
                        --s;
                        switch(p[s].t){
                            case ENTERO:
                                switch(p[s+1].t){
                                    case ENTERO:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.entero);
                                        p[s].Dato.entero=(p[s].Dato.entero==p[s+1].Dato.entero);  
                                        break;
                                    case BOOLEAN:
                                        p[s].Dato.entero=0;
                                        break;
                                    case CARACTER:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%c?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=0;
                                        break;
                                    case CADENA:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%s?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=0;
                                        break;
                                    case FLOAT:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%f?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=(p[s].Dato.entero==p[s+1].Dato.flotante);
                                        break;
                                }
                                
                                break;
                            case BOOLEAN:
                                switch(p[s+1].t){
                                    case ENTERO:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.booleano,p[s+1].Dato.entero);
                                        p[s].Dato.entero=0;
                                        break;
                                    case BOOLEAN:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.booleano,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=(p[s].Dato.booleano==p[s+1].Dato.booleano);
                                        break;
                                    case CARACTER:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%c?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.booleano,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=0;
                                        break;
                                    case CADENA:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%s?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.booleano,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=0;
                                        break;
                                    case FLOAT:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d=%f?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.booleano,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=0;
                                        break;
                                }

                                break;
                            case CARACTER:
                                switch(p[s+1].t){
                                    case ENTERO:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.caracter,p[s+1].Dato.entero);
                                        p[s].Dato.entero=0;
                                        break;
                                    case BOOLEAN:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.caracter,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=0;
                                        break;
                                    case CARACTER:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%c?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.caracter,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=(p[s].Dato.caracter==p[s+1].Dato.caracter);

                                        break;
                                    case CADENA:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%s?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.caracter,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=0;

                                        break;
                                    case FLOAT:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c=%f?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.caracter,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=0;

                                        break;
                                }

                                break;

                            case CADENA:
                                switch(p[s+1].t){
                                    case ENTERO:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.cadena,p[s+1].Dato.entero);
                                        p[s].Dato.entero=0;
                                        break;
                                    case BOOLEAN:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.cadena,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=0;
                                        break;
                                    case CARACTER:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%c?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.cadena,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=0;
                                        break;
                                    case CADENA:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%s?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.cadena,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=(p[s].Dato.cadena==p[s+1].Dato.cadena);
                                        break;
                                    case FLOAT:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s=%f?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.cadena,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=0;
                                        break;
                                }
                                p[s].t=ENTERO;
                                break;


                            case FLOAT:
                                switch(p[s+1].t){
                                    case ENTERO:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.entero);
                                        p[s].Dato.entero=0;
                                        break;
                                    case BOOLEAN:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=0;
                                        break;
                                    case CARACTER:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%c?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=0;
                                        break;
                                    case CADENA:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%s?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=0;
                                        break;
                                    case FLOAT:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%f?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=(p[s].Dato.flotante==p[s+1].Dato.flotante);
                                        break;
                                }

                                break;
                        }
                        break;


                    case 9: //----------> DIFERENTE DE (!=)
                        --s;
                        switch(p[s].t){
                            case ENTERO:
                                switch(p[s+1].t){
                                    case ENTERO:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.entero);
                                        p[s].Dato.entero=(p[s].Dato.entero!=p[s+1].Dato.entero);
                                       
                                        break;
                                    case BOOLEAN:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=1;
                                        break;
                                    case CARACTER:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%c?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=1;
                                        break;
                                    case CADENA:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%s?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=1;
                                        break;
                                    case FLOAT:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%f?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=(p[s].Dato.entero!=p[s+1].Dato.flotante);
                                        p[s].t=ENTERO;
                                        break;
                                }

                                break;
                            case BOOLEAN:
                                switch(p[s+1].t){
                                    case ENTERO:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.booleano,p[s+1].Dato.entero);
                                        p[s].Dato.entero=1;
                                        break;
                                    case BOOLEAN:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.booleano,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=(p[s].Dato.booleano!=p[s+1].Dato.booleano);
                                        break;
                                    case CARACTER:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%c?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.booleano,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=1;
                                        break;
                                    case CADENA:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%s?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.booleano,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=1;
                                        break;
                                    case FLOAT:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %d!=%f?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.booleano,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=1;
                                        break;
                                }

                                break;
                            case CARACTER:
                                switch(p[s+1].t){
                                    case ENTERO:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.caracter,p[s+1].Dato.entero);
                                        p[s].Dato.entero=1;
                                        p[s].t=ENTERO;
                                        break;
                                    case BOOLEAN:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.caracter,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=1;
                                        p[s].t=ENTERO;
                                        break;
                                    case CARACTER:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%c?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.caracter,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=(p[s].Dato.caracter!=p[s+1].Dato.caracter);
                                        p[s].t=ENTERO;
                                        break;
                                    case CADENA:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%s?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.caracter,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=1;
                                        p[s].t=ENTERO;
                                        break;
                                    case FLOAT:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %c!=%f?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.caracter,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=1;
                                        p[s].t=ENTERO;
                                        break;
                                }

                                break;

                            case CADENA:
                                switch(p[s+1].t){
                                    case ENTERO:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.cadena,p[s+1].Dato.entero);
                                        p[s].Dato.entero=1;
                                        break;
                                    case BOOLEAN:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.cadena,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=1;
                                        break;
                                    case CARACTER:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%c?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.cadena,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=1;
                                        break;
                                    case CADENA:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%s?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.cadena,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=(p[s].Dato.cadena!=p[s+1].Dato.cadena);
                                        break;
                                    case FLOAT:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %s!=%f?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.cadena,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=1;
                                        break;
                                }

                                break;


                            case FLOAT:
                                switch(p[s+1].t){
                                    case ENTERO:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.entero);
                                        p[s].Dato.entero=0;
                                        break;
                                    case BOOLEAN:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%d?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.booleano);
                                        p[s].Dato.entero=0;
                                        break;
                                    case CARACTER:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%c?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.caracter);
                                        p[s].Dato.entero=0;
                                        break;
                                    case CADENA:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%s?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.cadena);
                                        p[s].Dato.entero=0;
                                        break;
                                    case FLOAT:
                                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f=%f?",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.flotante);
                                        p[s].Dato.entero=(p[s].Dato.flotante==p[s+1].Dato.flotante);
                                        break;
                                }

                                break;
                        }
                        p[s].t=ENTERO;
                        break;

                    case 10: //----------> MENOR QUE
                        --s;
                        if(p[s].t==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

                        if(p[s+1].t==ENTERO) op2=float(p[s+1].Dato.entero);
                        else op2=p[s+1].Dato.flotante;

                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f<%f?",s,i.ni,i.Dato.entero,i.t,op1,op2);
                        p[s].Dato.entero=(op1<op2);
                        p[s].t=ENTERO;
                        break;

                    case 11: //----------> MAYOR O IGUAL QUE
                        --s;
                        if(p[s].t==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

                        if(p[s+1].t==ENTERO) op2=float(p[s+1].Dato.entero);
                        else op2=p[s+1].Dato.flotante;

                        p[s].Dato.entero=(op1>=op2);
                        p[s].t=ENTERO;
                        break;

                    case 12: //----------> MAYOR QUE
                        --s; 
                       
                        if(p[s].t==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

                        if(p[s+1].t==ENTERO) op2=float(p[s+1].Dato.entero);
                        else op2=p[s+1].Dato.flotante;

                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f>%f?",s,i.ni,i.Dato.entero,i.t,op1,op2);
                        p[s].Dato.entero=(op1>op2);
                        p[s].t=ENTERO;
                        break;

                    case 13: //----------> MENOR O IGUAL QUE
                        --s;
                        
                        if(p[s].t ==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

                        if(p[s+1].t ==ENTERO) op2=float(p[s+1].Dato.entero);
                        else op2=p[s+1].Dato.flotante;

                        printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] %f<=%f?",s,i.ni,i.Dato.entero,i.t,op1,op2);
                        p[s].Dato.entero=(op1<=op2);
                        p[s].t=ENTERO;
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
                        if(p[s].t==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;
						printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] sqrt(%f)?",s,i.ni,i.Dato.entero,i.t,op1);
                        p[s].Dato.flotante=sqrt(op1);
                        p[s].t=FLOAT;
                        break;

                    case 21: //----------> POW
                        --s;
                        if(p[s].t==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

                        if(p[s+1].t==ENTERO) op2=float(p[s+1].Dato.entero);
                        else op2=p[s+1].Dato.flotante;
                        
						printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] pow(%f,%f)?",s,i.ni,i.Dato.entero,i.t,op1,op2);
                        
                        p[s].t=FLOAT;
                        p[s].Dato.flotante=pow(op1,op2);
                        break;
					
					case 22: //----------> SIN
                        
                        if(p[s].t==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;
						
						printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] sin(%f)?",s,i.ni,i.Dato.entero,i.t,op1);
                        p[s].Dato.flotante=sin(op1);
                        p[s].t=FLOAT;
                        break;
                    
                    case 23: //----------> COS
                        if(p[s].t==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

						printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] cos(%f)?",s,i.ni,i.Dato.entero,i.t,op1);
                        p[s].Dato.flotante=cos(op1);
                        p[s].t=FLOAT;
                        break;

                    case 24: //----------> TAN
                        if(p[s].t==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

						printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] tan(%f)?",s,i.ni,i.Dato.entero,i.t,op1);
                        p[s].Dato.flotante=tan(op1);
                        p[s].t=FLOAT;
                        break;

                    case 25: //----------> LOG 
                        if(p[s].t==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

						printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] log(%f)?",s,i.ni,i.Dato.entero,i.t,op1);
                        p[s].Dato.flotante=log(op1);
                        p[s].t=FLOAT;
                        break;

                    case 26: //---------->ARCSIN
                        if(p[s].t==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

						printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] asin(%f)?",s,i.ni,i.Dato.entero,i.t,op1);
                        p[s].Dato.flotante=asin(op1);
                        p[s].t=FLOAT;
                        break;

                    case 27: //---------->ARCCOS
                        if(p[s].t==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

						printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] acos(%f)?",s,i.ni,i.Dato.entero,i.t,op1);
                        p[s].Dato.flotante=acos(op1);
                        p[s].t=FLOAT;
                        break;

                    case 28: //---------->ARCTAN
                        if(p[s].t==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

						printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] atan(%f)?",s,i.ni,i.Dato.entero,i.t,op1);
                        p[s].Dato.flotante=atan(op1);
                        p[s].t=FLOAT;
                        break;

                    case 29: //---------->ROUND
                        if(p[s].t==ENTERO) op1=float(p[s].Dato.entero);
                        else op1=p[s].Dato.flotante;

						printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] round(%f)?",s,i.ni,i.Dato.entero,i.t,op1);
                        p[s].Dato.flotante=round(op1);
                        p[s].t=FLOAT;
                        break;

                    case 30: //----------> += 
                        --s;
                        if(p[s].t==ENTERO && p[s+1].t==ENTERO){
	                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma igual de %d + %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.entero);
	                          p[s].Dato.entero=p[s].Dato.entero+p[s+1].Dato.entero;
	                          p[s].t=ENTERO;
                        }else if(p[s].t==ENTERO && p[s+1].t==FLOAT){
                          	  printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma igual de %d + %f",s,i.ni,i.Dato.entero,i.t,p[s].Dato.entero,p[s+1].Dato.flotante);
	                          p[s].Dato.flotante=p[s].Dato.entero+p[s+1].Dato.flotante;
	                          p[s].t=FLOAT;
                        }else if(p[s].t==FLOAT && p[s+1].t==ENTERO){
	                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma igual de %f + %d",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.entero);
	                          p[s].Dato.flotante=p[s].Dato.flotante+p[s+1].Dato.entero;
	                          p[s].t=FLOAT;
                        }else if(p[s].t==FLOAT && p[s+1].t==FLOAT){
	                          printf("\n[ s = %d ] [ OPR ---> N = %d D= %d T = %d ] suma igual de %f + %f",s,i.ni,i.Dato.entero,i.t,p[s].Dato.flotante,p[s+1].Dato.flotante);
	                          p[s].Dato.flotante=p[s].Dato.flotante+p[s+1].Dato.flotante;
	                          p[s].t=FLOAT;
                        }else{
                            fclose(obj);
                            printf("\nError a tiempo de ejecucion\nError en tipos de datos");
                            printf("\n--Programa abortado--");
                            exit(1); //el error es fatal
                        }  

                    case 31: //----------> CONSOLE.WRITE
                    	
                    	printf("\n%s",p[s].Dato.cadena);
                    	s=s-1;
                        break;

                    case 32: //----------> CONSOLE.READ
                    	switch(i.t){
                    		s++;
                    		case ENTERO:
                    			scanf("%d",p[s].Dato.entero);
                    			p[s].t=ENTERO;
                    			break;
                    		case CADENA:
                    			scanf("%s",p[s].Dato.cadena);
                    			p[s].t=CADENA;
                    			break;
                    		case BOOLEAN:
                    			scanf("%d",p[s].Dato.booleano);
                    			p[s].t=BOOLEAN;
                    			break;
                    		case CARACTER:
                    			scanf("%c",p[s].Dato.caracter);
                    			p[s].t=CARACTER;
                    			break;
                    		case FLOAT:
                    			scanf("%f",p[s].Dato.flotante);
                    			p[s].t=FLOAT;
                    			break;
                    	}
                        break;

                    case 33: //----------> FCLOSE
                    	fclose(file_aux);
                        break;

                    case 34: //----------> FOPEN
                    	file_aux = fopen (p[s].Dato.cadena, "r");
                    	s=s-1;
                        break;

                    case 35: //----------> FSCANF
                    	s++;
                    	fscanf (file_aux, "%s", &p[s].Dato.cadena);
                    	p[s].t=CADENA;
                        break;

                    case 36: //----------> FGETS
                    	fgets (p[s].Dato.cadena, MAXSTRING, file_aux);
                    	p[s].t=CADENA;
                        break;

                    case 37: //----------> SPLIT
                        break;

                    case 38: //----------> LENGTH
                    	p[s].Dato.entero= strlen(p[s].Dato.cadena);
                    	p[s].t=ENTERO;
                        break;

                    case 39: //----------> CONCAT
                    	--s;
                    	strcat(p[s].Dato.cadena, p[s+1].Dato.cadena);
                        break;

                    case 40: //----------> REPLACE
                    	s=s-2;
                    	replace_cad(s);
                        break;

                    case 41: //----------> EQUAL
                    	--s;
                    	int res= strcmp(p[s].Dato.cadena,p[s+1].Dato.cadena);
                    	if(res<0 || res > 0){
                    		p[s].Dato.entero=0;
                    	}else{
                    		p[s].Dato.entero=1;
                    	}
                    	p[s].t=ENTERO;
                        break;

				}
			break; // FIN DE SWITCH DE OPR

			//------------------------------------------------------------------------------
			case CAR:
			    s++;
			    switch(i.t){
			      case ENTERO:
			        p[s].Dato.entero=p[base(i.ni,b)+i.Dato.entero].Dato.entero;
			        p[s].t = ENTERO;
			        printf("\nCAR : cargando %d en la direccion %d (s en %d)",p[base(i.ni,b)+i.Dato.entero].Dato.entero,s,s);
			        break;

			      case FLOAT:
			        p[s].Dato.flotante=p[base(i.ni,b)+i.Dato.entero].Dato.flotante;
			        p[s].t = FLOAT;
			        printf("\nCAR : cargando %f en la direccion %d (s en %d)",p[base(i.ni,b)+i.Dato.entero].Dato.flotante,s,s);
			        break;

			      case CARACTER:
			        p[s].Dato.caracter=p[base(i.ni,b)+i.Dato.entero].Dato.caracter;
			        p[s].t = CARACTER;
			        printf("\nCAR : cargando %c en la direccion %d (s en %d)",p[base(i.ni,b)+i.Dato.entero].Dato.caracter,s,s);
			        break;

			      case CADENA:
			        //p[s].Dato.cadena=p[base(i.ni,b)+i.Dato.entero].Dato.cadena;
			        strcpy(p[s].Dato.cadena,p[base(i.ni,b)+i.Dato.entero].Dato.cadena);       
			        p[s].t = CADENA;
			        printf("\nCAR : cargando %s en la direccion %d (s en %d)",p[base(i.ni,b)+i.Dato.entero].Dato.cadena,s,s);
			        break;

			      case BOOLEAN:
			        p[s].Dato.booleano=p[base(i.ni,b)+i.Dato.entero].Dato.booleano;
			        p[s].t = BOOLEAN;
			        printf("\nCAR : cargando %d en la direccion %d (s en %d)",p[base(i.ni,b)+i.Dato.entero].Dato.booleano,s,s);
			        break;
			    }
			    break;
			//---------------------------------------------------------------------------------

			case ALM:
	               printf("\nALM : almacenando %d en la direccion %d (s en %d)",p[s].Dato.entero,base(i.ni,b)+i.Dato.entero,s-1);
	               p[base(i.ni,b)+i.Dato.entero].Dato.entero=p[s].Dato.entero;
	               p[base(i.ni,b)+i.Dato.entero].t=p[s].t;
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
    			   s+=i.Dato.entero;
               	   break;

          	case SAL:
               printf("\nSAL : saltando incondicionalmente a la instruccion %d (s en %d)",i.Dato.entero,s);
               d=i.Dato.entero;
               break;

          	case SAC:
               printf("\nSAC : ");
	            if (p[s].Dato.entero==0) {
	    				d=i.Dato.entero;
	                  	printf("la condicion es falsa. saltando condicionalmente a la instruccion %d.",d);
	            }else
	                  	printf("la condicion es verdadera. prosigo en la instruccion %d",d);

	            --s;
            	printf("(s en %d)",s);
              break;

		}
	} while (d!=0);
}



//---------------------> FUNCIONES AUXILIARES
void replace_cad(int s){
	char string[MAXSTRING];
	char string2, string3;
	strcpy(string,p[s].Dato.cadena);
	string2=p[s+1].Dato.caracter;
	string3=p[s+2].Dato.caracter;
	int i;

	for (i=0; i< strlen(string); i++){
        if((int) string[i]== (int) string2){
            string[i]=string3;
        }
    }
    strcpy(p[s].Dato.cadena,string);
}