#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef CODIFICHE_H
#define CODIFICHE_H
#include "codifiche.h"
#endif

const char* decode39(short int larghezze[], int N){
	if(N < 8)	return "\0";
	short int k=0;
	while(larghezze[k] == 1 && k<=N)	k++;
	if(k == N+1)	return "\0";
	short int spess = larghezze[k];
	for(int i=k+1;i<=N;i++){
		if(larghezze[i] != spess && larghezze[i] != 1)
			return "\0";
	}
	//dopo aver controllato che siano presenti 2 spessori
	bool bits[N+1];
	bool bitsReverse[N+1];
	
	//riempio un vettore di bool con i bit rispettivi al codice (1 se è un elemento largo, 0 se è stretto)
	for(int j=0; j <= N; j++)
		bits[j] = larghezze[j] > 1;
		
	for(int j=N; j>=0; j--)
		bitsReverse[N-j] = bits[j];
	
	/*
	for(int j=0; j<=N; j++)
		printf("%d", bits[j]);
	printf("\n\n");
	*/
	
	//partendo dal vettore di bool lo decodifico 9 bit alla volta
	char* result = (char*) malloc( (N/9 + 2) * sizeof(char) );
	short int c=0;
	
	if(getVal39(bits, 0, 8) != '*' || getVal39(bits, N-8, N) != '*'){
		free(result);
		result = NULL;
	}
	
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
		result[c++] = '\0';
		return result;
	}
	
	c=0;
	result = (char*) malloc( (N/9 + 2) * sizeof(char));
	//Provo anche guardando i bit al contrario (lo scanner ha letto sottosopra)
	if(getVal39(bitsReverse, 0, 8) != '*' || getVal39(bitsReverse, N-8, N) != '*')
		return "\0";
		
	for(int j=10;j+8<N;j+=10){
		int temp = getVal39(bitsReverse, j, j+8);
		if(temp == -1 || (char)temp == '*')
			return "\0";
		result[c++] = (char)temp;
	}
	result[c++] = '\0';
	return result;
}
