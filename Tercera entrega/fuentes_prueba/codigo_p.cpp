#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pl0.h"
#include "codigo_p.h"
#include "auxiliares.h"

codigo_intermedio codigo[MAXIC]; //array con las instrucciones de código-p
int ic;                          //índice sobre el array de código

//gen
//coloca una instrucción más en el array de código intermedio
//ic se deja al final una posición adelante.
void gen (enum fcn f, int nivel,  tipo_dato tipoDato, valorPorTipo valor)
{
  if (ic > MAXIC - 1) {
    error(57); //error fatal: no queda más que terminar la compilación por una limitación específica del compilador
    //ya no hay más espacio para el código-p
    estadisticas();
    fclose(fp);
    exit(1);   // el error es fatal=exit
  }

//generar una instrucción de código-p
  codigo[ic].f  = f;
  codigo[ic].ni = nivel;
  codigo[ic].t = tipoDato;
  codigo[ic].Dato = valor;

  switch (tipoDato) {
  case TIPO_ENTERO:
    codigo[ic].Dato.entero  = valor.entero;
    break;
  case TIPO_FLOAT:
    codigo[ic].Dato.flotante = valor.flotante;
    break;
  case TIPO_CADENA:
    strcpy(codigo[ic].Dato.cadena, valor.cadena);
    break;
  case TIPO_CARACTER:
    codigo[ic].Dato.caracter = valor.caracter;
    break;
  case TIPO_BOOLEAN:
    codigo[ic].Dato.booleano = valor.booleano;
    break;
  }

  ++ic;
}

//listar_p: emitir listado de código-p
void listar_p()
{
  int i;

  char *mnemonico[] = {"LIT", "OPR", "CAR", "ALM", "LLA", "INS", "SAL", "SAC"};
  char *comentario[] = {";cargar una constante", ";operacion aritmetica, relacional o retornar", ";cargar una variable",
                        ";almacenamiento/instruccion de asignacion", ";llamada a procedimiento", ";asignacion de espacio de memoria",
                        ";salto incondicional", ";salto condicional"
                       };

  printf("\n\n --Listado de codigo-p simplificado generado por el compilador--\n\n");

  for (i = 0; i < ic; ++i) {
    switch (codigo[i].t) {
    case TIPO_ENTERO:
      printf("\n %4d  %3s %5d %5d %s", i, mnemonico[codigo[i].f], codigo[i].ni, codigo[i].Dato.entero, comentario[codigo[i].f]);
      break;
    case TIPO_FLOAT:
      printf("\n %4d  %3s %5d %2f %s", i, mnemonico[codigo[i].f], codigo[i].ni, codigo[i].Dato.flotante, comentario[codigo[i].f]);
      break;
    case TIPO_CADENA:
      printf("\n %4d  %3s %5d %s %s", i, mnemonico[codigo[i].f], codigo[i].ni, codigo[i].Dato.cadena, comentario[codigo[i].f]);
      break;
    case TIPO_CARACTER:
      printf("\n %4d  %3s %5d %c %s", i, mnemonico[codigo[i].f], codigo[i].ni, codigo[i].Dato.caracter, comentario[codigo[i].f]);
      break;
    case TIPO_BOOLEAN:
      printf("\n %4d  %3s %5d %5d %s", i, mnemonico[codigo[i].f], codigo[i].ni, codigo[i].Dato.booleano, comentario[codigo[i].f]);
      break;
    }


  }
}

//escribe_codigop: escribe un archivo con extensión .p conteniendo el código-p generado
void escribe_codigop(char *fuente)
{
//construir nombre de archivo con el mismo nombre del fuente pero con extensión .p

  FILE *obj;
  char *ptr;
  char codigo_p[TAMANO_MAX_NOMBRE_DEL_ARCHIVO_CODIGO_P];
  int pos, i;

//buscar un punto en el nombre del archivo fuente
//en ptr queda la posición que ocupa el caracter a buscar
  ptr = strchr(fuente, '.');
  if (ptr) {
    //si el apuntador es distinto de NULL,hay un punto en el nombre del fuente
    //encontramos la posición del punto, relativa al inicio del string
    pos = ptr - fuente;

    //dejamos en "codigo_p" solo el nombre del archivo (sin el punto ni la extensión)
    strncpy(codigo_p, fuente, pos);
    codigo_p[pos] = '\0';
  }
  else
    strcpy(codigo_p, fuente); //el fuente no tiene apellido

//finalmente,colocamos la extensión deseada
  strcat(codigo_p, ".p");

//grabar en el disco el archivo con código_p,a partir del array código creado por el generador de código intermedio
  if  ( (obj = fopen(codigo_p, "w+b")) == NULL ) {
    printf("\nOcurrio un error al intentar escribir el código intermedio (código-p)"); //error fatal.
    fclose(fp);
    exit(1);
  }

  for (i = 0; i < ic; ++i)
    fwrite(&codigo[i], sizeof(codigo_intermedio), 1, obj);

  fclose(obj);
}

