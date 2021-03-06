//m�quina virtual (int�rprete-ejecutor) de c�digo-p simplificado
//se usa una m�quina de stack para realizar la ejecuci�n del c�digo
//la implementaci�n del stack est� desguarnecida: no se ha implementado 
//ning�n nivel de abstracci�n ni de validaci�n
//noviembre 2012,abril de 2011
//se us� Microsoft Visual C++ 2010 para escribir esta mv

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAXIC     200  //tama�o m�ximo del array de c�digo-p
#define LONGSTACK 500  //tama�o-longitud del stack de datos                    

//instrucciones(mnem�nicos) del c�digo-p
enum fcn {LIT,OPR,CAR,ALM,LLA,INS,SAL,SAC};

typedef struct {
 enum fcn f; //mnem�nico
 int     ni; //nivel (0..MAXNIV)
 int     di; //direcci�n o desplazamiento (0..32767)
} codigo_intermedio;

codigo_intermedio codigo[MAXIC]; //array con las instrucciones de codigo-p
int ic;                          //�ndice sobre el array de c�digo-p

FILE *obj; //apuntador al archivo de c�digo intermedio

int p[LONGSTACK] ; //memoria de datos-stack

//arrays para mostrar como string el mnem�nico de la instrucci�n
char *mnemonico[]={"LIT","OPR","CAR","ALM","LLA","INS","SAL","SAC"};

//comentarios para una instrucci�n de codigo-p
 char *comentario[]={";cargar una constante",";operacion aritmetica, relacional o retornar",";cargar una variable",
                     ";almacenamiento/instruccion de asignacion",";llamada a procedimiento",";asignacion de espacio de memoria",
                     ";salto incondicional",";salto condicional"};

//prototipos de funciones
void interpretar(void),listar_p(void);
int base(int ni,int b);

//main: inicia la ejecuci�n
int main(int argc,char *argv[]) {
 if (argc!=2) //analizar argumentos de main
    printf("\nuso: pl0mv nombre_de_archivo");
 else  
    if ( (obj=fopen(argv[1],"r+b"))==NULL ) 
       printf("\nerror al abrir archivo %s",argv[1]);
	else {
       //leer el c�digo desde el archivo hacia un array en memoria
       ic=0;
       fread(&codigo[ic],sizeof(codigo_intermedio),1,obj);
       while ( !feof(obj) ) {
             ++ic;
             fread(&codigo[ic],sizeof(codigo_intermedio),1,obj);
       }
	   //cierra el archivo
       fclose(obj);

       listar_p(); //quita el comentario de esta l�nea si deseas verificar que el c�digo se cargo al array correctamente

	   //interpreta el codigo-p 
	   interpretar();   	
    }
   return(0);
 }

//interpretar: interpreta c�digo-p                
void interpretar(void) {

 register int d;       //registro de direcci�n del programa: apunta a la siguiente instrucci�n por ejecutar
 register int b;       //registro de direcci�n base
 register int s;       //apuntador al tope del stack
 codigo_intermedio i;  //registro de instrucci�n: contiene la siguiente instrucci�n a ejecutar		     
			
 s=-1;b=0;d=0;
 p[0]=p[1]=p[2]=0;   //ED,EE y DR para el programa principal
 
 //bucle de ejecuci�n
 do {
  
    i=codigo[d++];   
    printf("\n\nejecutando la instruccion %4d: %3s %5d %5d",d-1,mnemonico[i.f],i.ni,i.di); 
   
    switch(i.f) {
          case LIT:
               p[++s]=i.di;
	           printf("\nLIT : cargando la literal %d en la direccion %d (s en %d)",i.di,s,s);
               break;

          case OPR:
               printf("\nOPR : ");
			   //determinar de que operador se trata
               switch(i.di) {  
			       case 0: //retornar o fin
                        s=--b;
                        d=p[s+3];
                        b=p[s+2];
	                    printf("retornar a la instruccion %d, base=%d (s en %d)",d,b,s);
                        break;

                   case 1: //----------> MENOS UNARIO
	                    printf("- unario para %d (s en %d)",p[s],s);
                        p[s]=-p[s];
                        break;

                   case 2: //----------> SUMA
                        --s;
	                    printf("suma de %d + %d (s en %d)",p[s],p[s+1],s);
                        p[s]=p[s]+p[s+1];
	                    break;

                   case 3:
	                    --s; //----------> RESTA
                        printf("resta de %d - %d (s en %d)",p[s],p[s+1],s);
                        p[s]=p[s]-p[s+1];
                        break;

                   case 4: //----------> MULTIPLICACION
	                    --s;
                        printf("multiplicacion de %d * %d (s en %d)",p[s],p[s+1],s);
                        p[s]=p[s]*p[s+1];
                        break;

                   case 5: //---------->  DIVISION
                        --s;
	                    printf("division entera de %d / %d (s en %d)",p[s],p[s+1],s);
                        if(p[s+1]==0) {
						  fclose(obj);
	                      printf("\nError a tiempo de ejecucion\nSe intenta dividir entre cero");
	                      printf("\n--Programa abortado--");
	                      exit(1); //el error es fatal
                        }
                        p[s]=p[s]/p[s+1];
                        break;

                   case 6: //----------> ODD
                        printf("odd(%d)? (s en %d)",p[s],s);
                        p[s]=(p[s]%2!=0);
       
                   case 7: //sin uso
                        break;

                   case 8: //----------> IGUAL (==)
	                    --s;
                        printf("%d=%d? (s en %d)",p[s],p[s+1],s);
                        p[s]=(p[s]==p[s+1]);
                        break;

                   case 9: //----------> DIFERENTE DE (!=)
                        --s;
                        printf("%d!=%d? (s en %d)",p[s],p[s+1],s);
                        p[s]=(p[s]!=p[s+1]);
                        break;

                   case 10: //----------> MENOR QUE
                        --s;
                        printf("%d<%d? (s en %d)",p[s],p[s+1],s);
                        p[s]=(p[s]<p[s+1]);
                        break;

                   case 11: //----------> MAYOR O IGUAL QUE
                        --s;
                        printf("%d>=%d? (s en %d)",p[s],p[s+1],s);
                        p[s]=(p[s]>=p[s+1]);
                        break;

                   case 12: //----------> MAYOR QUE
                        --s; 
                        printf("%d>%d? (s en %d)",p[s],p[s+1],s);
                        p[s]=(p[s]>p[s+1]);
                        break;

                   case 13: //----------> MENOR O IGUAL QUE
	                    --s;
                        printf("%d<=%d? (s en %d)",p[s],p[s+1],s);
                        p[s]=(p[s]<=p[s+1]);
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
                        printf("squrt( %d )? (s en %d)",p[s],s);
                        p[s]=sqrt(p[s]);
                        break;
                    case 21: //----------> POW
                        --s;
                        printf("pow( %d, %d )? (s en %d) ",p[s],p[s+1],s);
                        p[s]=pow(p[s],p[s+1]);
                        break;
                    case 22: //----------> SIN
                        printf("sin( %d )? (s en %d)", p[s],s);
                        p[s]=sin(p[s]);
                        break;
                    case 23: //----------> COS
                        printf("cos( %d )? (s en %d)", p[s],s);
                        p[s]=cos(p[s]);
                        break;
                    case 24: //----------> TAN
                        printf("tan( %d )? (s en %d)", p[s],s);
                        p[s]=tan(p[s]);
                        break;
                    case 25: //----------> LOG 
                        printf("log( %d )? (s en %d)", p[s],s);
                        p[s]=log(p[s]);
                        break;
                    case 26: //---------->ARCSIN
                        printf("arctan( %d )? (s en %d)", p[s],s);
                        p[s]=asin(p[s]);
                        break;
                    case 27: //---------->ARCCOS
                        printf("arccos( %d )? (s en %d)", p[s],s);
                        p[s]=acos(p[s]);
                        break;
                    case 28: //---------->ARCTAN
                        printf("arctan( %d )? (s en %d)", p[s],s);
                        p[s]=atan(p[s]);
                        break;
                    case 29: //---------->ROUND
                        printf("round( %d )? (s en %d)", p[s],s);
                        p[s]=round(p[s]);
                        break;
                    case 30: //----------> += 
                        --s;
                        printf("suma igual de %d + %d (s en %d)",p[s],p[s+1],s);
                        p[s]=p[s]+p[s+1];
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
               p[++s]=p[base(i.ni,b)+i.di];
	           printf("\nCAR : cargando %d en la direccion %d (s en %d)",p[base(i.ni,b)+i.di],s,s);
               break;

          case ALM:
               printf("\nALM : almacenando %d en la direccion %d (s en %d)",p[s],base(i.ni,b)+i.di,s-1);
               p[base(i.ni,b)+i.di]=p[s];
               --s;
               break;

          case LLA:
               //generar un nuevo bloque
               p[s+1]=base(i.ni,b);
               p[s+2]=b;
               p[s+3]=d;
               printf("\nLLA : activando subrutina, enlaces y DR: %d %d %d",p[s+1],p[s+2],p[s+3]);

               b=s+1;d=i.di;
               printf("\n    : nueva base %d, instruccion %d (s en %d)",b,d,s);
               break;

          case INS:
               printf("\nINS : asignando %d espacios en el stack (s en %d)",i.di,s+i.di);
               s+=i.di;
               break;

          case SAL:
               printf("\nSAL : saltando incondicionalmente a la instruccion %d (s en %d)",i.di,s);
               d=i.di;
               break;

          case SAC:
               printf("\nSAC : ");
               if (p[s]==0) {
                  d=i.di;
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
 
//base: encuentra la base ni niveles m�s abajo en el stack para la operaci�n actual (CAR,ALM,LLA)
int base(int ni,int b)
{
 int b1;
 b1=b;
 while (ni>0) {
       b1=p[b1];
       --ni;
 }
 return (b1);
}

//listar_p: emitir listado de c�digo-p
void listar_p()
{
 int i;
   
 printf("\n\n --Listado de codigo-p simplificado generado por el compilador--\n\n");
  
 for(i=0;i<ic;++i) {
     printf("\n %4d  %3s %5d %5d %s",i,mnemonico[codigo[i].f],codigo[i].ni,codigo[i].di,comentario[codigo[i].f]);
 }
}


