#include "stdio.h"
#include "string.h"


#define MAX 100
int main(void){
	char string[MAX],respuesta[MAX];
	int inicio, fin;

	strcpy(string,"kate\0");
	inicio=1;
	fin=2;
	/*string2='a';
	string3='z';
	int i;

	for (i=0; i< strlen(string); i++){
        if((int) string[i]== (int) string2){
            string[i]=string3;
        }
    }
	*/

    int i,j=0;
    for(i=0;i<strlen(string);i++){
    	if(i<=fin && i>=inicio){
    		printf("%c",string[i]);
 			respuesta[j]=string[i];
 			j++;   		
    	}
    }
    respuesta[j]='\0';
    strcpy(string,respuesta);
    printf("\n\n%s\n",string);
}