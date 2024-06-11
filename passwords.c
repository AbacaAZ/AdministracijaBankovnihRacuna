#define _CRT_SECURE_NO_WARNINGS
#include "passwords.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

int inputPassword(char* const password){
	char password2[31] = { '\0' };

	do {
		printf("Enter password: \n");
		enterPassword(password);

		if (!strcmp(password, "end") || !passwordCondition(password)) {
			return RETURN_ABORT;
		}

		printf("Confirm password: \n");
		enterPassword(password2);

		if (!strcmp(password2, "end") || !passwordCondition(password2)) {
			return RETURN_ABORT;
		}

		if (strcmp(password, password2)) {
			system("cls");
			printf("The passwords don't match\n\n");
			return RETURN_FAILURE;
		}
		else {
			return RETURN_SUCCESS;
		}
	} while (1);
}

void enterPassword(char* const password) {
	char temp[31] = { '\0' };
	for (int i = 0; i < 30; i++) {
		temp[i] = _getch();

		switch (temp[i]) {
		case 8:
			if (i >= 1) {
				i--;
				printf("\b \b");
			}
			break;
		case '\n':
		case 13:
			temp[i] = '\0';
			printf("\n");
			return 0;
			break;
		default:
			printf("*");
			password[i] = temp[i];
		}
	}
}

int registerPassword(char* const password, ACCOUNT* const acc) {
	PASSWORD pass = { 0 };
	PASSWORD temp = { 0 };
	size_t nmRead = 0;
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

	while (1) {
		nmRead = fread(&temp, sizeof(PASSWORD), 1, fp);
		if (temp.hash == -1 && temp.index == -1 && strcmp(temp.password, "EMPTY")) {
			fseek(fp, (-1) * sizeof(PASSWORD), SEEK_CUR);
			break;
		}
		if (!nmRead) {
			break;
		}
	}

	fwrite(&pass, sizeof(PASSWORD), 1, fp);

	fclose(fp);
	return RETURN_SUCCESS;
}

int setPassword(ACCOUNT* const acc) {
	int temp = -1;
	char password[31] = { '\0' };

	switch (inputPassword(password)) {
	case RETURN_ABORT:
		return RETURN_ABORT;
		break;
	case RETURN_FAILURE:
		return RETURN_FAILURE;
		break;
	}


	switch(registerPassword(password, acc)) {
	case RETURN_TRY_AGAIN:
		return RETURN_TRY_AGAIN;
		break;
	}

	return RETURN_SUCCESS;
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

int changePassword(const ACCOUNT* const acc) {
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
			fclose(fp);
			return RETURN_FAILURE;
		}

		
		if (pass.hash == hash && pass.index == acc->index) {
			char password[31] = { '\0' };

			inputPassword(password);
			encrypt(password);

			strcpy(pass.password, password);

			while (checkIfUniquePassword(&pass) == RETURN_TRY_AGAIN) {
				inputPassword(password) // fix
				checkIfUniquePassword(&pass);
			}
			
			fseek(fp, (-1) * sizeof(PASSWORD), SEEK_CUR);
			fwrite(&pass, sizeof(PASSWORD), 1, fp);

			break;
		}
	}

	fclose(fp);
	return RETURN_SUCCESS;
}

int checkIfUniquePassword(PASSWORD* const newPass) {
	FILE* fp = NULL;
	fp = fopen("passwords.bin", "rb+");
	if (!fp) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	PASSWORD pass = { 0 };
	size_t nmRead = 0;

	while (1) {
		nmRead = fread(&pass, sizeof(PASSWORD), 1, fp);
		if (nmRead == 0) {
			break;
		}

		if (pass.hash == newPass->hash && pass.index == newPass->index && !strcmp(pass.password, newPass->password)) {
			printf("Please enter a different password\n");
			fclose(fp);
			return RETURN_TRY_AGAIN;
		}
		else if (pass.hash == newPass->hash && pass.index == newPass->index) {
			newPass->index++;
		}
	}

	fclose(fp);
	return RETURN_SUCCESS;
}

void deletePassword(const ACCOUNT* const acc) {
	if (!acc) {
		return;
	}

	FILE* fp = NULL;
	fp = fopen("passwords.bin", "rb+");
	if (!fp) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	PASSWORD pass = { 0 };
	PASSWORD empty = { -1, -1, "EMPTY"};
	size_t nmRead = 1;
	int hash = 0;
	hash = hashNameSurname(acc);

	while (1) {
		nmRead = fread(&pass, sizeof(PASSWORD), 1, fp);
		if (!nmRead) {
			break;
		}
		else if (pass.hash == hash && pass.index == acc->index) {
			fseek(fp, (-1) * sizeof(PASSWORD), SEEK_CUR);
			fwrite(&empty, sizeof(PASSWORD), 1, fp);
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