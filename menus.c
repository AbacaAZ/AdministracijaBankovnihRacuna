#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <stdlib.h>
#include "menus.h"
#include "util.h"
#include "bank_account.h"
#include "passwords.h"

extern const ACCOUNT* currAcc = NULL;

int mainMenu() {
	char choice = 0;

	enum choices {
		END_PROGRAM = '0',
		TRANSACTION_HISTORY = '1',
		REQUEST_LOAN = '2',
		TRANSFER_BALANCE = '3',
		ACCOUNT_SETTINGS = '4'
	};

	do {
		printMainMenu();

		do {
			choice = getch();
		} while (choice < '0' && choice > '4');

		switch (choice) {
		case TRANSACTION_HISTORY:
			break;
		case REQUEST_LOAN:
			break;
		case TRANSFER_BALANCE:
			break;
		case ACCOUNT_SETTINGS:
			system("cls");
			choice = accountSettingsMenu();
			break;
		case END_PROGRAM:
			return RETURN_END;
			break;
		default:
			system("cls");
		}
	} while (choice);

	return RETURN_SUCCESS;
}

int accountSettingsMenu() {
	char choice = 0;

	enum choices {
		GO_BACK = '0',
		CHANGE_PASSWORD = '1',
		DELETE_ACCOUNT = '2'
	};

	do {
		printAccountSettingsMenu();

		do {
			choice = getch();
		} while (choice < '0' && choice > '2');

		switch (choice) {
		case CHANGE_PASSWORD:
			if (changePassword(currAcc)) {
				system("cls");
				printf("Password change failed\n\n");
			}
			else {
				system("cls");
				printf("Password succesfully changed\n\n");
			}
			return RETURN_END;
			break;
		case DELETE_ACCOUNT:

			return RETURN_SUCCESS;
			break;
		case GO_BACK:
			system("cls");
			return RETURN_END;
			break;
		default:
			system("cls");
		}
	} while (choice);
}

int loginMenu() {
	volatile char choice = 0;

	enum choices {
		END_PROGRAM = '0',
		SIGN_UP = '1',
		LOG_IN = '2'
	};

	while (currAcc == NULL) {
		printLoginMenu();

		do {
			choice = getch();
		} while (choice < '0' && choice > '2');

		switch (choice) {
		case SIGN_UP:
			signUp();
			return RETURN_SUCCESS;
			break;
		case LOG_IN:
			login(NULL);
			return RETURN_SUCCESS;
			break;
		case END_PROGRAM:
			return RETURN_END;
			break;
		default:
			system("cls");
		}
	};
}

void printMainMenu() {
	printf("Logged in as:\n");
	printAccount(currAcc);
	printf("Please input a number coresponding to an option (0 - 4)\n");
	printf("1 - Transaction history\n2 - Request a loan\n3 - Transfer balance\n4 - Account settings\n0 - Exit program\n");
}

void printLoginMenu() {
	printf("Please input a number coresponding to an option (0 - 2)\n");
	printf("1 - Sign up\n2 - Log in\n0 - Exit program\n");
}

void printAccountSettingsMenu() {
	printf("Logged in as:\n");
	printAccount(currAcc);
	printf("Please input a number coresponding to an option (0 - 2)\n");
	printf("1 - Change password\n2 - Delete account\n0 - Go back\n");
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
	system("cls");
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
			system("cls");
			return RETURN_ABORT;
		}
	}
	else {
		system("cls");
		return RETURN_ABORT;
	}
	

	int matches = 0;
	ACCOUNT** accs = findByFullName(name, surname, &matches);
	if (!accs) {
		system("cls");
		return RETURN_NOT_FOUND;
	}

	int match = 0;
	int id = -1;
	const int LOGIN_ATTEMPTS = 3;

	for (int i = 0; i < LOGIN_ATTEMPTS; i++) {
		printf("Enter passsword: ");
		if (!inputString(password, passwordCondition) || !password) {
			return RETURN_ABORT;
		}

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
			system("cls");
			return RETURN_SUCCESS;
			break;
		} else if (i < LOGIN_ATTEMPTS - 1) {
			printf("Password and name don't match!\nAttempts left: %d\n", LOGIN_ATTEMPTS - 1 - i);
		}

	}

	if (!currAcc) {
		system("cls");
		return RETURN_FAILURE;
	}

	return RETURN_END;
}