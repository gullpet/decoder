#include <stdbool.h>

#ifndef CODIFICHE_H
#define CODIFICHE_H
#include "codifiche.h"
#endif

const char* prova128(short int* spessori, short int N);

const char* decode128(short int larghezze[], short int N){
	if(N < 20)	return "\0";
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
				return "\0";
		}
	}
	
	if(!v1 || !v2 || !v3 || !v4)
		return "\0";
	short int spessori[N+1];
	for(int i=0;i<=N;i++)
		spessori[i] = larghezze[i];
	
	const char* res = prova128(spessori, N);
	if(*res != '\0')	return res;
	
	for(int i=N; i>=0; i--)
		spessori[N-i] = larghezze[i];
	return prova128(spessori, N);
}

const char* prova128(short int* spessori, short int N){ 
	int check_digit = 0;
	char mode_att = 'A';
	const int* start = getVal128(spessori, 0, 5, mode_att);
	switch(start[0]){
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
			return "\0";
	}
	check_digit += start[1];
	if(*getVal128(spessori, N-6, N, mode_att) != -20)
		return "\0";	
	char* ans = (char*) malloc(N-13 * sizeof(char));
	short int cont = 0;
	for(int i=6;i+5<N-12;i+=6){
		const int* result = getVal128(spessori, i, i+5, mode_att);
		if(result == NULL || *result == -99)	return "\0";
		check_digit += result[1] * (i/6);
		if(mode_att == 'C' && result[1] < 100){
			ans[cont++] = result[0]/10 + 48;
			ans[cont++] = result[0]%10 + 48;
		}
		else{
			if(*result <= -20)	return "\0";
			if(*result <= -1 && *result >= -4)	ans[cont++] = '?';
			else if(*result <= -11 && *result >= -13)
				mode_att = 65 + ((*result * -1) - 10);
			else
				ans[cont++] = *result;
		}
	}
	check_digit %= 103;
	const int* check = getVal128(spessori, N-12, N-7, mode_att);
	if(check[1] != check_digit)
		return "\0";
	ans[cont++] = '\0';
	return ans;
}
