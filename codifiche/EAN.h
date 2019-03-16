#ifndef EAN_H_INCLUDED
#define EAN_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "codifiche.h"

short* toInt(short int partenza, short int fine, bool bits[]){
	int res = ((int)bits[partenza])+1;
	for(int i=partenza+1; i<=fine; i++){
		res *= 10;
		res += ((int)bits[i])+1;
	}
	return getValEAN(res);
}

unsigned char* provaEAN13(bool bits[]);

unsigned char* decodeEAN13(uint16_t larghezze[], short int N){
	unsigned int nElem = 0;
	for(int i=0;i<=N;i++)
		nElem += larghezze[i];

	if(nElem != 95)
		return NULL;
	
	bool bits[95];
	
	unsigned short cont = 0;
	bool stato_att = 1;
	for(int i=0;i<=N;i++){
		for(int j=0;j<larghezze[i];j++){
			bits[cont] = stato_att;
			cont++;
		}
		stato_att = !stato_att;
	}
	
	unsigned char* result = provaEAN13(bits);
	
	if(result != NULL)	return result;
	
	cont = 0;
	stato_att = 1;
	for(int i=0;i<=N;i++){
		for(int j=0;j<larghezze[i];j++){
			bits[95-cont-1] = stato_att;
			cont++;
		}
		stato_att = !stato_att;
	}

	return provaEAN13(bits);
	
}

unsigned char* provaEAN13(bool bits[]){
	unsigned char* result = (unsigned char*) malloc(sizeof(unsigned char*) * 14);
	unsigned char mode[13];
	
	///////////// controllo di start
	{			 
		short* temp = toInt(0, 2, bits);
		if(*temp != -5){
			free(result);
			free(temp);
			return NULL;
		}
		free(temp);
	}			 
	/////////////
				 
	short int c = 1;
	for(int j=3;j<45;j+=7){
		short* temp = toInt(j, j+6, bits);
		if(*temp < 0 || temp[1] < 0){
			free(result);
			free(temp);
			return NULL;
		}
				 
		result[c] = temp[0] + 48;
		mode[c-1] = temp[1];
		c++;
		free(temp);
	}			 
				 
	/////////////
	{
		short* temp = toInt(45, 49, bits);
		if(*temp != -6){
			free(result);
			free(temp);
			return NULL;
		}
		free(temp);
	}
	/////////////
				 	
	for(int j=50;j<92;j+=7){
		short* temp = toInt(j, j+6, bits);
		if(*temp < 0 || temp[1] < 0){
			free(result);
			free(temp);
			return NULL;
		}
		result[c] = temp[0] + 48;
		mode[c-1] = temp[1];
		c++;
		free(temp);
	}			 
				 
	result[c] = '\0';
	mode[c-1] = '\0';
	c++;
	
	/////////////
	{
		short* tmp = toInt(92, 94, bits);
		if(*tmp != -5){
			free(result);
			free(tmp);
			return NULL;
		}
		free(tmp);
	}
	/////////////
	
	//controllo della prima cifra
	if(strcmp("AAAAAACCCCCC", (const char*)mode) == 0){
		*result = '0';
		return result;
	}
	else if(strcmp("AABABBCCCCCC", (const char*)mode) == 0){
		*result = '1';
		return result;
	}
	else if(strcmp("AABBABCCCCCC", (const char*)mode) == 0){
		*result = '2';
		return result;
	}
	else if(strcmp("AABBBACCCCCC", (const char*)mode) == 0){
		*result = '3';
		return result;
	}
	else if(strcmp("ABAABBCCCCCC", (const char*)mode) == 0){
		*result = '4';
		return result;
	}
	else if(strcmp("ABBAABCCCCCC", (const char*)mode) == 0){
		*result = '5';
		return result;
	}
	else if(strcmp("ABBBAACCCCCC", (const char*)mode) == 0){
		*result = '6';
		return result;
	}
	else if(strcmp("ABABABCCCCCC", (const char*)mode) == 0){
		*result = '7';
		return result;
	}
	else if(strcmp("ABABBACCCCCC", (const char*)mode) == 0){
		*result = '8';
		return result;
	}
	else if(strcmp("ABBABACCCCCC", (const char*)mode) == 0){
		*result = '9';
		return result;
	}
	else{
		free(result);
		return NULL;
	}
}
#endif       //EAN_H_INCLUDED
