#include "codifiche/c39.h"
#include "codifiche/c128.h"
#include "codifiche/EAN.h"
#include "codifiche/larghezze.h"
#include <stdlib.h>
#include <stdio.h>

const char** decodifica(short int larghezzeT[], short int partenza, short int fine);

const char** leggi(short int tc[], short int N){
		short int larghezze[N];
		
		short int first=tc[0], second=tc[1];
		larghezze[0] = tc[0];
		larghezze[1] = second - first;
		first = second;
	
		for(int i=2;i<N;i++){
			second = tc[i];
			larghezze[i] = second - first;
			first = second;
		}
		
		//Scelgo i possibili punti di inizio e fine del simbolo
		short int p[N];
		short int f[N];
		short int psize = 0;
		short int fsize = 0;
		p[psize++] = 1;
		for(int i=1;i<N-1;i++){
			if(larghezze[i] > larghezze[i+1] * 5)
				p[psize++] = i+1;
			if(i>0 && larghezze[i] > larghezze[i-1] * 5)
				f[fsize++] = i-1;
		}

		f[fsize++] = N-1;
		
		//provo a decodificare scegliendo gli intervalli più opportuni
		unsigned short int jp = 0;
		unsigned short int jf = 0;
		for(; jp<psize && jf<fsize; jp++, jf++){
			
			while(jf < fsize-1 && p[jp] > f[jf])	jf++;
			while(jp < psize-1 && p[jp+1] < f[jf]) 	jp++;
			
			const char** simbolo = decodifica(larghezze, p[jp], f[jf]);
			if(*(*simbolo) != '\0')	return simbolo;
		}
		const char** nf = (const char**) malloc(sizeof(const char*) * 2);
		*nf = "\0";
		nf[1] = "\0";
		return nf;
}

const char** decodifica(short int larghezzeT[], short int partenza, short int fine){
	short int larghezze[fine-partenza+1];
	
	const char** ris = (const char**) malloc(sizeof(const char*) * 2);
		
	for(int i=partenza;i<=fine;i++)
		larghezze[i-partenza] = larghezzeT[i];
	
	aggiustaLarghezze(larghezze, fine-partenza);
	
	if(larghezze[0] == 0){	
		*ris = "\0";
		ris[1] = "\0";
		return ris;
	}
	for(int i=0;i<=fine-partenza;i++){
		if(larghezze[i] > 4){	
			*ris = "\0";
			ris[1] = "\0";
			return ris;
		}
	}
	for(int i=0;i<3;i++){
		if(!i){
			const char* res = decode39(larghezze, fine-partenza);
			if(*res != '\0'){
				*ris = res;
				ris[1] = "39";
				return ris;
			}
		}
		else if(i==1){
			const char* res = decode128(larghezze, fine-partenza);
			if(*res != '\0'){ 
				*ris = res;
				ris[1] = "128";
				return ris;
			}
		}
		else{
			const char* res = decodeEAN13(larghezze, fine-partenza);
			if(*res != '\0'){
				*ris = res;
				ris[1] = "EAN13";
				return ris;
			}
		}
	}
	
	*ris = "\0";
	ris[1] = "\0";
	return ris;
}
