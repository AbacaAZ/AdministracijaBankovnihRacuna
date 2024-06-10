#pragma once

typedef struct bankAccount {
	double balance;
	char name[31];
	char surname[31];
	int ID;
	int index;
}ACCOUNT;

void init();
ACCOUNT* createAccount();
void registerAccount(ACCOUNT* const acc);
int setName(ACCOUNT* const acc);
int setSurname(ACCOUNT* const acc);
void setID(ACCOUNT* const acc);
int nameCondition(const char* const str);
int surnameCondition(const char* const str);
int getLastID();
int countAccounts();
void printAccount(const ACCOUNT* const acc);
void printAccounts(const ACCOUNT* const* const accs, const int n);
ACCOUNT** findByFullName(const char* const name, const char* const surname, int* const count);
ACCOUNT* findByID(const int ID);