#define _CRT_SECURE_NO_WARNINGS
#include "app.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "../Terminal/terminal.h"
#include <time.h>

float MaxAmo = 10000;
int saveIndx =  0 ;
ST_transaction_t transactionDB[255];

void appStart(void)
{
	ST_accountsDB_t refrance[1];

	uint32_t transCounter = 0;

	ST_accountsDB_t accountsDB[255] =
	{
		{10500.50, RUNNING, "4539319503436467"},
		{1200.50, RUNNING, "8989374615436851"},
		{9000.50, RUNNING, "1631689343463654"},
		{13000.50, RUNNING, "7825063301701872"},
		{15000.50, BLOCKED, "5807007076043875"},
	};

	ST_cardData_t CARDX = { "", "", "" };
	//GET CARD
	getCardHolderName(&CARDX);
	getCardExpiryDate(&CARDX);
	getCardPAN(&CARDX);
	// SET CARD
	strcpy(transactionDB[saveIndx].cardHolderData.cardHolderName, CARDX.cardHolderName);
	strcpy(transactionDB[saveIndx].cardHolderData.cardExpirationDate, CARDX.cardExpirationDate);
	strcpy(transactionDB[saveIndx].cardHolderData.primaryAccountNumber, CARDX.primaryAccountNumber);
	//GET TERMINAL
	ST_terminalData_t TERMINALX = { 0,0,"" };
	setMaxAmount(&TERMINALX, MaxAmo);
	getTransactionDate(&TERMINALX);
	strcpy(transactionDB[saveIndx].terminalData.transactionDate, TERMINALX.transactionDate);
	// SET TERMINAL
	transactionDB[saveIndx].terminalData.maxTransAmount = TERMINALX.maxTransAmount;
	strcpy(transactionDB[saveIndx].terminalData.transactionDate, TERMINALX.transactionDate);
	if (isCardExpired(&CARDX, &TERMINALX) == TERMINAL_OK)
	{
		getTransactionAmount(&TERMINALX);
		transactionDB[saveIndx].terminalData.transAmount = TERMINALX.transAmount;
		transactionDB[saveIndx].transactionSequenceNumber = ++transCounter;
	}
	else
	{
		printf("\n\"Declined Expired Card\"\n");
		exit(EXIT_SUCCESS);
	}
	if (isBelowMaxAmount(&TERMINALX) == EXCEED_MAX_AMOUNT)
	{
		printf("\n\"Declined Amount Exceeding Limit\"\n");
		exit(EXIT_SUCCESS);
	}
	else if (isValidAccount(&CARDX, accountsDB) == ACCOUNT_NOT_FOUND)
	{
		printf("\n\"Declined Invalid Account\"\n");
		exit(EXIT_SUCCESS);
	}
	else if (isAmountAvailable(&TERMINALX, refrance) == LOW_BALANCE)
	{
		printf("\n\"Declined Insuffecient Funds\"\n");
		exit(EXIT_SUCCESS);
	}
	else if (recieveTransactionData(transactionDB) == DECLINED_STOLEN_CARD)
	{
		printf("\n\"Declined Card (Stolen)\"\n");
		exit(EXIT_SUCCESS);
	}
	else 
	{
		for (int i = 0; i < 255; i++)
		{
			if (strcmp(transactionDB[saveIndx].cardHolderData.primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0)
			{
				accountsDB[i].balance -= transactionDB[saveIndx].terminalData.transAmount;
				printf("\nYou New Amount is: %f\n", accountsDB[i].balance);
			}
		}
	}

}

int main()
{

	ppStart();
	
}






// {"Mahmoud Zain Salah Diab", "4539319503436467", "05/24"}, {9000, 10000, ""}, APPROVED, 1    5807007076043875
//  // {"Mohamed Zain Salah Diab", "8989374615436851", "06/24"}, {10500, 10000, ""}, DECLINED_INSUFFECIENT_FUND, 2
// {"neassma Zain Salah Diab", "1631689343463654", "11/24"}, {9500, 10000, ""}, DECLINED_INSUFFECIENT_FUND, 3}
//  // {"Hatem Mohamed sultan", "7825063301701872", "12/22"}, {10000, 10000, ""}, DECLINED_INSUFFECIENT_FUND, 4}
// {"Ahmed Mohamed sultan", "5807007076043875", "02/24"}, {10000, 10000, ""},(2)DECLINED_STOLEN_CARD, 5}


