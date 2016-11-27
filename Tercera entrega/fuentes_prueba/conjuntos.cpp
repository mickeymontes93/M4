#include "stdio.h"
#include "conjuntos.h"
#include "parametros.h"
#include "lexico.h"
#include "auxiliares.h"
#include "scanner.h"

//conjuntos de tokens para manejo de errores         
//tokens iniciales de declaración, de instrucción y de factor
int tokinidecl[NOTOKENS],tokiniinst[NOTOKENS],tokinifact[NOTOKENS];

//conjunto de sincronización de arranque
int set_arranque[NOTOKENS]; 

//INICIO DE CONJUNTOS SIGUIENTES.

int sig_auxpuntoycoma[NOTOKENS]; //Se utiliza en ABRIRARCHIVO,ASIGNACION,INS_CONSOLEREAD,INS_CONSOLEWRITE
int sig_auxllavec[NOTOKENS]; //BLOQUE,CASE
int sig_auxinstr[NOTOKENS]; // INS_DO, INS_FOR, INS_IF, INS_SWITCH, INS_WHILE
int sig_cadvar[NOTOKENS]; 
int sig_compageneral[NOTOKENS];
int sig_companum[NOTOKENS]; 
int sig_condicion[NOTOKENS]; 
int sig_conjunvar[NOTOKENS]; 
int sig_data_cad[NOTOKENS]; 
int sig_data_num[NOTOKENS]; 
int sig_declaracion[NOTOKENS];
int sig_exprbool[NOTOKENS];
int sig_exprnum[NOTOKENS];
int sig_auxfunc[NOTOKENS]; //EXPRESION_CAD,FUN_CONCAT,FUN_REPLACE,FUN_SUBSTRING,IDENTIFICADOR
int sig_funcion[NOTOKENS]; // SE USA TAMBIEN CON VARIABLE
int sig_funcinst[NOTOKENS];
int sig_identificador[NOTOKENS];
int sig_instruccion[NOTOKENS];
int sig_llaveinstr[NOTOKENS];
int sig_operacionnum[NOTOKENS];
int sig_parencondicion[NOTOKENS];
int sig_termino[NOTOKENS];
int sig_tipofile[NOTOKENS];

//FIN DE CONJUNTOS SIGUIENTES.

void conjuntos_siguientes(){
		init_set(sig_auxpuntoycoma);
		sig_auxpuntoycoma[tok_finlinea]=1;

		init_set(sig_auxllavec);
		sig_auxllavec[tok_llavec]=1;

		init_set(sig_cadvar);
		sig_cadvar[tok_coma]=1;

		init_set(sig_compageneral);
		sig_compageneral[tok_substring]=sig_compageneral[tok_concat]=sig_compageneral[tok_replace]=1;
		sig_compageneral[tok_cadena]=sig_compageneral[tok_caracter]=sig_compageneral[tok_fscanf]=sig_compageneral[tok_fgets]=1;
		union_set(sig_compageneral,sig_compageneral,sig_companum);
		
		init_set(sig_companum);
		sig_companum[tok_round]= sig_companum[tok_sin]=sig_companum[tok_cos]=sig_companum[tok_arcsin]=sig_companum[tok_arccos]=sig_companum[tok_parena]=1;
		sig_companum[tok_arctan]=sig_companum[tok_sqrt]=sig_companum[tok_length]=sig_companum[tok_pow]=sig_companum[tok_log]=1;
		sig_companum[tok_numero]=sig_companum[tok_flotante]=sig_companum[tok_sum]=sig_companum[tok_resta]=sig_companum[tok_id]=1;

		init_set(sig_auxinstr);
		sig_auxinstr[tok_id]=sig_auxinstr[tok_if]=sig_auxinstr[tok_while]=sig_auxinstr[tok_for]=sig_auxinstr[tok_switch]=1;
		sig_auxinstr[tok_do]=sig_auxinstr[tok_write]=sig_auxinstr[tok_read]=sig_auxinstr[tok_fileopen]=1;

		init_set(sig_conjunvar);
		sig_conjunvar[tok_llavea]=sig_conjunvar[tok_finlinea]=1;

		init_set(sig_condicion);
		sig_condicion[tok_coma]=sig_condicion[tok_parenc]=1;
		union_set(sig_condicion,sig_condicion,sig_conjunvar);

		init_set(sig_data_cad);
		sig_data_cad[tok_coma]=sig_data_cad[tok_finlinea]=sig_data_cad[tok_parenc]=sig_data_cad[tok_igual]=sig_data_cad[tok_negacion]=1;

		init_set(sig_data_num);
		sig_data_num[tok_parenc]=sig_data_num[tok_multi]=sig_data_num[tok_divi]=sig_data_num[tok_igual]=sig_data_num[tok_negacion]=sig_data_num[tok_finlinea]=1;
		sig_data_num[tok_mayor]= sig_data_num[tok_menor] = sig_data_num[tok_mayorigual] = sig_data_num[tok_menorigual] = sig_data_num[tok_coma]=1;


		init_set(sig_declaracion);
		sig_declaracion[tok_funcint]=sig_declaracion[tok_funcchar]=sig_declaracion[tok_funcstr]=sig_declaracion[tok_funcfloat]=sig_declaracion[tok_bool]=1;
		sig_declaracion[tok_id]=sig_declaracion[tok_if]=sig_declaracion[tok_while]=sig_declaracion[tok_for]=sig_declaracion[tok_switch]=sig_declaracion[tok_do]=1;
		sig_declaracion[tok_write]=sig_declaracion[tok_read]=sig_declaracion[tok_fileopen]=sig_declaracion[tok_return]=1;

		init_set(sig_exprbool);
		sig_exprbool[tok_or]=sig_exprbool[tok_and]=sig_exprbool[tok_finlinea]=1;

		init_set(sig_auxfunc);
		sig_auxfunc[tok_coma]=sig_auxfunc[tok_finlinea]=sig_auxfunc[tok_parenc]=sig_auxfunc[tok_igual]=sig_auxfunc[tok_negacion]=1;	

		init_set(sig_exprnum);
		sig_exprnum[tok_parenc]=sig_exprnum[tok_multi]=sig_exprnum[tok_divi]=sig_exprnum[tok_igual]=sig_exprnum[tok_negacion]=sig_exprnum[tok_menor]=1;
		sig_exprnum[tok_menorigual]=sig_exprnum[tok_mayor]=sig_exprnum[tok_mayorigual]=sig_exprnum[tok_coma]=sig_exprnum[tok_finlinea]=1;

		init_set(sig_funcion);
		sig_funcion[tok_id]=1;
		
		init_set(sig_identificador);
		union_set(sig_identificador,sig_identificador,sig_auxfunc);
		sig_identificador[tok_sumasign]=1;
		
		init_set(sig_instruccion);
		sig_instruccion[tok_return]=sig_instruccion[tok_llavec]=sig_instruccion[tok_break]=sig_instruccion[tok_fclose]=1;

		init_set(sig_llaveinstr);
		sig_llaveinstr[tok_while]=1;	

		init_set(sig_operacionnum);
		sig_operacionnum[tok_multi]=sig_operacionnum[tok_divi]=sig_operacionnum[tok_igual]=sig_operacionnum[tok_negacion]=sig_operacionnum[tok_menor]=1;
		sig_operacionnum[tok_menorigual]=sig_operacionnum[tok_mayor]=sig_operacionnum[tok_mayorigual]=sig_operacionnum[tok_coma]=sig_operacionnum[tok_finlinea]=1;
		sig_operacionnum[tok_parenc]=1;

		init_set(sig_parencondicion);
		sig_parencondicion[tok_llavea];

		init_set(sig_termino);
		sig_termino[tok_sum]=sig_termino[tok_resta]=1;

		init_set(sig_tipofile);
		sig_tipofile[tok_parenc]=1;

}

void inicializa_conjuntos() {
//creación de conjuntos de tokens iniciales
init_set(tokinidecl);
//tokinidecl[consttok]=tokinidecl[vartok]=tokinidecl[proctok]=1;
    
init_set(tokiniinst);
//tokiniinst[calltok]=tokiniinst[begintok]=tokiniinst[iftok]=tokiniinst[whiletok]=1;
   
init_set(tokinifact);
//tokinifact[ident]=tokinifact[numero]=tokinifact[parena]=1;

//creación del set de arranque del parser con los tokens iniciales de declaración+tokens iniciales de instrucción+punto
union_set(set_arranque,tokinidecl,tokiniinst);
//set_arranque[punto]=1;
}

//init_set                     
//crear al conjunto vacío
//0 en todas las posiciones del array
void init_set(int conjunto[])
{ 
 int i;
 for (i=0;i<NOTOKENS;++i) 
   conjunto[i]=0;
}

//copia_set
//copia el segundo conjunto en el primero
void copia_set(int conjunto1[],int conjunto2[])
{
 int i;
 for (i=0;i<NOTOKENS;++i)
  conjunto1[i]=conjunto2[i];
}

//union_set
//la unión de conjuntos: conjunto1=conjunto2+conjunto3
void union_set(int conjunto1[],int conjunto2[],int conjunto3[])
{
 int i;
 copia_set(conjunto1,conjunto2);
 for (i=0;i<NOTOKENS;++i)
  if (conjunto3[i]==1)
   conjunto1[i]=1;
}

//test
//rutina de comprobación (test) y posible salto de texto sobre el programa fuente
void test (int conjunto1[],int conjunto2[],int n)
{
 int conj_union[NOTOKENS];

 if (conjunto1[token]==0) { 
  //el token no está en el conjunto1
  error(n); //se marca el error
  //se arma un conjunto de estabilización
  union_set(conj_union,conjunto1,conjunto2);
  //se salta texto de manera "inteligente"
  while (conj_union[token]==0) 
   obtoken();
 }
}
