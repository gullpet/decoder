#ifndef C39_H_INCLUDED
#define C39_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "codifiche.h"

unsigned char* decode39(uint16_t larghezze[], int N){
	if(N < 8)	return NULL;
	unsigned short k=0;
	while(larghezze[k] == 1 && k<=N)	k++;
	if(k == N+1)	return NULL;
	unsigned short spess = larghezze[k];

	for(int i=k+1;i<=N;i++){
		if(larghezze[i] != spess && larghezze[i] != 1)
			return NULL;
	}

	//dopo aver controllato che siano presenti 2 spessori
	//modified: from stack[200] to heap[150]
	bool* bits = (bool*) malloc(sizeof(bool) * N+2);
	
	//riempio un vettore di bool con i bit rispettivi al codice (1 se è un elemento largo, 0 se è stretto)
	for(int j=0; j<=N; j++)
		bits[j] = larghezze[j] > 1;
	
	//partendo dal vettore di bool lo decodifico 9 bit alla volta
	unsigned char* result = (unsigned char*) malloc( (N/9 + 5) * sizeof(unsigned char) );
	short int c=0;
	
	if(getVal39(bits, 0, 8) != '*' || getVal39(bits, N-8, N) != '*'){
		free(result);
		result = NULL;
	}
	else
		result[c++] = '*';
	
	for(int j=10;j+8<N && result != NULL;j+=10){
		int temp = getVal39(bits, j, j+8);
		if(temp == -1 || (char)temp == '*'){
			free(result);
			result = NULL;	//non esiste un carattere con quella codifica
			break;
		}
		result[c++] = (char)temp;
	}
	
	if(result != NULL){
		result[c++] = '*';
		result[c++] = '\0';
		free(bits);
		return result;
	}
	
	
	for(int i=N;i>=0;i--)
		bits[N-i] = larghezze[i] > 1;
		
	c=0;
	result = (unsigned char*) malloc( (N/9 + 5) * sizeof(unsigned char));
	result[c++] = '*';
	//Provo anche guardando i bit al contrario (lo scanner ha letto sottosopra)
	if(getVal39(bits, 0, 8) != '*' || getVal39(bits, N-8, N) != '*'){
		free(result);
		free(bits);
		return NULL;
	}
	for(int j=10;j+8<N;j+=10){
		int temp = getVal39(bits, j, j+8);
		if(temp < 0 || (char)temp == '*'){
			free(result);
			free(bits);
			return NULL;
		}
		result[c++] = (unsigned char)temp;
	}
	
	free(bits);
	result[c++] = '*';
	result[c++] = '\0';
	return result;
}
#endif          //C39_H_INCLUDED
