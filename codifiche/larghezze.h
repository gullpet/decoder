#ifndef LARGHEZZE_H_INCLUDED
#define LARGHEZZE_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>

/* La tolleranza è alta per provare ad eliminare le imperfezioni rilevate dal laser
 * funziona in modo abbastanza efficiente sia per 2 spessori (39) che per 4 (128 ed EAN13)
*/
#define tolleranza 0.70

short int approssima(uint16_t a, uint16_t b){
	return (a/b) + (a/(double)b > (a/b) + tolleranza); 
}

typedef struct{
    uint16_t l;
	uint8_t ind;
}larg;

int compare(const void* a, const void* b){
	larg* as = (larg*) a;
	larg* bs = (larg*) b;
	return as->l - bs->l;
}


/*
void aggiustaLarghezze(uint16_t* larghezze, unsigned int N){
  uint16_t modulo = WINT_MAX;
  for(int i=0;i<N;i++){
    if(larghezze[i] <= 0){
        *larghezze = 0;
        return;
    }
    if(larghezze[i] < modulo)
        modulo = larghezze[i];
  }
  
  for(int i=0;i<N;i++)
    larghezze[i] = approssima(larghezze[i], modulo);
  
}
*/

void aggiustaLarghezze(uint16_t* larghezze, unsigned int N){
	
	larg* larghezzeTemp = (larg*) malloc(sizeof(larg) * N+1);
	for(int i=0;i<=N;i++){
		larghezzeTemp[i].l = larghezze[i];
		larghezzeTemp[i].ind = i;
	}
	qsort(larghezzeTemp, N+1, sizeof(larg), compare);
	larghezze[larghezzeTemp[0].ind] = 1;
	
	uint16_t media=larghezzeTemp[0].l;
	uint16_t att=0;
	
	if(media <= 0){
		larghezze[0] = 0;
		free(larghezzeTemp);
		return;
	}
	
	for(int i=1;i<=N;i++){
		if(approssima(larghezzeTemp[i].l, larghezzeTemp[0].l) == approssima(larghezzeTemp[att].l, larghezzeTemp[0].l))
			media+=larghezzeTemp[i].l;
		else{
			media/=i-att;
			
			for(int k=att;k<i;k++)
				larghezze[larghezzeTemp[k].ind] = media;
			
			media = larghezzeTemp[i].l;
			att = i;
		}
	}
	
	media/=(N+1-att);
	for(int k=att;k<=N;k++)
		larghezze[larghezzeTemp[k].ind] = media;
	
	uint16_t modulo = larghezze[0];
	
	for(int i=1;i<=N;i++)
		if(larghezze[i] < modulo)	modulo = larghezze[i];
	
	for(int i=0;i<=N;i++)
		larghezze[i] = approssima(larghezze[i], modulo);
	
	free(larghezzeTemp);
}

#endif
