#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bank_account.h"
#include "util.h"
#include "passwords.h"

extern int g_accounts = 0;

void init() {
	FILE* fp = NULL;
	fp = fopen("passwords.bin", "wbx");
	if (fp) {
		fclose(fp);
	}

	fp = fopen("accounts.bin", "wbx");
	if (fp) {
		fclose(fp);
	}

	g_accounts = countAccounts();
}

void printAccount(const ACCOUNT* const acc) {
	if (acc) {
		printf("Name:\t %s\nSurname: %s\nBalance: %0.2lf\nID:\t %d\n\n", acc->name, acc->surname, acc->balance, acc->ID);
	}
}

void printAccounts(const ACCOUNT* const* const accs, const int n) {
	if (accs) {
		for (int i = 0; i < n; i++) {
			if (accs[i]) {
				printf("Name:\t %s\nSurname: %s\nBalance: %0.2lf\nID:\t %d\n\n", accs[i]->name, accs[i]->surname, accs[i]->balance, accs[i]->ID);
			}
		}
	}
}

ACCOUNT** findByFullName(const char* const name, const char* const surname, int* const matches) {
	ACCOUNT** temp = NULL;
	temp = (ACCOUNT**)calloc(g_accounts, sizeof(ACCOUNT*));
	if (!temp) {
		perror("Failed to allocate memory for account");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < g_accounts; i++) {
		temp[i] = (ACCOUNT*)calloc(1, sizeof(ACCOUNT));
		if (!temp[i]) {
			safeFreeArr(&temp, i + 1);
			perror("Failed to allocate memory for account");
			exit(EXIT_FAILURE);
		}
	}

	FILE* fp = NULL;
	fp = fopen("accounts.bin", "rb+");
	if (!fp) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	int i = 0;
	size_t read = 1;
	while (1) {
		read = fread(temp[i], sizeof(ACCOUNT), 1, fp);
		if (!read) {
			break;
		} else if (!strcmp(temp[i]->name, name) && !strcmp(temp[i]->surname, surname)) {
			i++;
		}
	}

	*matches = i;
	if (*matches == 0) {
		fclose(fp);
		safeFreeArr(&temp, g_accounts);
		system("cls");
		printf("No accounts with that name and surname\n\n");
		return NULL;
	}

	ACCOUNT** accs = NULL;
	accs = (ACCOUNT**)calloc(*matches, sizeof(ACCOUNT*));
	for (int j = 0; j < *matches; j++) {
		accs[j] = (ACCOUNT*)calloc(1, sizeof(ACCOUNT));
		if (!accs[j]) {
			safeFreeArr(&accs, j + 1);
			break;
		}
	}

	for (int j = 0; j < *matches; j++) {
		strcpy(accs[j]->name, temp[j]->name);
		strcpy(accs[j]->surname, temp[j]->surname);
		accs[j]->ID = temp[j]->ID;
		accs[j]->index = temp[j]->index;
	}

	safeFreeArr(&temp, g_accounts);
	fclose(fp);

	return accs;
}

ACCOUNT* findByID(const int ID) {
	system("cls");
	ACCOUNT* acc = NULL;
	acc = (ACCOUNT*)calloc(1, sizeof(ACCOUNT));
	if (!acc) {
		perror("Failed to allocate memory for account");
		return NULL;
	}
	
	FILE* fp = NULL;
	fp = fopen("accounts.bin", "rb+");
	if (!fp) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	size_t read = 0;
	while (acc) {
		read = fread(acc, sizeof(ACCOUNT), 1, fp);
		if (acc->ID == ID) {
			break;
		}
		if (!read) {
			break;
		}
	}

	fclose(fp);

	if (!read) {
		printf("Account with that ID does NOT exist\n");
		return NULL;
	}
	return acc;
}

ACCOUNT* createAccount(){
	system("cls");
	ACCOUNT* acc = NULL;
	acc = (ACCOUNT*)calloc(1, sizeof(ACCOUNT));
	if (!acc) {
		perror("Failed to allocate memory for account");
		return NULL;
	}

	printf("Enter \"end\" to cancel input\n");

	if (setName(acc) || setSurname(acc)) {
		return NULL;
	}

	setID(acc);
	int check = setPassword(acc);
	while (check == RETURN_TRY_AGAIN) {
		check = setPassword(acc);
	}

	if (check) {
		return NULL;
	}
	acc->balance = 0;

	g_accounts++;
	system("cls");
	return acc;
}

void registerAccount(ACCOUNT* const acc) {
	if (!acc) {
		return;
	}

	FILE* fp = NULL;
	fp = fopen("accounts.bin", "rb+");
	if (!fp) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}


	fseek(fp, 0, SEEK_END);
	fwrite(acc, sizeof(ACCOUNT), 1, fp);

	fclose(fp);
}

void setID(ACCOUNT* const acc) {
	static int lastID = -1;

	if (lastID == -1) {
		lastID = getLastID();
	}

	acc->ID = ++lastID;
}

int getLastID() {
	FILE* fp = NULL;
	fp = fopen("accounts.bin", "rb+");
	if (!fp) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	int id = 0;
	ACCOUNT temp = { 0 };
	size_t read = 0;
	fseek(fp, (long long)(sizeof(ACCOUNT)) * (-1), SEEK_END);
	
	read = fread(&temp, sizeof(ACCOUNT), 1, fp);

	if (read) {
		id = temp.ID;
	}

	fclose(fp);
	return id;
}

int countAccounts() {
	FILE* fp = NULL;
	fp = fopen("accounts.bin", "rb+");
	if (!fp) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	int count = -1;
	ACCOUNT* acc = NULL;
	acc = (ACCOUNT*)calloc(1, sizeof(ACCOUNT));
	if (!acc) {
		perror("Failed to allocate memory for account");
		return -1;
	}

	size_t read = 0;
	do {
		read = fread(acc, sizeof(ACCOUNT), 1, fp);
		count++;
	}while (read);

	fclose(fp);
	return count;
}

int setName(ACCOUNT* const acc) {
	char name[31] = { '\0' };
	printf("Enter name: ");
	if (!inputString(name, nameCondition) || !strcmp(name, "end")) {
		return 1;
	}

	strcpy(acc->name, name);
	return 0;
}

int setSurname(ACCOUNT* const acc) {
	char surname[31] = { '\0' };
	printf("Enter surname: ");
	if (!inputString(surname, surnameCondition) || !strcmp(surname, "end")) {
		return 1;
	}

	strcpy(acc->surname, surname);
	return 0;
}

int nameCondition(const char* const str) {
	int alpha = 0;

	for (int i = 0; str[i] != '\0'; i++) {
		alpha = isalpha(str[i]);
		if (!alpha) {
			printf("Name can contain only alphabetic characters.\nRe-enter name: ");
			break;
		}
	}
	
	return alpha;
}

int surnameCondition(const char* const str) {
	int alpha = 0;

	for (int i = 0; str[i] != '\0'; i++) {
		alpha = isalpha(str[i]);
		if (!alpha) {
			printf("Surname can contain only alphabetic characters.\nRe-enter surname: ");
			break;
		}
	}

	return alpha;
}