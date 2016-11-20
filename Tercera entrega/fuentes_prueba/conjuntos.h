#include "parametros.h"

//conjuntos de tokens para manejo de errores         
//tokens iniciales de declaración, de instrucción y de factor
extern int tokinidecl[NOTOKENS],tokiniinst[NOTOKENS],tokinifact[NOTOKENS];

//conjunto de sincronización de arranque
extern int set_arranque[NOTOKENS]; 

void inicializa_conjuntos(),init_set(int conjunto[]),copia_set(int conjunto1[],int conjunto2[]);
void conjuntos_siguientes();
void union_set(int conjunto1[],int conjunto2[],int conjunto3[]),test (int conjunto1[],int conjunto2[],int n);

extern int sig_auxpuntoycoma[NOTOKENS]; //Se utiliza en ABRIRARCHIVO,ASIGNACION,INS_CONSOLEREAD,INS_CONSOLEWRITE
extern int sig_auxllavec[NOTOKENS]; //BLOQUE,CASE
extern int sig_auxinstr[NOTOKENS]; // INS_DO, INS_FOR, INS_IF, INS_SWITCH, INS_WHILE
extern int sig_cadvar[NOTOKENS]; 
extern int sig_compageneral[NOTOKENS];
extern int sig_companum[NOTOKENS]; 
extern int sig_condicion[NOTOKENS]; 
extern int sig_conjunvar[NOTOKENS]; 
extern int sig_data_cad[NOTOKENS]; 
extern int sig_data_num[NOTOKENS]; 
extern int sig_declaracion[NOTOKENS];
extern int sig_exprbool[NOTOKENS];
extern int sig_exprnum[NOTOKENS];
extern int sig_auxfunc[NOTOKENS];
extern int sig_funcion[NOTOKENS]; // SE USA TAMBIEN CON VARIABLE
extern int sig_funcinst[NOTOKENS];
extern int sig_identificador[NOTOKENS];
extern int sig_instruccion[NOTOKENS];
extern int sig_llaveinstr[NOTOKENS];
extern int sig_operacionnum[NOTOKENS];
extern int sig_parencondicion[NOTOKENS];
extern int sig_termino[NOTOKENS];
extern int sig_tipofile[NOTOKENS];