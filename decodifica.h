#ifndef DECODIFICA_H_INCLUDED
#define DECODIFICA_H_INCLUDED

#include "codifiche/c39.h"
//#include "codifiche/c128.h"
#include "codifiche/EAN.h"
#include "codifiche/larghezze.h"

//#include "MLD_C.h"
#include "define.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  int type;
  unsigned char* result;
  int lenght;
} G_ResultHandle;

G_ResultHandle* leggi(uint16_t tc[], unsigned short N);

void* G_Decode(uint16_t* edges, unsigned int len, unsigned int scncnt, unsigned int safeMinimumRead){
	G_ResultHandle* handle = leggi(edges, len);
	return (void*)handle;
}

int G_GetResultProperty(void* resHandle, int property){
    G_ResultHandle* ptr = (G_ResultHandle*) resHandle;
    if(property == RESULT_SYMBOLOGY)
       return ptr->type;
    else
       return COMPLETE;
}

int G_GetResultLength(void* resHandle){
    G_ResultHandle* ptr = (G_ResultHandle*) resHandle;
    return ptr->lenght;
}

int G_GetResultContent(void* resHandle, unsigned char** pDestination){
    G_ResultHandle* ptr = (G_ResultHandle*) resHandle;
    *pDestination = ptr->result;
    return 42;
}

G_ResultHandle* decodifica(unsigned short partenza, unsigned short fine);

uint16_t larghezze[150]; //(uint16_t*) malloc(sizeof(uint16_t) * N);
uint8_t  p[150]; //(uint8_t*) malloc(sizeof(uint8_t) * N);
uint8_t  f[150]; //(uint8_t*) malloc(sizeof(uint8_t) * N);

// restituisce 1 handle oppure NULL.
G_ResultHandle* leggi(uint16_t tc[], unsigned short N){
        
    G_ResultHandle* simbolo;
  
    if(N > 150)
       	return NULL;
        
	larghezze[0] = tc[0];
               
	for(int i=1;i<N;i++)
          larghezze[i] = tc[i] - tc[i-1];
                
	//Scelgo i possibili punti di inizio e fine del simbolo

	uint8_t psize = 0;
	uint8_t fsize = 0;
	p[psize++] = 1;
	for(int i=1;i<N-1;i++){
		if(larghezze[i] > larghezze[i+1] * 6)
			p[psize++] = i+1;
		if(i>0 && larghezze[i] > larghezze[i-1] * 6)
			f[fsize++] = i-1;
	}
  
	f[fsize++] = N-1;
	//provo a decodificare scegliendo gli intervalli più opportuni
	uint8_t jp = 0;
	uint8_t jf = 0;
	for(; jp<psize && jf<fsize; jp++, jf++){
		
		while(jf < fsize-1 && p[jp] > f[jf])	jf++;
		while(jp < psize-1 && p[jp+1] < f[jf]) 	jp++;
		
		//simbolo = (G_ResultHandle*)malloc(sizeof(G_ResultHandle));   
        //simbolo->type = DL_INVALID_PROPERTY;
                simbolo = decodifica(p[jp], f[jf]);
                
		if(simbolo->type != DL_INVALID_PROPERTY)	
                {  return simbolo;
                }
                else
                {  free(simbolo);
                   simbolo = NULL;
                }
	}
        
    //free(p);
	//free(f);
    //free(larghezze);
	return NULL;
                
}



// crea sempre 1 G_ResultHandle

//return handle has to be deallocated in the main function after being used
G_ResultHandle* decodifica(unsigned short partenza, unsigned short fine){
	
    uint16_t *dec_larghezze = (uint16_t *)malloc(150*sizeof(uint16_t) );
	G_ResultHandle* resHandle = (G_ResultHandle*) malloc( sizeof(G_ResultHandle) );
	
	for(int i=partenza;i<=fine;i++)
		dec_larghezze[i-partenza] = larghezze[i];
	
	aggiustaLarghezze(dec_larghezze, fine-partenza);
        
	if(larghezze[0] <= 0){
		resHandle->type = DL_INVALID_PROPERTY;
		free(dec_larghezze);
        return resHandle;
	}
	for(int i=0;i<=fine-partenza;i++){
		if(dec_larghezze[i] > 4){
			resHandle->type = DL_INVALID_PROPERTY;
            free(dec_larghezze);
			return resHandle;
		}
	}

	////////////////////////////////

	{
		unsigned char* res = decode39(dec_larghezze, fine-partenza);
		if(res != NULL){
			resHandle->result = res;
			resHandle->lenght = strlen((const char*)res);
			resHandle->type = MLD_CODE_39_STANDARD;
            free(dec_larghezze);
            return resHandle;
		}
	}

	///////////////////////////////
    /*
	{
		unsigned char* res = decode128(larghezze, fine-partenza);
		if(res != NULL){
			resHandle.result = res;
			resHandle.lenght = strlen((const char*)res);
			resHandle.type = MLD_CODE_128_STANDARD;
            return resHandle;
		}
	}
    */
	///////////////////////////////
	
	{
		unsigned char* res = decodeEAN13(dec_larghezze, fine-partenza);
		if(res != NULL){
			resHandle->result = res;
			resHandle->lenght = strlen((const char*)res);
            resHandle->type = MLD_EAN_13;
            free(dec_larghezze);
			return resHandle;
		}
	}

    free(dec_larghezze);
	resHandle->type = DL_INVALID_PROPERTY;
	return resHandle;
}
#endif
