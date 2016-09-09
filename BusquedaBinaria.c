#include<stdio.h> 
#include <string.h>

char reservadas[3][9]={"aaaaa",
						"bbbbb",
						"ccccc"}; 

int main()
{
	int i = busqueda_binaria_palabras_reservadas("aaaaa");
	printf("%d",i);
	return 0;
}

int busqueda_binaria_palabras_reservadas(char *palabra)
{
	int ini = 0;
	int fin = (sizeof(reservadas)/sizeof(*reservadas));
	
	int medio;
	int comparacion;
	
	while(ini <= fin){
		medio = (ini + fin)/2;
			printf("medio val %d \n",medio);
			printf("medio %s \n",reservadas[medio]);
			printf("palabra %s \n",palabra);
			
		comparacion = strcmp(palabra, reservadas[medio]);
		switch(comparacion)
		{
			case -1: 
				fin = medio - 1;
				break;
			case 1: 
				ini = medio + 1;
				break;
			case 0: 
				return medio;
		}
	}
	return -1;
}
