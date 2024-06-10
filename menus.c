#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
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
		END_PROGRAM,
		SIGN_UP,
		LOG_IN
	};

	do {
		printf("Please input a number coresponding to an option (0 - 2)\n");
		printLoginMenu();

		do {
			scanf("%1c", &choice);
			choice -= '0';
		} while (choice < 0 && choice > 2);

		switch (choice) {
		case SIGN_UP:
			if (signUp()) {

			}
			break;
		case LOG_IN:
			if (login(NULL)) {

			}
			break;
		case END_PROGRAM:
			return 0;
			break;
		default:
			system("cls");
			break;
		}
	} while (currAcc == NULL);

	return 1;
}

void printLoginMenu() {
	printf("1 - Sign up\n2 - Log in\n0 - Exit program\n");
}

ACCOUNT* signUp() {
	ACCOUNT* acc = NULL;
	acc = createAccount();
	if (!acc) {
		login(acc);
	}
	return acc;
}

int login(const ACCOUNT* const acc) {
	if (acc != NULL) {
		currAcc = acc;
		return 1;
	}

	char name[31] = { '\0' };
	char surname[31] = { '\0' };
	char password[31] = { '\0' };
	printf("Enter \"end\" to cancel input\n");

	printf("Enter name: ");
	if (inputString(name, nameCondition)) {
		printf("Enter surname: ");
		if (!inputString(surname, surnameCondition)) {
			return 1;
		}
	}
	else {
		return 1;
	}
	
	system("cls");

	int matches = 0;
	ACCOUNT** accs = findByFullName(name, surname, &matches);

	int match = 0;
	int id = -1;

	for (int i = 0; i < 3; i++) {
		printf("Enter passsword: ");
		if (!inputString(password, passwordCondition)) {
			return 1;
		}
		
		if (!password) {
			return 1;
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
		return 1;
	}

	return 0;
}