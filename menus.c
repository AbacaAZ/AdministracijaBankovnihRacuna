#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <stdlib.h>
#include "menus.h"
#include "util.h"
#include "bank_account.h"
#include "passwords.h"

const ACCOUNT* currAcc = NULL;

int mainMenu() {
	char choice = 0;

	enum choices {
		END_PROGRAM,
	};

	do {
		//printMainMenu();

		do {
			scanf("%1c", &choice);
			choice -= '0';
		} while (choice < 0 && choice > 2);

		switch (choice) {
		case 1:
			break;
		case 2:
			break;
		case END_PROGRAM:
			return 0;
			break;
		default:
			printf("Please input a number coresponding to an option (0 - 2)\n");
		}
	} while (choice);

	return 1;
}

int loginMenu() {
	system("cls");
	char choice = 0;

	enum choices {
		END_PROGRAM = '0',
		SIGN_UP = '1',
		LOG_IN = '2'
	};

	do {
		printf("Please input a number coresponding to an option (0 - 2)\n");
		printLoginMenu();

		do {
			scanf("%1c", &choice);
		} while (choice < 0 && choice > 2);

		switch (choice) {
		case SIGN_UP:
			if (signUp()) {
				return RETURN_FAILURE;
			}
			break;
		case LOG_IN:
			if (login(NULL)) {
				return RETURN_FAILURE;
			}
			break;
		case END_PROGRAM:
			return RETURN_SUCCESS;
			break;
		default:
			system("cls");
			break;
		}
	} while (currAcc == NULL);

	return RETURN_SUCCESS;
}

void printLoginMenu() {
	printf("1 - Sign up\n2 - Log in\n0 - Exit program\n");
}

ACCOUNT* signUp() {
	ACCOUNT* acc = NULL;
	acc = createAccount();

	if (acc) {
		registerAccount(acc);
		if (login(acc)) {
			return NULL;
		}
	}
	return acc;
}

int login(const ACCOUNT* const acc) {
	if (acc != NULL) {
		currAcc = acc;
		return RETURN_SUCCESS;
	}

	char name[31] = { '\0' };
	char surname[31] = { '\0' };
	char password[31] = { '\0' };

	system("cls");
	printf("Enter \"end\" to cancel input\n");

	printf("Enter name: ");
	if (inputString(name, nameCondition)) {
		printf("Enter surname: ");
		if (!inputString(surname, surnameCondition)) {
			return RETURN_FAILURE;
		}
	}
	else {
		return RETURN_FAILURE;
	}
	

	int matches = 0;
	ACCOUNT** accs = findByFullName(name, surname, &matches);

	int match = 0;
	int id = -1;

	for (int i = 0; i < 3; i++) {
		printf("Enter passsword: ");
		if (!inputString(password, passwordCondition)) {
			return RETURN_FAILURE;
		}
		
		if (!password) {
			return RETURN_FAILURE;
		}
		system("cls");

		for (int j = 0; j < matches; j++) {
			match = !isWrongPassword(accs[j], password);
			if (match) {
				id = accs[j]->ID;
				safeFreeArr(&accs, matches);
				break;
			}
		}

		if (match) {
			currAcc = findByID(id);
			break;
		} else if (i < 2) {
			printf("Password and name don't match!\nAttempts left: %d\n", 2 - i);
		}

	}

	if (!currAcc) {
		return RETURN_FAILURE;
	}

	return RETURN_SUCCESS;
}