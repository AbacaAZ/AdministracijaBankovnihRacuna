#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "bank_account.h"
#include "transactions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


TRANS* createTrans(const ACCOUNT* const sender, const ACCOUNT* const reciever, const double value) {
	TRANS* trans = NULL;
	trans = (TRANS*)calloc(1, sizeof(TRANS));
	if (!trans) {
		perror("Failed to allocate memory for transaction");
		return NULL;
	}

	strcat(trans->sender, sender->name);
	strcat(trans->sender, " ");
	strcat(trans->sender, sender->surname);
	trans->senderID = sender->ID;
	
	strcat(trans->reciever, reciever->name);
	strcat(trans->reciever, " ");
	strcat(trans->reciever, reciever->surname);
	trans->recieverID = reciever->ID;

	trans->value = value;

	printf(trans->sender);
	printf(" >>> %0.2lf >>> ", trans->value);
	printf(trans->reciever);
	printf("\n");

	return trans;
}

TRANS* addToTail(TRANS* head, const ACCOUNT* const sender, const ACCOUNT* const reciever, const double value) {
	TRANS* temp = NULL;
	temp = createTrans(sender, reciever, value);

	if (!head) {
		head = temp;
	}
	else {
		TRANS* current = head;
		while (current->next) {
			current = current->next;
		}

		current->next = temp;
	}

	return head;
}

TRANS** findTransByName (TRANS* head) {
	int count = 0;
	TRANS* current = NULL;
	while (current) {
		current = current->next;

	}
}