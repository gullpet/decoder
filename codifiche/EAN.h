#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#ifndef CODIFICHE_H
#define CODIFICHE_H
#include "codifiche.h"
#endif

const char codificheEAN13[10][13] = {
	"AAAAAACCCCCC\0",
	"AABABBCCCCCC\0",
	"AABBABCCCCCC\0",
	"AABBBACCCCCC\0",
	"ABAABBCCCCCC\0",
	"ABBAABCCCCCC\0",
	"ABBBAACCCCCC\0",
	"ABABABCCCCCC\0",
	"ABABBACCCCCC\0",
	"ABBABACCCCCC\0",
};

const char* toInt(short int partenza, short int fine, bool bits[]){
	int res = ((int)bits[partenza])+1;
	for(int i=partenza+1; i<=fine; i++){
		res *= 10;
		res += ((int)bits[i])+1;
	}
	return getValEAN(res);
}

const char* provaEAN13(bool bits[]);

const char* decodeEAN13(short int larghezze[], short int N){
	short int nElem = 0;
	for(int i=0;i<=N;i++)
		nElem += larghezze[i];

	if(nElem != 95)
		return "\0";
	
	bool bits[nElem];
	bool bitsReverse[nElem];
	
	short int cont = 0;
	bool stato_att = 1;
	for(int i=0;i<=N;i++){
		for(int j=0;j<larghezze[i];j++){
			bits[cont] = stato_att;
			bitsReverse[nElem-cont-1] = stato_att;
			cont++;
		}
		stato_att = !stato_att;
	}
	
	const char* result = provaEAN13(bits);
	if(*result != '\0')	return result;
	return provaEAN13(bitsReverse);
	
}

const char* provaEAN13(bool bits[]){
	char* result = (char*) malloc(sizeof(char) * 13);
	char* mode = (char*) malloc(sizeof(char) * 12);
	
	///////////// controllo di start
	{			 
		const char* leper = toInt(0, 2, bits);
		if(*leper != -5)
			return "\0";
	}			 
	/////////////
				 
	short int c = 1;
	for(int j=3;j<45;j+=7){
		const char* leper = toInt(j, j+6, bits);
		if(*leper == -5)
			return "\0";
				 
		result[c] = leper[0] + 48;
		mode[c-1] = leper[1];
		c++;	 
	}			 
				 
	/////////////
	{
		const char* leper = toInt(45, 49, bits);
		if(*leper != -6)
			return "\0";
	}
	/////////////
				 	
	for(int j=50;j<92;j+=7){
		const char* leper = toInt(j, j+6, bits);
		if(*leper == -5)
			return "\0";
				 
		result[c] = leper[0] + 48;
		mode[c-1] = leper[1];
		c++;	 
	}			 
				 
	result[c] = '\0';
	mode[c-1] = '\0';
	c++;
	
	/////////////
	{
		const char* leper = toInt(92, 94, bits);
		if(*leper != -5)
			return "\0";
	}
	/////////////
	
	for(int i=0;i<10;i++){
		if(strcmp(codificheEAN13[i], mode) == 0){
			*result = i + 48;
			return result;
		}
	}
	return "\0";
}
