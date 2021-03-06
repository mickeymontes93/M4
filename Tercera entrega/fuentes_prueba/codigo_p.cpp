#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pl0.h"
#include "codigo_p.h"
#include "auxiliares.h"

codigo_intermedio codigo[MAXIC]; //array con las instrucciones de c�digo-p
int ic;                          //�ndice sobre el array de c�digo

//gen
//coloca una instrucci�n m�s en el array de c�digo intermedio
//ic se deja al final una posici�n adelante.
void gen (enum fcn f, int nivel,  tipo_dato tipoDato, valorPorTipo valor)
{
  if (ic > MAXIC - 1) {
    error(57); //error fatal: no queda m�s que terminar la compilaci�n por una limitaci�n espec�fica del compilador
    //ya no hay m�s espacio para el c�digo-p
    estadisticas();
    fclose(fp);
    exit(1);   // el error es fatal=exit
  }

//generar una instrucci�n de c�digo-p
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

//listar_p: emitir listado de c�digo-p
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

//escribe_codigop: escribe un archivo con extensi�n .p conteniendo el c�digo-p generado
void escribe_codigop(char *fuente)
{
//construir nombre de archivo con el mismo nombre del fuente pero con extensi�n .p

  FILE *obj;
  char *ptr;
  char codigo_p[TAMANO_MAX_NOMBRE_DEL_ARCHIVO_CODIGO_P];
  int pos, i;

//buscar un punto en el nombre del archivo fuente
//en ptr queda la posici�n que ocupa el caracter a buscar
  ptr = strchr(fuente, '.');
  if (ptr) {
    //si el apuntador es distinto de NULL,hay un punto en el nombre del fuente
    //encontramos la posici�n del punto, relativa al inicio del string
    pos = ptr - fuente;

    //dejamos en "codigo_p" solo el nombre del archivo (sin el punto ni la extensi�n)
    strncpy(codigo_p, fuente, pos);
    codigo_p[pos] = '\0';
  }
  else
    strcpy(codigo_p, fuente); //el fuente no tiene apellido

//finalmente,colocamos la extensi�n deseada
  strcat(codigo_p, ".p");

//grabar en el disco el archivo con c�digo_p,a partir del array c�digo creado por el generador de c�digo intermedio
  if  ( (obj = fopen(codigo_p, "w+b")) == NULL ) {
    printf("\nOcurrio un error al intentar escribir el c�digo intermedio (c�digo-p)"); //error fatal.
    fclose(fp);
    exit(1);
  }

  for (i = 0; i < ic; ++i)
    fwrite(&codigo[i], sizeof(codigo_intermedio), 1, obj);

  fclose(obj);
}

