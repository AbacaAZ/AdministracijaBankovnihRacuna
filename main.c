#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "menus.h"
#include "transactions.h"
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "passwords.h"

int main() {
	init();
	extern const int g_accounts;

	/*ACCOUNT* temp3 = NULL;
	temp3 = createAccount();
	if (temp3) {
		registerAccount(temp3);
	}*/
	

	printf("ACCOUNTS: %d\n", g_accounts);

	
	loginMenu();
}