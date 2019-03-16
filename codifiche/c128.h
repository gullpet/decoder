#ifndef C128_H_INCLUDED
#define C128_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

#include "codifiche.h"

unsigned char* prova128(uint16_t* spessori, short int N);

unsigned char* decode128(uint16_t larghezze[], short int N){
	if(N < 20)	return NULL;
	bool v1=false, v2=false, v3=false, v4=false;
	for(int i=0;i<=N;i++){
		switch(larghezze[i]){
			case 1:
				v1=true;
				break;
			case 2:
				v2=true;
				break;
			case 3:
				v3=true;
				break;
			case 4:
				v4=true;
				break;
			default:
				return NULL;
		}
	}
	
	if(!v1 || !v2 || !v3 || !v4)
		return NULL;
        
	uint16_t spessori[200];
	for(int i=0;i<=N;i++)
		spessori[i] = larghezze[i];
	
	unsigned char* res = prova128(spessori, N);
	if(res != NULL)	return res;
	
	for(int i=N; i>=0; i--)
		spessori[N-i] = larghezze[i];
	return prova128(spessori, N);
}

unsigned char* prova128(uint16_t spessori[], short int N){
	int check_digit = 0;
	char mode_att = 'A';
	int* ctr = getVal128(spessori, 0, 5, mode_att);
	switch(ctr[0]){
		case -21:
			mode_att = 'A';
			break;
		case -22:
			mode_att = 'B';
			break;
		case -23:
			mode_att = 'C';
			break;
		default:
			free(ctr);
			return NULL;
	}
	check_digit += ctr[1];
	free(ctr);

	ctr = getVal128(spessori, N-6, N, mode_att);
	if(*ctr != -20){
		free(ctr);
		return NULL;
	}
	free(ctr);

	unsigned char* ans = (unsigned char*) malloc(sizeof(unsigned char) * N);
	unsigned int cont = 0;
	for(int i=6;i+5<N-12;i+=6){
		int* result = getVal128(spessori, i, i+5, mode_att);
		if(result == NULL || *result == -99){
			free(ans);
			free(result);
			return NULL;
		}
		check_digit += result[1] * (i/6);
		if(mode_att == 'C' && result[1] < 100){
			ans[cont++] = result[0]/10 + 48;
			ans[cont++] = result[0]%10 + 48;
		}
		else{
			if(*result <= -20){
				free(ans);
				free(result);
				return NULL;
			}
			if(*result <= -1 && *result >= -4)	ans[cont++] = '?';
			else if(*result <= -11 && *result >= -13)
				mode_att = 65 + ((*result * -1) - 10);
			else
				ans[cont++] = *result;
		}
		free(result);
	}
	check_digit %= 103;
	int* check = getVal128(spessori, N-12, N-7, mode_att);
	if(check[1] != check_digit){
		free(check);
		free(ans);
		return NULL;
	}
	free(check);
	ans[cont++] = '\0';
	return ans;
}
#endif          //C128_H_INCLUDED
