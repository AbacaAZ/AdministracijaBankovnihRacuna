#pragma once

typedef struct transaction {
	double value;
	char sender[62];
	char reciever[62];
	struct transaction* next;
	int senderID;
	int recieverID;
}TRANS;

TRANS* createTrans(const ACCOUNT* const sender, const ACCOUNT* const reciever, const double value);
TRANS* addToTail(TRANS* head, const ACCOUNT* const sender, const ACCOUNT* const reciever, const double value);
TRANS** findTransByName(TRANS* head);