#pragma once
#include "bank_account.h"

enum returnIndicators {
	RETURN_SUCCESS,
	RETURN_ABORT,
	RETURN_FAILURE,
	RETURN_TRY_AGAIN
};

typedef struct password {
	int hash;
	int index;
	char password[31];
}PASSWORD;

char* encrypt(char* const pass);
char* decrypt(char* const pass);
int hashNameSurname(const ACCOUNT* const acc);
int sumStr(const char* const str);
int adjustDuplicateIndex(PASSWORD* const newPass);
int isWrongPassword(const ACCOUNT* const acc, const char* const password);
void changePassword(ACCOUNT* const acc);
int passwordCondition(const char* const str);
int setPassword(ACCOUNT* const acc);
int inputPassword(char* const password);