#pragma once

#include "bank_account.h"

int mainMenu();
int loginMenu();
void printLoginMenu();
int login(const ACCOUNT* const acc);
ACCOUNT* signUp();