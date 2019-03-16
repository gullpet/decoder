#include <stdio.h>
#include <time.h>

#include "define.h"
#include "decodifica.h"

typedef void* ResultHandle;

typedef struct{
	int len;
	unsigned char* dataPtr;
	int type;
} LABEL_STRUCT_TYPE;

int main(){
	clock_t start = clock();
	FILE *fr;
	//FILE *fw;
	fr = fopen ("input/Example.txt", "r");
	//fw = fopen ("output.txt", "w");
	
	//leggo 1 riga alla volta e provo a decodificarla
	
	do{
		short int N;
		fscanf(fr, "//%hd\n", &N);
		fscanf(fr, "%hd", &N);
		uint16_t tc[N];
		
		for(int i=0;i<N;i++)
			fscanf(fr, " %hd", tc+i);
		
		fscanf(fr, "\n");
		
		LABEL_STRUCT_TYPE* lPtr = (LABEL_STRUCT_TYPE*) malloc(sizeof(LABEL_STRUCT_TYPE));
		ResultHandle* resHandle;
		if((resHandle = G_Decode(tc, N, -1, -1)) != NULL){
			lPtr->type = G_GetResultProperty(resHandle, RESULT_SYMBOLOGY);
			lPtr->len = G_GetResultLength(resHandle);
			
			if(lPtr->type != DL_INVALID_PROPERTY){
				G_GetResultContent(resHandle, (unsigned char**) &(lPtr->dataPtr) );
				printf("Simbolo decodificato: %s (Codice ", lPtr->dataPtr);
				switch(lPtr->type){
					case MLD_CODE_39_STANDARD:
						printf("39");
						break;
					case MLD_CODE_128_STANDARD:
						printf("128");
						break;
					default:
						printf("EAN13");
				}
				printf(")\n");
				free(lPtr->dataPtr);
			}
			free(resHandle);
		}
		free(lPtr);

	}while(!feof(fr));
	
	clock_t end = clock();
	printf("%f\n", ((double) end-start)/CLOCKS_PER_SEC);
	//fclose(fw);
	fclose(fr);
}
