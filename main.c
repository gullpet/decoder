#include <stdio.h>
#include <time.h>

#include "decodifica.h"

int main(){
	FILE *fr;
	//FILE *fw;
	fr = fopen ("input/Example.txt", "r");
	//fw = fopen ("output.txt", "w");
	
	//leggo 1 riga alla volta e provo a decodificarla
	
	do{
		short int N;
		fscanf(fr, "//%hd\n", &N);
		fscanf(fr, "%hd", &N);
		short int tc[N];
	
		for(int i=0;i<N;i++)
			fscanf(fr, " %hd", tc+i);
		
		fscanf(fr, "\n");
		
		const char** result = leggi(tc, N);
		if(strcmp(*result, "\0"))
			printf("Simbolo decodificato: %s (Codice %s)\n", *result, result[1]);

	}while(!feof(fr));
	
	//fclose(fw);
	fclose(fr);
}
