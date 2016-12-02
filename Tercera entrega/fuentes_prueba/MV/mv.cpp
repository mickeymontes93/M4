#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <string.h>

#define MAXID     100  /*Maxima longitud de los identificadores        */
#define MAXIC     1000  /*Tamaño del array codigo: instrucciones codigo-p*/
#define LONGSTACKs 500  /*Longitud del stack de datos                    */
#define MAXTEXT   300  /*Maxima longitud de los identificadores        */

/*    Instrucciones del ensamblador (codigo-p)    */
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC,RET};
char *mnemonico[]={"LIT","OPR","CAR","ALM","LLA","INS","SAL","SAC","RET"};

typedef struct {
 enum fcn f; //es el mnemónico de la instrucción
 int      n; //es la diferencia estática de nivel de la variable: Un 0 global, Un 1 nivel local.
 int      d; //este valor depende del valor de f.
 int      t; //indica el tipo de dato con el que la instrucción trabaja:
			/*
			0. Es 0 si la instrucción no trabaja con ningún tipo de datos en específico.
			1. Es 1 si se trabaja con números enteros.
			2. Es 2 si se trabajo con números reales.
			3. Es 3 si se trabaja con booleano.
			4. Es 4 si se trabaja con texto.
			5. Es 5 si se trabaja con caracter.
			6. En el caso de la instrucción LLA indicará la cantidad de casillas utilizadas por los parámetros del subalgoritmo a ejecutar.
			7. En el caso de la instrucción INS con valor de 1 en n indicará si la variable para la que se reservará espacio en la pila de memoria se trata de un arreglo			
			*/
 union{
	int entero;
	float flotante;
	int booleano;
	char caracter;
	char texto[MAXTEXT];
 }dato;
} codigo_intermedio;

codigo_intermedio codigo[MAXIC]; //array con las instrucciones de codigo-p
int ic;                          //índice sobre el array de código-p

enum tipoPilaDato {tref, tint, tflt, tboo, tstr, tchr ,ED, EE, DR};
char *tipoPilaDatoS[]={"tref", "tint", "tflt", "tboo", "tstr", "tchr" ,"ED", "EE", "DR"};

typedef struct {
 tipoPilaDato tipo;
 union{
	int entero;
	float flotante;
	int booleano;
	char caracter;
	char texto[MAXTEXT];
 }dato;
} stack;

stack pilaDatos[MAXIC];
stack ret[25];					 //array para paso de parametros
int iret;						 //índice sobre el array de ret
int ip; 

FILE *obj; //apuntador al archivo de código intermedio

void listar_p(void);
void listar_pilaDatos(int opcion,int topPila);
void interpretar(); 
void colocarPilaDatos(int posicion,codigo_intermedio i,int EEg);
tipoPilaDato traducirAtipo(int t);
void colocarPilaDatosALM(int posicion,tipoPilaDato tipo,int topePila);
void InvertirCadena(char * cadena);
void uppCadena(char * cadena);
void listar_pilaRet(int ix,int topPila);
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
		   printf("\n IC----->%d",ic);
		   fread(&codigo[ic],sizeof(codigo_intermedio),1,obj);
		   printf("\tF----->%d",codigo[ic].f);
		   while ( !feof(obj) ) {
				 ++ic;
				 fread(&codigo[ic],sizeof(codigo_intermedio),1,obj);
				  printf("\n IC----->%d",ic);
				  printf("\tF----->%d",codigo[ic].f);
				 
		   }
		   //cierra el archivo
		   fclose(obj);

		   //listar_p(); //quita el comentario de esta línea si deseas verificar que el código se cargo al array correctamente
		

		   //interpreta el codigo-p 
		   interpretar();   	
		}
		printf("\n\nFinalizo la ejecucion\n");
		system("PAUSE");
		return(0);
 }

//interpretar: interpreta código-p                
void interpretar(void) {

 register int d;       //registro de dirección del programa: apunta a la siguiente instrucción por ejecutar
 register int b;       //registro de dirección base
 register int s;       //apuntador al tope del stack
 codigo_intermedio i;  //registro de instrucción: contiene la siguiente instrucción a ejecutar		     
	
 int bandera;
 int bandera2;
 int DRg=0;
 int EDg=0;
 int EEg=0;
 int controlPrintf=0;

 s=-1;b=0;d=0;
 pilaDatos[0].tipo=ED;
 pilaDatos[1].tipo=EE;
 pilaDatos[2].tipo=DR;
 pilaDatos[0].dato.entero=pilaDatos[1].dato.entero=pilaDatos[2].dato.entero=0;   //ED,EE y DR para el programa principal

 do {
	 i=codigo[d++];
	 if(controlPrintf)printf("\n\nejecutando %d: %s n=%1d d=%1d t=%1d (s=%d)",d-1,mnemonico[i.f],i.n,i.d,i.t,s); 
	 switch(i.f) {
		 case LIT:		
		 /* f(LIT)  n  d  t*   :Cargar un valor literal al tope de la pila. Este valor literal puede ser un número
								entero, un número real, un carácter o una cadena.
								t= Tipo de variable int, boo etc. (para acceder .dato)*/
            colocarPilaDatos(++s,i,EEg);
            break;
		case OPR:
		/*	f(OPR)  n  d* t    :Referencia a operación relacional, aritmética o predefinida en el lenguaje 
								d=	0: retornar o fin	1: menos unario		2: sumar				
									3: resta			4: multiplicacion	5: division
									6: odd				7: even				8: esigual ==
									9: diferentede !=  10: menor <		   11: menorigual <=
								   12: mayor >		   13: mayorigual >=   14: max 
								   15: min			   16: abs			   17: pow
								   18: reverse		   19: upp			   20: append
								   21: out (t=tipo)*/
			if(controlPrintf)printf("\nOPR : ");
			switch(i.d){
			case 0:
				if(i.n==0){
					d=DRg;
					s=EDg;
					if(controlPrintf)printf("retornar o fin d:DR=%d s:ED=%d",d,s);
					for(bandera=s;bandera>=0;bandera--){
						if(pilaDatos[bandera].tipo==DR){
							DRg=pilaDatos[bandera].dato.entero;
							EDg=pilaDatos[bandera+1].dato.entero;
							EEg=pilaDatos[bandera+2].dato.entero;
							if(controlPrintf)printf("\n DR: %d ED: %d EE: %d",DRg,EDg,EEg);
							bandera=-1;
						}
					}
				}else if(i.n==1){
					if(controlPrintf)printf("guardar el return");
					d=DRg;
					pilaDatos[EDg+1].tipo=pilaDatos[s].tipo;
					switch (pilaDatos[EDg+1].tipo)
					{
						case 0: pilaDatos[EDg+1].dato.entero      =pilaDatos[s].dato.entero+EEg;break;
						case 1: pilaDatos[EDg+1].dato.entero      =pilaDatos[s].dato.entero;break;
						case 2: pilaDatos[EDg+1].dato.flotante    =pilaDatos[s].dato.flotante;break;
						case 3: pilaDatos[EDg+1].dato.booleano    =pilaDatos[s].dato.booleano;break;
						case 4: strcpy(pilaDatos[EDg+1].dato.texto,pilaDatos[s].dato.texto);break;
						case 5: pilaDatos[EDg+1].dato.caracter    =pilaDatos[s].dato.caracter;break;
					}
					s=EDg+1;
					for(bandera=s-1;bandera>=0;bandera--){
						if(pilaDatos[bandera].tipo==DR){
							DRg=pilaDatos[bandera].dato.entero;
							EDg=pilaDatos[bandera+1].dato.entero;
							EEg=pilaDatos[bandera+2].dato.entero;
							//printf("\n DR: %d ED: %d EE: %d",DRg,EDg,EEg);
							bandera=-1;
						}
					}
				}
				break;

			case 1://menos unario
				if(controlPrintf)printf("menos unario");
                if(pilaDatos[s].tipo==tint)
					pilaDatos[s].dato.entero=-pilaDatos[s].dato.entero;
				else if(pilaDatos[s].tipo==tflt)
					pilaDatos[s].dato.flotante=-pilaDatos[s].dato.flotante;
				if(pilaDatos[s].tipo==tboo)
					pilaDatos[s].dato.booleano=!pilaDatos[s].dato.booleano;
				break;

			case 2://sumar
				if(controlPrintf)printf("sumar");
					 if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.entero   = pilaDatos[s-1].dato.entero   + pilaDatos[s].dato.entero;
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.flotante = pilaDatos[s-1].dato.flotante + pilaDatos[s].dato.flotante;
				else if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.entero   = pilaDatos[s-1].dato.entero   + pilaDatos[s].dato.flotante;
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.flotante = pilaDatos[s-1].dato.flotante + pilaDatos[s].dato.entero;
				s--;
				break;

			case 3://restar
				if(controlPrintf)printf("restar");
					 if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.entero   = pilaDatos[s-1].dato.entero   - pilaDatos[s].dato.entero;
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.flotante = pilaDatos[s-1].dato.flotante - pilaDatos[s].dato.flotante;
				else if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.entero   = pilaDatos[s-1].dato.entero   - pilaDatos[s].dato.flotante;
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.flotante = pilaDatos[s-1].dato.flotante - pilaDatos[s].dato.entero;
				s--;
				break;
			case 4://multiplicar
				if(controlPrintf)printf("multiplicar");
					 if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.entero   = pilaDatos[s-1].dato.entero   * pilaDatos[s].dato.entero;
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.flotante = pilaDatos[s-1].dato.flotante * pilaDatos[s].dato.flotante;
				else if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.entero   = pilaDatos[s-1].dato.entero   * pilaDatos[s].dato.flotante;
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.flotante = pilaDatos[s-1].dato.flotante * pilaDatos[s].dato.entero;
				s--;
				break;
			case 5://divirdr
				if(controlPrintf)printf("dividir");
				if(pilaDatos[s].tipo==tint && pilaDatos[s].dato.entero==0){   printf("\nError division entre 0, se cerrara el programa"); system("PAUSE"); exit(1);}
				if(pilaDatos[s].tipo==tflt && pilaDatos[s].dato.flotante==0){ printf("\nError division entre 0, se cerrara el programa"); system("PAUSE"); exit(1);}
					 if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.entero   = pilaDatos[s-1].dato.entero   / pilaDatos[s].dato.entero;
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.flotante = pilaDatos[s-1].dato.flotante / pilaDatos[s].dato.flotante;
				else if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.entero   = pilaDatos[s-1].dato.entero   / pilaDatos[s].dato.flotante;
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.flotante = pilaDatos[s-1].dato.flotante / pilaDatos[s].dato.entero;
				s--;
				break;
			case 6://odd
				if(controlPrintf)printf("odd");
				if(pilaDatos[s].tipo==tint){ 
					pilaDatos[s].dato.booleano=!(pilaDatos[s].dato.entero%2!=0);
					pilaDatos[s].tipo=tboo;
				}else{
					printf("\nError solo se puede sacar par a numeros enteros"); system("PAUSE"); exit(1);
				}
				break;
			case 7://even
				if(controlPrintf)printf("even");
				if(pilaDatos[s].tipo==tint){ 
					pilaDatos[s].dato.booleano=(pilaDatos[s].dato.entero%2!=0);
					pilaDatos[s].tipo=tboo;
				}else{
					printf("\nError solo se puede sacar par a numeros enteros"); system("PAUSE"); exit(1);
				}
				break;

			case 8://esigual ==
				if(controlPrintf)printf(" es igual de ==");
                     if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.entero   == pilaDatos[s].dato.entero);
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.flotante == pilaDatos[s].dato.flotante);
				else if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.entero   == pilaDatos[s].dato.flotante);
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.flotante == pilaDatos[s].dato.entero);
				pilaDatos[s-1].tipo=tboo;
				s--;
				break;

			case 9://esdiferente !=
				if(controlPrintf)printf(" es diferente de !=");
                     if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.entero   != pilaDatos[s].dato.entero);
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.flotante != pilaDatos[s].dato.flotante);
				else if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.entero   != pilaDatos[s].dato.flotante);
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.flotante != pilaDatos[s].dato.entero);
				pilaDatos[s-1].tipo=tboo;
				s--;
				break;

			case 10://es menor
				if(controlPrintf)printf(" es menor <");
                     if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.entero   < pilaDatos[s].dato.entero);
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.flotante < pilaDatos[s].dato.flotante);
				else if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.entero   < pilaDatos[s].dato.flotante);
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.flotante < pilaDatos[s].dato.entero);
				pilaDatos[s-1].tipo=tboo;
				s--;
				break;

			case 11://es menor igual
				if(controlPrintf)printf(" es menorigual <=");
                     if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.entero   <= pilaDatos[s].dato.entero);
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.flotante <= pilaDatos[s].dato.flotante);
				else if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.entero   <= pilaDatos[s].dato.flotante);
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.flotante <= pilaDatos[s].dato.entero);
				pilaDatos[s-1].tipo=tboo;
				s--;
				break;

			case 12://es mayor
				if(controlPrintf)printf(" es mayo >");
                     if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.entero   > pilaDatos[s].dato.entero);
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.flotante > pilaDatos[s].dato.flotante);
				else if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.entero   > pilaDatos[s].dato.flotante);
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.flotante > pilaDatos[s].dato.entero);
				pilaDatos[s-1].tipo=tboo;
				s--;
				break;

			case 13://es mayor igual
				if(controlPrintf)printf(" es mayorigual >=");
                     if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.entero   >= pilaDatos[s].dato.entero);
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.flotante >= pilaDatos[s].dato.flotante);
				else if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.entero   >= pilaDatos[s].dato.flotante);
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.flotante >= pilaDatos[s].dato.entero);
				pilaDatos[s-1].tipo=tboo;
				s--;
				break;

			case 14://max
				if(controlPrintf)printf(" maximo de %d",i.n);
				float maximo;
					 if(pilaDatos[s].tipo==tint) maximo=pilaDatos[s].dato.entero; 
				else if(pilaDatos[s].tipo==tflt) maximo=pilaDatos[s].dato.flotante; 
				for(bandera=s;bandera>s-i.n;bandera--){
						 if(pilaDatos[bandera].tipo==tint && pilaDatos[bandera].dato.entero   > maximo ) maximo=pilaDatos[bandera].dato.entero; 
					else if(pilaDatos[bandera].tipo==tflt && pilaDatos[bandera].dato.flotante > maximo ) maximo=pilaDatos[bandera].dato.flotante; 
				}
				s=s-i.n+1;
				pilaDatos[s].tipo=tflt;
				pilaDatos[s].dato.flotante=maximo;
				break;
			case 15://min
				if(controlPrintf)printf(" minimo de %d",i.n);
				float minimo;
					 if(pilaDatos[s].tipo==tint) minimo=pilaDatos[s].dato.entero; 
				else if(pilaDatos[s].tipo==tflt) minimo=pilaDatos[s].dato.flotante; 
				for(bandera=s;bandera>s-i.n;bandera--){
						 if(pilaDatos[bandera].tipo==tint && pilaDatos[bandera].dato.entero   < minimo ) minimo=pilaDatos[bandera].dato.entero; 
					else if(pilaDatos[bandera].tipo==tflt && pilaDatos[bandera].dato.flotante < minimo ) minimo=pilaDatos[bandera].dato.flotante; 
				}
				s=s-i.n+1;
				pilaDatos[s].tipo=tflt;
				pilaDatos[s].dato.flotante=minimo;
				break;
			case 16://abs
				if(controlPrintf)printf(" abs ");
					 if(pilaDatos[s].tipo==tint && pilaDatos[s].dato.entero  <0) pilaDatos[s].dato.entero   =-pilaDatos[s].dato.entero;
				else if(pilaDatos[s].tipo==tflt && pilaDatos[s].dato.flotante<0) pilaDatos[s].dato.flotante =-pilaDatos[s].dato.flotante;
				break;
			case 17://pow
				if(controlPrintf)printf(" pow ");
					 if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.flotante = pow((float)pilaDatos[s-1].dato.entero   , pilaDatos[s].dato.entero);
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.flotante = pow(pilaDatos[s-1].dato.flotante , pilaDatos[s].dato.flotante);
				else if(pilaDatos[s-1].tipo==tint && pilaDatos[s].tipo==tflt) pilaDatos[s-1].dato.flotante = pow(pilaDatos[s-1].dato.entero   , pilaDatos[s].dato.flotante);
				else if(pilaDatos[s-1].tipo==tflt && pilaDatos[s].tipo==tint) pilaDatos[s-1].dato.flotante = pow(pilaDatos[s-1].dato.flotante , pilaDatos[s].dato.entero);
				pilaDatos[s-1].tipo=tflt;
				s--;
				break;
			case 18://reverse
				if(controlPrintf)printf("reverse");
				InvertirCadena(pilaDatos[s].dato.texto);
				break;
			case 19://upp
				if(controlPrintf)printf("mayusculas");
				uppCadena(pilaDatos[s].dato.texto);
				break;
			case 20://append
				if(controlPrintf)printf(" append ");
				strcat(pilaDatos[s-1].dato.texto,pilaDatos[s].dato.texto);
				s--;
				break;
			case 21://out
				if(controlPrintf)printf(" out ");
					 if(pilaDatos[s].tipo==tint) printf("%d",pilaDatos[s].dato.entero);
				else if(pilaDatos[s].tipo==tflt) printf("%f",pilaDatos[s].dato.flotante);
				else if(pilaDatos[s].tipo==tboo && pilaDatos[s].dato.booleano==1) printf("true");
				else if(pilaDatos[s].tipo==tboo && pilaDatos[s].dato.booleano==0) printf("false");
				else if(pilaDatos[s].tipo==tstr){// para ver los enter
					if(!strcmp(pilaDatos[s].dato.texto,"\\n"))
						printf("\n");
					else if(!strcmp(pilaDatos[s].dato.texto,"clear") || !strcmp(pilaDatos[s].dato.texto,"CLEAR"))
						system("cls");
					else if(!strcmp(pilaDatos[s].dato.texto,"pause") || !strcmp(pilaDatos[s].dato.texto,"PAUSE"))
						system("PAUSE");
					else
						printf("%s",pilaDatos[s].dato.texto);
				}
				else if(pilaDatos[s].tipo==tchr) printf("%c",pilaDatos[s].dato.caracter);
				s--;
				break;
			case 22: // and
				if(controlPrintf)printf(" and ");
				if(pilaDatos[s-1].tipo==tboo && pilaDatos[s].tipo==tboo) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.booleano  && pilaDatos[s].dato.booleano);
				s--;
				break;
			case 23: // or
				if(controlPrintf)printf(" or ");
				if(pilaDatos[s-1].tipo==tboo && pilaDatos[s].tipo==tboo) pilaDatos[s-1].dato.booleano = (pilaDatos[s-1].dato.booleano || pilaDatos[s].dato.booleano);
				s--;
				break;
			case 24: // not
				if(controlPrintf)printf(" not ");
					 if(pilaDatos[s].tipo==tboo && pilaDatos[s].dato.booleano==1) pilaDatos[s].dato.booleano = 0;
				else if(pilaDatos[s].tipo==tboo && pilaDatos[s].dato.booleano==0) pilaDatos[s].dato.booleano = 1;
				break;
			case 25: //in validar bien
			/*	f(OPR)  n d22 t    :leer un dato y colocarlo al tope de la pila. 
									t= tipo de dato*/
				if(controlPrintf)printf("\n in ");
				if(controlPrintf)printf("poner al tope un tipo %d\n",i.t);
				s++;//estoy al tope de la pila incremento en 1
				pilaDatos[s].tipo=traducirAtipo(i.t);
				switch (i.t)
				{
				case tref://busos
					printf("Tipo de dato referencia");
					break;
				case tint:
					scanf("%d",&pilaDatos[s].dato.entero);
					break;
				case tflt:
					scanf("%f",&pilaDatos[s].dato.flotante);
					break;
				case tboo:
					scanf("%s",pilaDatos[s].dato.texto);
					if(!strcmp(pilaDatos[s].dato.texto,"true") || !strcmp(pilaDatos[s].dato.texto,"TRUE")){
						pilaDatos[s].dato.booleano = 1;
					}else if(!strcmp(pilaDatos[s].dato.texto,"false") || !strcmp(pilaDatos[s].dato.texto,"FALSE")){
						pilaDatos[s].dato.booleano = 0;
					}else{
						printf("\nError se esperaba un TRUE o FALSE"); system("PAUSE"); exit(1);
					} 
					break;
				case tstr:
					scanf("%s",pilaDatos[s].dato.texto);
					break;
				case tchr:
					scanf("%c",&pilaDatos[s].dato.caracter);
					break;
				}
				break;
			case 26: //comparar el tope de la pila con el anterior
				if(controlPrintf)printf("Comparando %d con %d",s,s-1);
				if(pilaDatos[s].tipo==pilaDatos[s-1].tipo){
					if(pilaDatos[s].tipo==tint && pilaDatos[s].dato.entero==pilaDatos[s-1].dato.entero){
						pilaDatos[s-1].tipo=tboo;
						pilaDatos[s-1].dato.booleano=1;
					}else if(pilaDatos[s].tipo==tflt && pilaDatos[s].dato.flotante==pilaDatos[s-1].dato.flotante){
						pilaDatos[s-1].tipo=tboo;
						pilaDatos[s-1].dato.booleano=1;
					}else if(pilaDatos[s].tipo==tboo && pilaDatos[s].dato.booleano==pilaDatos[s-1].dato.booleano){
						pilaDatos[s-1].tipo=tboo;
						pilaDatos[s-1].dato.booleano=1;
					}else if(pilaDatos[s].tipo==tchr && pilaDatos[s].dato.caracter==pilaDatos[s-1].dato.caracter){
						pilaDatos[s-1].tipo=tboo;
						pilaDatos[s-1].dato.booleano=1;
					}else if(pilaDatos[s].tipo==tstr && !strcmp(pilaDatos[s].dato.texto,pilaDatos[s-1].dato.texto)){
						pilaDatos[s-1].tipo=tboo;
						pilaDatos[s-1].dato.booleano=1;
					}else{
						pilaDatos[s-1].tipo=tboo;
						pilaDatos[s-1].dato.booleano=0;
					}
				}else{
					pilaDatos[s-1].tipo=tboo;
					pilaDatos[s-1].dato.booleano=0;
				}
				s--;
				break;
			case 27: //Eliminar tope de la pila
				if(controlPrintf)printf("Eliminando tope de la pila");
					s--;
				break;
			case 28: //Leer un archivo 
				if(controlPrintf)printf("Ruta: %s",pilaDatos[s].dato.texto);
					leer(pilaDatos[s].dato.texto);
					s--;
				break;
			case 29: //tamanio de una cadena 
				if(controlPrintf)printf("tamanio de una cadena: %s",pilaDatos[s].dato.texto);
				pilaDatos[s].tipo=tint;
				pilaDatos[s].dato.entero=strlen(pilaDatos[s].dato.texto);
				break;
			}
			break;
		case CAR:
		/*	f(CAR)  n  d* t    :Cargar una variable al tope de la pila. Para el caso de los arreglos 
								cargará la posición que se especifique.*/
			if(i.n==1){
				if(pilaDatos[s].tipo==tint){
					if(pilaDatos[s].dato.entero>=i.t || pilaDatos[s].dato.entero<0){
						printf("\nError se sobrepasa el tamanio de la lista"); system("PAUSE"); exit(1);
					}else{
						i.d=i.d+pilaDatos[s].dato.entero;
						s--;
					}
				}else if(pilaDatos[s].tipo==tflt){
					if(pilaDatos[s].dato.flotante>=i.t || pilaDatos[s].dato.flotante<0){
						printf("\nError se sobrepasa el tamanio de la lista"); system("PAUSE"); exit(1);
					}else{
						i.d=i.d+pilaDatos[s].dato.flotante;
						s--;
					}
				}
			}
			if(pilaDatos[i.d+EEg].tipo!=tref){
				if(controlPrintf)printf("Cargar una variable por valor direccion:%d",i.d);
				pilaDatos[++s]=pilaDatos[i.d+EEg];
			}else{
				if(controlPrintf)printf("Cargar una variable por referencia tipo:%d-direccion:%d",pilaDatos[pilaDatos[i.d+EEg].dato.entero].tipo,i.d);
				pilaDatos[++s]=pilaDatos[pilaDatos[i.d+EEg].dato.entero];
				while(pilaDatos[s].tipo==tref)
					pilaDatos[s]=pilaDatos[pilaDatos[s].dato.entero];
			}
			break;
		case ALM:
		/*	f(ALM)  n* d* t*   :Instrucción de almacenamiento equivalente a la asignación lo que hay al tope de la pila a una direccion.
								d= Direccion del dato 
								n=	0:No es una lista
									1:Es una lista buscar en pila la celda 
								t= Tama;o de una lista*/
			if(controlPrintf)printf("\nALM : almacenando %s en la direccion %d (s en %d)",tipoPilaDatoS[pilaDatos[i.d].tipo],i.d,s);
            bandera2=0;
			if(i.n==1){
				if(pilaDatos[s-1].tipo==tint){
					if(pilaDatos[s-1].dato.entero>=i.t || pilaDatos[s-1].dato.entero<0){
						printf("\nError se sobrepasa el tamanio de la lista"); system("PAUSE"); exit(1);
					}else{
						i.d=i.d+pilaDatos[s-1].dato.entero;
						bandera2=1;
					}
				}else if(pilaDatos[s-1].tipo==tflt){
					if(pilaDatos[s-1].dato.flotante>=i.t || pilaDatos[s-1].dato.flotante<0){
						printf("\nError se sobrepasa el tamanio de la lista"); system("PAUSE"); exit(1);
					}else{
						i.d=i.d+pilaDatos[s-1].dato.flotante;
						bandera2=1;
					}
				}
			}
			if(pilaDatos[i.d+EEg].tipo!=tref){
				if(controlPrintf)printf("Almacenar una variable por valor i.d(%d)+EEg(%d)=",i.d,EEg,i.d+EEg);
				colocarPilaDatosALM(i.d+EEg,pilaDatos[i.d+EEg].tipo,s);
			}else{
				if(controlPrintf)printf("Almacenar una variable por referencia");
				bandera=i.d+EEg;
				while(pilaDatos[pilaDatos[bandera].dato.entero].tipo==tref){
					bandera=pilaDatos[bandera].dato.entero;
					//printf("\n\nReferencias %d",bandera);
				}
				colocarPilaDatosALM(pilaDatos[bandera].dato.entero,pilaDatos[i.d+EEg].tipo,s);
				
			}
			s--;
			if(bandera2==1) s--;
			break;
		case LLA:
		/*   f(LAA)  n  d* t    :Salto incondicional 
								d= Direccion de salto */
			if(controlPrintf)printf("\nLLA : invocando un algoritmo %d (s en %d)",i.d,s);
            DRg=d;	//donde se realizo la llamada
			EDg=s;	//tope de la pila 
			EEg=EEg+i.t;	//variables llevadas hasta el momento
			d=i.d;
			break;
		case INS:
		/*   f(INS)  n* d* t*   :Instrucción de asignación de espacio en la pila, tanto de espacio para variables 
								de control como espacio para variables del programa.
								n= 1: para ED, EE, DR 0: tipear segun la instruccion
								d= Cantidad de celdas
								t= Tipo de variable int, boo etc.*/
			 if(controlPrintf)printf("\nINS : asignando %d espacios en el stack (s en %d)\n",i.d,s+i.d);
             int ix;
			 if(i.n==0){
				 for(ix=0;ix<i.d;ix++){
					s++;
					pilaDatos[s].tipo=traducirAtipo(i.t);
					switch (pilaDatos[s].tipo)
					{
					case tref: pilaDatos[s].dato.entero=0;
						break;
					case tint: pilaDatos[s].dato.entero=0;
						break;
					case tflt: pilaDatos[s].dato.flotante=0;
						break;
					case tboo: pilaDatos[s].dato.booleano=0;
						break;
					case tchr: pilaDatos[s].dato.caracter=' ';
						break;
					case tstr: strcpy(pilaDatos[s].dato.texto,"");
						break;
					}
				 }
			 }else{
				pilaDatos[++s].tipo=DR;	//direccion de donde fue realizada el call
				pilaDatos[s].dato.entero=DRg;
				pilaDatos[++s].tipo=ED;	//direccion del stack tope cuando se hizo la llamada
				pilaDatos[s].dato.entero=EDg;
				pilaDatos[++s].tipo=EE;	//cantidad de parametros
				pilaDatos[s].dato.entero=EEg;
			 }
			break;
		case SAL:
		/*   f(SAL)  n  d* t    :Salto incondicional 
								d= Direccion de salto */
			if(controlPrintf)printf("\nSAL : saltando incondicionalmente a la instruccion %d (s en %d)",i.d,s);
            d=i.d;
			break;

		case SAC:
		/*	f(SAC)  n  d* t 	:Selecciona lo que hay en el tope de la pila y si es true(1) no salta si el false si(0)	
								d= Direccion del salto*/
			if(pilaDatos[s].dato.booleano==0){
				if(controlPrintf)printf("\nSAC : Falso asi que salto a %d (s en %d) ",i.d,s);
				d=i.d;
			}else{ if(controlPrintf)printf("\nSAC : Verdadero no saltare %d (s en %d) ",i.d,s);
			}
			s--;
			break;

		case RET:
			if(i.n==1){//1:Traslada la candidad de datos del tope de la pila hasta 'd' a un pila de referencia d= #numero de celdas a trasladar
				iret=i.d;
				if(controlPrintf)printf("	ret t1 :%d\n",iret);
				for(bandera=0;bandera<iret;bandera++){
					//printf("posiciones(%d)",s-bandera);
					ret[bandera].tipo=pilaDatos[s].tipo;
						 if(pilaDatos[s].tipo==tint || pilaDatos[s].tipo==tref) ret[bandera].dato.entero=pilaDatos[s].dato.entero;
					else if(pilaDatos[s].tipo==tflt) ret[bandera].dato.flotante=pilaDatos[s].dato.flotante;
					else if(pilaDatos[s].tipo==tboo) ret[bandera].dato.booleano=pilaDatos[s].dato.booleano;
					else if(pilaDatos[s].tipo==tchr) ret[bandera].dato.caracter=pilaDatos[s].dato.caracter;
					else if(pilaDatos[s].tipo==tchr) strcpy(ret[bandera].dato.texto,pilaDatos[s].dato.texto);
					s--;
				}
				//listar_pilaRet(-1,iret-1);
				
			}else if(i.n==0){//0:Crear una celda en la pila de datos para link d= indicia la cantidad de celdas t= inidica que sera link cuando es 0
				if(controlPrintf)printf("	ret t0 \n");
				for(bandera=0;bandera<i.d;bandera++)
					colocarPilaDatos(++s,i,EEg);
			}else if(i.n==2){//2:Traslada las celdas de la pila de referencia a la pila de datos en los respectivos parametros d= #numero de celdas a trasladar				
				iret=i.d-1;
				//listar_pilaRet(-1,i.d);
				if(controlPrintf)printf("	ret t2 :%d\n",iret);
				bandera=s;
				for(iret=0;iret<i.d;iret++){
					////printf("pilaDatos[%d]=ret[%d]",bandera,iret);
					//pilaDatos[bandera].tipo=ret[s].tipo;
						 if(pilaDatos[bandera].tipo==tint || pilaDatos[bandera].tipo==tref) pilaDatos[bandera].dato.entero=ret[iret].dato.entero;
					else if(pilaDatos[bandera].tipo==tflt) pilaDatos[bandera].dato.flotante=ret[iret].dato.flotante;
					else if(pilaDatos[bandera].tipo==tboo) pilaDatos[bandera].dato.booleano=ret[iret].dato.booleano;
					else if(pilaDatos[bandera].tipo==tchr) pilaDatos[bandera].dato.caracter=ret[iret].dato.caracter;
					else if(pilaDatos[bandera].tipo==tchr) strcpy(pilaDatos[bandera].dato.texto,ret[iret].dato.texto);
					bandera--;
				}
				if(controlPrintf)listar_pilaRet(-1,iret-1);
			}
			break;
	 }
	 if(controlPrintf)listar_pilaDatos(-1,s);
	 if(controlPrintf)getch(); 
 } while (d!=0);
 //printf("Salio");
}

void listar_pilaDatos(int ix,int topPila){
	printf("\nPila de datos Stack\n");
	if(ix==-1){
		for(ix=0;ix<=topPila;ix++){
			if(pilaDatos[ix].tipo==tref || pilaDatos[ix].tipo==tint || pilaDatos[ix].tipo==EE || pilaDatos[ix].tipo==DR || pilaDatos[ix].tipo==ED)
				printf("%d) %d de tipo %s\n",ix,pilaDatos[ix].dato.entero,tipoPilaDatoS[pilaDatos[ix].tipo]);
			else if(pilaDatos[ix].tipo==tflt)
				printf("%d) %f de tipo %s\n",ix,pilaDatos[ix].dato.flotante,tipoPilaDatoS[pilaDatos[ix].tipo]);
			else if(pilaDatos[ix].tipo==tboo)
				printf("%d) %d de tipo %s\n",ix,pilaDatos[ix].dato.booleano,tipoPilaDatoS[pilaDatos[ix].tipo]);
			else if(pilaDatos[ix].tipo==tchr)
				printf("%d) %c de tipo %s\n",ix,pilaDatos[ix].dato.caracter,tipoPilaDatoS[pilaDatos[ix].tipo]);
			else if(pilaDatos[ix].tipo==tstr)
				printf("%d) %s de tipo %s\n",ix,pilaDatos[ix].dato.texto,tipoPilaDatoS[pilaDatos[ix].tipo]);
		}
	}else{
		if(pilaDatos[ix].tipo==tref || pilaDatos[ix].tipo==tint || pilaDatos[ix].tipo==EE || pilaDatos[ix].tipo==DR || pilaDatos[ix].tipo==ED)
			printf("%d) %d de tipo %s\n",ix,pilaDatos[ix].dato.entero,tipoPilaDatoS[pilaDatos[ix].tipo]);
		else if(pilaDatos[ix].tipo==tflt)
			printf("%d) %f de tipo %s\n",ix,pilaDatos[ix].dato.flotante,tipoPilaDatoS[pilaDatos[ix].tipo]);
		else if(pilaDatos[ix].tipo==tboo)
			printf("%d) %d de tipo %s\n",ix,pilaDatos[ix].dato.booleano,tipoPilaDatoS[pilaDatos[ix].tipo]);
		else if(pilaDatos[ix].tipo==tchr)
			printf("%d) %c de tipo %s\n",ix,pilaDatos[ix].dato.caracter,tipoPilaDatoS[pilaDatos[ix].tipo]);
		else if(pilaDatos[ix].tipo==tstr)
			printf("%d) %s de tipo %s\n",ix,pilaDatos[ix].dato.texto,tipoPilaDatoS[pilaDatos[ix].tipo]);
	}
}

void listar_pilaRet(int ix,int topPila){
	printf("\nPila de datos RET\n");
	if(ix==-1){
		for(ix=0;ix<=topPila;ix++){
			if(ret[ix].tipo==tref || ret[ix].tipo==tint || ret[ix].tipo==EE || ret[ix].tipo==DR || ret[ix].tipo==ED)
				printf("%d) %d de tipo %s\n",ix,ret[ix].dato.entero,tipoPilaDatoS[ret[ix].tipo]);
			else if(ret[ix].tipo==tflt)
				printf("%d) %f de tipo %s\n",ix,ret[ix].dato.flotante,tipoPilaDatoS[ret[ix].tipo]);
			else if(ret[ix].tipo==tboo)
				printf("%d) %d de tipo %s\n",ix,ret[ix].dato.booleano,tipoPilaDatoS[ret[ix].tipo]);
			else if(ret[ix].tipo==tchr)
				printf("%d) %c de tipo %s\n",ix,ret[ix].dato.caracter,tipoPilaDatoS[ret[ix].tipo]);
			else if(ret[ix].tipo==tstr)
				printf("%d) %s de tipo %s\n",ix,ret[ix].dato.texto,tipoPilaDatoS[ret[ix].tipo]);
		}
	}else{
		if(ret[ix].tipo==tref || ret[ix].tipo==tint || ret[ix].tipo==EE || ret[ix].tipo==DR || ret[ix].tipo==ED)
			printf("%d) %d de tipo %s\n",ix,ret[ix].dato.entero,tipoPilaDatoS[ret[ix].tipo]);
		else if(ret[ix].tipo==tflt)
			printf("%d) %f de tipo %s\n",ix,ret[ix].dato.flotante,tipoPilaDatoS[ret[ix].tipo]);
		else if(ret[ix].tipo==tboo)
			printf("%d) %d de tipo %s\n",ix,ret[ix].dato.booleano,tipoPilaDatoS[ret[ix].tipo]);
		else if(ret[ix].tipo==tchr)
			printf("%d) %c de tipo %s\n",ix,ret[ix].dato.caracter,tipoPilaDatoS[ret[ix].tipo]);
		else if(ret[ix].tipo==tstr)
			printf("%d) %s de tipo %s\n",ix,ret[ix].dato.texto,tipoPilaDatoS[ret[ix].tipo]);
	}
}

void colocarPilaDatos(int posicion,codigo_intermedio i,int EEg){
	pilaDatos[posicion].tipo=traducirAtipo(i.t);
	switch (i.t)
	{
		case 0: pilaDatos[posicion].dato.entero      =i.dato.entero+EEg;break;
		case 1: pilaDatos[posicion].dato.entero      =i.dato.entero;break;
		case 2: pilaDatos[posicion].dato.flotante    =i.dato.flotante;break;
		case 3: pilaDatos[posicion].dato.booleano    =i.dato.booleano;break;
		case 4: strcpy(pilaDatos[posicion].dato.texto,i.dato.texto);break;
		case 5: pilaDatos[posicion].dato.caracter    =i.dato.caracter;break;
	}
}

void colocarPilaDatosALM(int posicion,tipoPilaDato tipo,int topePila){
	//printf("\n tipo: %d dato tipo-> %d en %d",tipo,pilaDatos[topePila].tipo,posicion);
	//system("PAUSE");
	switch (tipo)
	{
		case 0: pilaDatos[posicion].dato.entero=pilaDatos[topePila].dato.entero;
			break;
		case 1: if(pilaDatos[topePila].tipo==tint)
					pilaDatos[posicion].dato.entero=pilaDatos[topePila].dato.entero;
				else if(pilaDatos[topePila].tipo==tflt)
					pilaDatos[posicion].dato.entero=pilaDatos[topePila].dato.flotante;
				break;
		case 2: if(pilaDatos[topePila].tipo==tint)
					pilaDatos[posicion].dato.flotante=pilaDatos[topePila].dato.entero;
				else if(pilaDatos[topePila].tipo==tflt)
					pilaDatos[posicion].dato.flotante=pilaDatos[topePila].dato.flotante;
				break;
		case 3: pilaDatos[posicion].dato.booleano=pilaDatos[topePila].dato.booleano;break;
		case 4: strcpy(pilaDatos[posicion].dato.texto,pilaDatos[topePila].dato.texto);break;
		case 5: pilaDatos[posicion].dato.caracter=pilaDatos[topePila].dato.caracter;break;
	}
}

tipoPilaDato traducirAtipo(int t){
	switch(t){
	case 0: return tref;break;
	case 1: return tint;break;
	case 2: return tflt;break;
	case 3: return tboo;break;
	case 4: return tstr;break;
	case 5: return tchr;break;
	case 6: return ED;break;
	case 7: return EE;break;
	case 8: return DR;break;
	}
} 

//listar_p: emitir listado de código-p
void listar_p()
{
 int i;
 char *comentario[]={";cargar una constante",";operacion ",";cargar una variable",
                     ";almacenamiento/instruccion de asignacion",";llamada a procedimiento",";asignacion de espacio de memoria",
                     ";salto incondicional",";salto condicional"," Ret "};
 char *operacion[]={" Retornar "," Menos unario "," Sumar "," Restar "," Multiplicar "," Dividir ",
					  " odd "," even "," == "," != "," < "," <= "," > "," >= "," Max "," Mim ", " abs ",
					  " pow "," reverse ", " upp ", " append "," out " 
					  };
 char *tipo_dato[]={"NULL","INT","FLT","BOO","STR","CHR"};
 char *Funcion[]={"0) Crear una celda en la pila de datos para link","1) Traslada del tope de la pila a un pila de referencia", "2) Traslada de la pila de referencia a la pila de datos"};

 printf("\n\n --Listado de codigo-p simplificado generado por el compilador--\n\n");
  
 for(i=0;i<ic;++i) {
     printf("\n %4d- F (%3s)\t N= %3d D= %3d T= %3d   \t",i,mnemonico[codigo[i].f],codigo[i].n,codigo[i].d,codigo[i].t);
 }
}

void InvertirCadena(char * cadena){
 // Longitud de la cadena
 int len = strlen(cadena);
 // invertir cadena
 int m=0, n=len;
 char tem;
 while(n>m){
  tem = cadena[m];
  cadena[m++] = cadena[--n];
  cadena[n] = tem;
 }
}

void uppCadena(char * cadena){
 // Longitud de la cadena
 int n = strlen(cadena);
 // invertir cadena
 int m=0;
 
 while(m<n){
  cadena[m] = toupper(cadena[m]);
  m++;
 }
}

void leer(char *nombre){
	FILE *file;
	char c;
    file = fopen(nombre,"r");
    if(file == NULL)
    {
        printf("Error al abrir archivo");
    }else{
		while((c=fgetc(file))!=EOF){
			printf("%c",c);
		}
		fclose(file);
	}
	
}
