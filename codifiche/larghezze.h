#include <stdlib.h>
#include <stdio.h>

/* La tolleranza è alta per provare ad eliminare le imperfezioni rilevate dal laser
 * funziona in modo abbastanza efficiente sia per 2 spessori (39) che per 4 (128 ed EAN13)
*/
#define tolleranza 0.70

short int approssima(short int a, short int b){
	return (a/b) + (a/(double)b > (a/b) + tolleranza); 
}

struct larg{
	short int l;
	short int ind;
};

int compare(const void* a, const void* b){
	struct larg* as = (struct larg*) a;
	struct larg* bs = (struct larg*) b;
	return as->l - bs->l;
}

void aggiustaLarghezze(short int larghezze[], short int N){
	
	struct larg larghezzeTemp[N+1];
	for(int i=0;i<=N;i++){
		larghezzeTemp[i].l = larghezze[i];
		larghezzeTemp[i].ind = i;
	}
	qsort(larghezzeTemp, N+1, sizeof(struct larg), compare);
	larghezze[larghezzeTemp[0].ind] = 1;
	
	short int media=larghezzeTemp[0].l;
	short int att=0;
	
	if(!media){
		larghezze[0] = 0;
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
	
	short int modulo = larghezze[0];
	
	for(int i=1;i<=N;i++)
		if(larghezze[i] < modulo)	modulo = larghezze[i];
	
	for(int i=0;i<=N;i++)
		larghezze[i] = approssima(larghezze[i], modulo);
}
