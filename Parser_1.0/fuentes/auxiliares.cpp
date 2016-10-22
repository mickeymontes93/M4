//funciones auxiliares del compilador
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "pl0.h"
#include "auxiliares.h"
#include "lexico.h"

int MAXLINEA =  1000;  //tamaño máximo de una línea del programa fuente
int MAXDIGIT =  5;  //máximo número de dígitos en los enteros
int MAXID    =  200;  //máxima longitud de los identificadores
char *linea;     //buffer de líneas
char *lex;        //último lexeme leído ( +1 para colocar "\0")
int cantElementosSplit;

//error: por el momento todo error es fatal
int error(int no) {
  fclose(fp);//cerrar el programa fuente
  printf ("\n^ error %d: %s\n",no,mensaje_de_error[no]);
  exit(1); //estoy en modo de pánico...cualquier error es fatal
}

//inicializar_espec
//construcción de la tabla de tokens para operadores y símboles especiales de pl0
void inicializar_espec() {
  int i;
  for (i = 0; i <= 254; ++i)
    espec[i] = tok_null;

  espec[38] = tok_amp;
  espec[40] = tok_parena;
  espec[41] = tok_parenc;
  espec[42] = tok_multi;
  espec[43] = tok_sum;
  espec[44] = tok_coma;
  espec[45] = tok_resta;
  espec[47] = tok_divi;
  espec[59] = tok_finlinea;
  espec[91] = tok_corcha;
  espec[92] = tok_corchc;
  espec[123] = tok_llavea;
  espec[125] = tok_llavec;
}

//imprime_token: transforma de enumerado a string. no aparecerá más en el compilador
void imprime_token() {

  char *token_string[] = { "tok_null", "tok_id", "tok_numero", "tok_flotante", "tok_caracter", "tok_cadena", "tok_boolean", "tok_amp", "tok_llavea", "tok_llavec",
                           "tok_parena", "tok_parenc", "tok_corcha", "tok_corchc", "tok_finlinea", "tok_punto", "tok_coma", "tok_sum", "tok_resta", "tok_multi",
                           "tok_divi", "tok_asignar", "tok_sumasign", "tok_mayor", "tok_menor", "tok_mayorigual", "tok_menorigual", "tok_igual", "tok_negacion",
                           "tok_and", "tok_or", "tok_varint", "tok_varfloat", "tok_varchar", "tok_varstr", "tok_varbool", "tok_if", "tok_else", "tok_switch",
                           "tok_case", "tok_default", "tok_break", "tok_while", "tok_do", "tok_for", "tok_write", "tok_read", "tok_funcint", "tok_funcfloat",
                           "tok_funcchar", "tok_funcstr", "tok_bool", "tok_funcvoid", "tok_arrEntero", "tok_arrFlotante", "tok_arrCaracter", "tok_arrCadena",
                           "tok_arrBooleano", "tok_main", "tok_file", "tok_fileopen", "tok_fscanf", "tok_fgets", "tok_fclose", "tok_split", "tok_trim", "tok_round",
                           "tok_sqrt", "tok_substring", "tok_pow", "tok_length", "tok_concat", "tok_replace", "tok_equal", "tok_sin", "tok_cos", "tok_tan", "tok_arcsin",
                           "tok_arccos", "tok_arctan", "tok_log", "tok_true", "tok_false", "", "tok_return"
                         };

  if (token != COMENTARIO)
    printf("\t -> %10s", token_string[token]);

}


void parametrizarScanner() {
  char* archivo = leerArchivo("param.txt");
  if (archivo != NULL) {
    char** lineas = str_split(archivo, '\n');
    int cantLineas = cantElementosSplit;
    for (int i = 0; i < cantLineas; i++) {
      char** parametro = str_split(*(lineas + i), ';');
      int cant = cantElementosSplit;
      if (cant == 2) {
        if (strcmp(*parametro, "MAXLINEA") == 0) {
          MAXLINEA = atoi(*(parametro + 1));
        } else {
          if (strcmp(*parametro, "MAXDIGIT") == 0) {
            MAXDIGIT = atoi(*(parametro + 1));
          } else {
            if (strcmp(*parametro, "MAXID") == 0) {
              MAXID = atoi(*(parametro + 1));
            }
          }
        }
      }
    }
  } else {
    FILE *file = fopen("param.txt", "w");
    fprintf(file, "MAXLINEA;%d\n", MAXLINEA);
    fprintf(file, "MAXDIGIT;%d\n", MAXDIGIT);
    fprintf(file, "MAXID;%d", MAXID);
    fclose(file);
  }

  printf("MAXLINEA: %d\n", MAXLINEA);
  printf("MAXDIGIT: %d\n", MAXDIGIT);
  printf("MAXID: %d\n", MAXID);

  linea = (char*) malloc(MAXLINEA * (sizeof(char)));
  lex = (char*) malloc((MAXID + 1) * (sizeof(char)));
}


char* leerArchivo(char *filename) {
  char *file_contents;
  char car;
  long input_file_size, i = 0, cont = 0;
  FILE *input_file = fopen(filename, "r");

  if ( input_file == NULL )  {
    //perror("Archivo no encontrado.\n");
    return NULL;
  }

  fseek(input_file, 0, SEEK_END);
  input_file_size = ftell(input_file);
  rewind(input_file);
  file_contents =  (char*) malloc((input_file_size + 1) * (sizeof(char)));

  while ( ( car = fgetc(input_file) ) != EOF ) {
    if (car != '\r') {
      file_contents[i] = car;
      i++;
    }
  }

  fclose(input_file);
  file_contents[i] = '\0';
  return file_contents;
}


char** str_split(char* a_str, const char a_delim) {
  char** result    = 0;
  size_t count     = 0;
  char* tmp        = a_str;
  char* last_comma = 0;
  char delim[2];
  delim[0] = a_delim;
  delim[1] = 0;

  /* Count how many elements will be extracted. */
  while (*tmp) {
    if (a_delim == *tmp) {
      count++;
      last_comma = tmp;
    }
    tmp++;
  }

  /* Add space for trailing token. */
  count += last_comma < (a_str + strlen(a_str) - 1);
  cantElementosSplit = count;
  /* Add space for terminating null string so caller
     knows where the list of returned strings ends. */
  count++;

  result = (char**) malloc(sizeof(char*) * count);

  if (result) {
    size_t idx  = 0;
    char* token = strtok(a_str, delim);

    while (token) {
      assert(idx < count);
      *(result + idx++) = strdup(token);
      token = strtok(NULL, delim);
    }
    assert(idx == count - 1);
    *(result + idx) = 0;
  }

  return result;
}
