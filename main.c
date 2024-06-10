#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include "menus.h"
#include "transactions.h"
#include "util.h"
#include "passwords.h"

int main() {
	init();
	
	int option = -1;
	while (option != RETURN_END) {

		switch (loginMenu()) {
		case RETURN_END:
			option = RETURN_END;
			break;
		case RETURN_SUCCESS:

			switch (mainMenu()) {
			case RETURN_END:
				option = RETURN_END;
				break;
			default:
				continue;
			}

			break;
		default:
			continue;
		}

	}
	
}