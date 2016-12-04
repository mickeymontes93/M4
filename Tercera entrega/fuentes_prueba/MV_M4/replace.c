#include "stdio.h"
#include "string.h"


#define MAX 100
int main(void){
	char string[MAX];
	char string2, string3;
	strcpy(string,"aaaaaaaaabbaacccccccccc\0");
	string2='a';
	string3='z';
	int i;

	for (i=0; i< strlen(string); i++){
        if((int) string[i]== (int) string2){
            string[i]=string3;
        }
    }

    printf("%s",string);
}