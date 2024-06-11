#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bank_account.h"

char* allocateStringMem(char* str, const int n) {
	str = (char*)calloc(n, sizeof(char));
	if (!str) {
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}

	return str;
}

char* inputString(char* const str, int (*condition)(const char* const)) {
	int check = 1;

	do {
		scanf("%30[^\n]%*c", str);
		check = strcmp(str, "end");
	} while (check && !condition(str));

	if (!check) {
		return NULL;
	}

	return str;
}

void safeFree(void** const p) {
	if (*p) {
		free(*p);
	}
	*p = NULL;
}

void safeFreeArr(void*** const pArr, const int n) {
	if (*pArr != NULL) {
		for (int i = 0; i < n; i++) {
			if (pArr[0][i]) {
				free(pArr[0][i]);
			}
			pArr[0][i] = NULL;
		}
		free(*pArr);
	}
	*pArr = NULL;
}