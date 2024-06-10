#define _CRT_SECURE_NO_WARNINGS
#include "passwords.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int inputPassword(char* const password){
	char password2[31] = { '\0' };

	do {
		printf("Enter password: ");
		if (!inputString(password, passwordCondition)/* || !strcmp(password, "end")*/) {
			return RETURN_ABORT;
		}

		printf("Confirm password: ");
		if (!inputString(password2, passwordCondition)/* || !strcmp(password, "end")*/) {
			return RETURN_ABORT;
		}

		if (strcmp(password, password2)) {
			printf("The passwords don't match\n");
			return RETURN_FAILURE;
		}
		else {
			return RETURN_SUCCESS;
		}
	} while (1);
}

int registerPassword(const char* const password, ACCOUNT* const acc) {
	PASSWORD pass = { 0 };
	int nmRead = 0;
	int hash = 0;
	hash = hashNameSurname(acc);

	encrypt(password);
	strcpy(pass.password, password);
	pass.hash = hash;
	pass.index = 0;
	if (adjustDuplicateIndex(&pass)) {
		return RETURN_TRY_AGAIN;
	}
	acc->index = pass.index;

	FILE* fp = NULL;
	fp = fopen("passwords.bin", "rb+");
	if (!fp) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	fseek(fp, 0, SEEK_END);
	fwrite(&pass, sizeof(PASSWORD), 1, fp);

	fclose(fp);
	return RETURN_SUCCESS;
}

int setPassword(ACCOUNT* const acc) {

	char password[31] = { '\0' };

	switch (inputPassword(password)) {
	case RETURN_ABORT:
		return RETURN_ABORT;
		break;
	case RETURN_FAILURE:
		return RETURN_FAILURE;
		break;
	default:
		return RETURN_SUCCESS;
	}

	switch(registerPassword(password, acc)) {
	case RETURN_TRY_AGAIN:
		return RETURN_TRY_AGAIN;
		break;
	default:
		return RETURN_SUCCESS;
	}
}

int passwordCondition(const char* const str) {
	int valid = 0;

	for (int i = 0; str[i] != '\0'; i++) {
		valid = isalnum(str[i]);
		if (!valid) {
			printf("Password can contain only alphanumerical characters.\nRe-enter password: ");
			break;
		}
	}

	return valid;
}

void changePassword(ACCOUNT* const acc) {
	FILE* fp = NULL;
	fp = fopen("passwords.bin", "rb+");
	if (!fp) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	PASSWORD pass = { 0 };
	size_t nmRead = 0;
	int hash = 0;
	hash = hashNameSurname(acc);

	while (1) {
		nmRead = fread(&pass, sizeof(PASSWORD), 1, fp);
		if (nmRead == 0) {
			break;
		}

		if (pass.hash == hash && pass.index == acc->index) {
			//setPassword(acc); // wrong
			fseek(fp, (-1) * (long long)(sizeof(PASSWORD)), SEEK_CUR);
			//fwrite()
			break;
		}
	}

	fclose(fp);
}

int isWrongPassword(const ACCOUNT* const acc, const char* const password) {
	FILE* fp = NULL;
	fp = fopen("passwords.bin", "rb+");
	if (!fp) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	PASSWORD pass = { 0 };
	size_t nmRead = 0;
	int retValue = 0;
	int hash = 0;
	hash = hashNameSurname(acc);
	char encPass[30] = { '\0' };
	strcpy(encPass, password);
	encrypt(encPass);

	while (1) {
		nmRead = fread(&pass, sizeof(PASSWORD), 1, fp);
		if (nmRead == 0) {
			break;
		}

		if (pass.hash == hash && pass.index == acc->index) {
			fclose(fp);
			
			retValue = strcmp(encPass, pass.password);
			break;
		}
	}

	fclose(fp);
	return retValue;
}

int adjustDuplicateIndex(PASSWORD* const newPass) {
	FILE* fp = NULL;
	fp = fopen("passwords.bin", "rb+");
	if (!fp) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	PASSWORD pass = { 0 };
	size_t nmRead = 0;

	while(1) {
		nmRead = fread(&pass, sizeof(PASSWORD), 1, fp);
		if (nmRead == 0) {
			break;
		}

		if (pass.hash == newPass->hash && pass.index == newPass->index && !strcmp(pass.password, newPass->password)) {
			printf("Please enter a different password\n");
			fclose(fp);
			return RETURN_TRY_AGAIN;
		} else if (pass.hash == newPass->hash && pass.index == newPass->index) {
			newPass->index++;
		}

		
	}

	fclose(fp);
	return RETURN_SUCCESS;
}

char* encrypt(char* const pass) {
	for (int i = 0; pass[i] != '\0'; i++) {
		pass[i] = pass[i] - 32 + i;
	}

	return pass;
}

char* decrypt(char* const pass) {
	for (int i = 0; pass[i] != '\0'; i++) {
		pass[i] = pass[i] + 32 - i;
	}

	return pass;
}

int hashNameSurname(const ACCOUNT* const acc) {
	int hash = 0;

	hash = sumStr(acc->name);
	hash += sumStr(acc->surname);

	return hash;
}

int sumStr(const char* const str) {
	int sum = 0;

	for (int i = 0; str[i] != '\0'; i++) {
		sum += str[i];
	}

	return sum;
}