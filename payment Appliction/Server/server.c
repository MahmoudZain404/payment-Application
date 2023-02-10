/*
 * File:   server.c
 * Author: mahmoud zain el abidean salah
 */
#define _CRT_SECURE_NO_WARNINGS // To disable deprecation (localtime function unsafe error )
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "server.h"

ST_accountsDB_t refrance[1];
int index = 0;
uint32_t transCounter;
float MaxAmount = 10000;

ST_accountsDB_t accountsDB[256] =
{
	{10500.50, RUNNING, "4539319503436467"},
	{1200.50, RUNNING, "8989374615436851"},
	{9000.50, RUNNING, "1631689343463654"},
	{13000.50, RUNNING, "7825063301701872"},
	{15000.50, BLOCKED, "5807007076043875"},
};

										// {"Mahmoud Zain Salah Diab", "4539319503436467", "05/24"}, {9000, 10000, ""}, APPROVED, 1    5807007076043875
										// {"Mohamed Zain Salah Diab", "8989374615436851", "06/24"}, {10500, 10000, ""}, DECLINED_INSUFFECIENT_FUND, 2
ST_transaction_t transactionDB[256] =
{
	{0,0,0,0}
};										// {"neassma Zain Salah Diab", "1631689343463654", "11/24"}, {9500, 10000, ""}, DECLINED_INSUFFECIENT_FUND, 3}
										// {"Hatem Mohamed sultan", "7825063301701872", "12/22"}, {10000, 10000, ""}, DECLINED_INSUFFECIENT_FUND, 4}
										// {"Ahmed Mohamed sultan", "5807007076043875", "02/24"}, {10000, 10000, ""}, DECLINED_STOLEN_CARD, 5}

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	
		if (index < 255 && index > -1)
		{
			EN_serverError_t isValid = isValidAccount(&transData[index].cardHolderData,accountsDB);
			EN_serverError_t isBlocked = isBlockedAccount(refrance);
			EN_serverError_t amontAvli = isAmountAvailable(&transData[index].terminalData, refrance);

			
			if (isValid != SERVER_OK)
			{
				return FRAUD_CARD;
			}
			else if (isBlocked != SERVER_OK)
			{
				return DECLINED_STOLEN_CARD;
			}
			else if (amontAvli != SERVER_OK)
			{
				return DECLINED_INSUFFECIENT_FUND;
			}
			else 
			{
				for (int i = 0; i < 255; i++)
				{
					if (strcmp(transData[index].cardHolderData.primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0)
					{
						accountsDB[i].balance -= transData[index].terminalData.transAmount;
					}
				}
				return APPROVED;
			}
		}
		else 
		{
			return INTERNAL_SERVER_ERROR;
		}
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence) 
{
	EN_cardError_t Name = getCardHolderName(cardData);
	EN_cardError_t ExpDate = getCardExpiryDate(cardData);
	EN_cardError_t PAN = getCardPAN(cardData);
	EN_terminalError_t isPANVal = isValidCardPAN(cardData); // TERMINAL_OK

	if (Name == CARD_OK && ExpDate == CARD_OK && isPANVal == TERMINAL_OK)
	{
		for (int i = 0; i < 255; i++) 
		{
			if (strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0)
			{
				strcpy(refrance[0].primaryAccountNumber, accountsDB[i].primaryAccountNumber);
				refrance[0].balance = accountsDB[i].balance;
				refrance[0].state = accountsDB[i].state;
				return SERVER_OK;
			}
		}
	}
	return ACCOUNT_NOT_FOUND;
}


EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) 
{
	if (refrance[0].state == BLOCKED)
	{
		return BLOCKED_ACCOUNT;
	}
	else
	{
		return SERVER_OK;
	}
}


EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence) 
{
	if (termData->transAmount == 0)
	{
		getTransactionAmount(termData);
	}
	if (termData->transAmount > refrance[0].balance)
	{
		return LOW_BALANCE;
	}
	else
	{

		return SERVER_OK;
	}
}
 

EN_serverError_t saveTransaction(ST_transaction_t* transData) 
{
	
	
	//printf("%d", index);
	//transData[index].transactionSequenceNumber = transCounter++;


		// GET CARD
		ST_cardData_t Card = { "", "", "" };
		getCardHolderName(&Card);
		//printf("%c", Card.cardHolderName);
		getCardExpiryDate(&Card);
		getCardPAN(&Card);
		//GET TERMINAL
		ST_terminalData_t Terminal = { 0,0,"" };
		getTransactionAmount(&Terminal);
		setMaxAmount(&Terminal, MaxAmount);
		getTransactionDate(&Terminal);
	printf("\nPlease Enter Your Transaction sequence Number: ");
	scanf_s("%d", &transData[index].transactionSequenceNumber);
	transCounter = transData[index].transactionSequenceNumber;
		if(transData[index].transactionSequenceNumber < 255)
		{
			// SET CARD
			strcpy(transData[index].cardHolderData.cardHolderName, Card.cardHolderName);
			strcpy(transData[index].cardHolderData.cardExpirationDate, Card.cardExpirationDate);
			strcpy(transData[index].cardHolderData.primaryAccountNumber, Card.primaryAccountNumber);
			// SET TERMINAL
			transData[index].terminalData.transAmount = Terminal.transAmount;
			transData[index].terminalData.maxTransAmount = Terminal.maxTransAmount;
			strcpy(transData[index].terminalData.transactionDate, Terminal.transactionDate);
			//SET ACCOUNT STATE
			EN_transState_t state = recieveTransactionData(transData);
			transData[index].transState = state;

			
			
			listSavedTransactions();
		}
		else 
		{
			return SAVING_FAILED;
		}
	return SERVER_OK;
}



void isValidAccountTest(void)
{
	printf("\nTester Name: Mahmoud zain salah\nFunction Name: isValidAccount\n");
	EN_serverError_t res = 0;
	ST_cardData_t case1 = { "Mahmoud Zain Salah Diab","5807007076043875","02/24" };
	res = isValidAccount(&case1, accountsDB);
	printf("\nTest case 1 \nInput Data: %s\t%s\t%s \nExpected Result: 0 (SERVER_OK)\nActual Result: %d (SERVER_OK)\n", case1.cardHolderName, case1.primaryAccountNumber, case1.cardExpirationDate, res);
	res = 0;
	ST_cardData_t case2 = { "Mahmoud Zain Salah Diab","5807007076043876","02/24" };
	res = isValidAccount(&case2, accountsDB);
	printf("\nTest case 2 \nInput Data: %s\t%s\t%s \nExpected Result: 3 (ACCOUNT_NOT_FOUND)\nActual Result: %d (ACCOUNT_NOT_FOUND)\n", case2.cardHolderName, case2.primaryAccountNumber, case2.cardExpirationDate, res);
}
void isBlockedAccountTest(void)
{
	printf("\nTester Name: Mahmoud zain salah\nFunction Name: isBlockedAccount\n");
	EN_serverError_t res = 0;
	ST_cardData_t case1 = { "Mahmoud Zain Salah Diab","5807007076043875","02/24" }; //BLOCKED_ACCOUNT
	isValidAccount(&case1, accountsDB);
	res = isBlockedAccount(accountsDB); 
	printf("\nTest case 1 \nInput Data: (Referace = %s) \nExpected Result: 5 (BLOCKED_ACCOUNT)\nActual Result: %d (BLOCKED_ACCOUNT)\n", refrance[0].primaryAccountNumber, res);
	res = 0;
	ST_cardData_t case2 = { "Mahmoud Zain Salah Diab","4539319503436467","02/24" }; //RUNNING
	isValidAccount(&case2, accountsDB);
	res = isBlockedAccount(accountsDB);
	printf("\nTest case 2 \nInput Data: (Referace = %s) \nExpected Result: 0 (SERVER_OK)\nActual Result: %d (SERVER_OK)\n", refrance[0].primaryAccountNumber, res);
}
void isAmountAvailableTest(void)
{
	printf("\nTester Name: Mahmoud zain salah\nFunction Name: isAmountAvailable\n");
	EN_serverError_t res = 0;
	ST_cardData_t cCase1 = { "Mahmoud Zain Salah Diab","4539319503436467","02/24" }; //RUNNING
	isValidAccount(&cCase1, accountsDB);
	ST_terminalData_t tCase1 = {3000,0,""};
	res = isAmountAvailable(&tCase1,accountsDB);
	printf("\nTest case 1 \nInput Data: (Amount: %f) \nExpected Result: 0 (SERVER_OK)\nActual Result: %d (SERVER_OK)\n", tCase1.transAmount, res);
	res = 0;
	ST_terminalData_t tCase2 = { 16000,0,"" };
	res = isAmountAvailable(&tCase2, accountsDB);
	printf("\nTest case 2 \nInput Data: (Amount: %f) \nExpected Result: 4 (LOW_BALANCE)\nActual Result: %d (LOW_BALANCE)\n", tCase2.transAmount, res);
}
void saveTransactionTest(void)
{
	printf("\nTester Name: Mahmoud zain salah\nFunction Name: saveTransaction\n");
	
	EN_serverError_t res = 0;
	res = saveTransaction(transactionDB);
	printf("\nTest case 1 \nInput Data: (sequence number: %d) \nExpected Result: 0 (SERVER_OK)\nActual Result: %d (SERVER_OK)\n" , transCounter, res);
}
void recieveTransactionDataTest(void)
{
	printf("\nTester Name: Mahmoud zain salah\nFunction Name: recieveTransactionData\n");
	EN_transState_t res = 0;
	ST_transaction_t tdb[5] =
	{
		{{"Mahmoud Zain Salah Diab", "5807007076043875", "05/24"}, {9000, 10000, ""}, 0, 0 },
		{{"Mohamed Zain Salah Diab", "4539319503436467", "06/24"}, {20000, 10000, ""}, 0, 0 },
		{{"neassma Zain Salah Diab", "8989374515436851", "11/24"}, {9500, 10000, ""}, 0, 0},
		{{"Hatem Mohamed sultan", "1631689343463654", "12/22"}, {8000, 10000, ""}, 0, 0},
	};
	  
	res = recieveTransactionData(tdb);
	printf("\nTest case 1 \nInput Data: (%s\t%s\t%s ) \nExpected Result: 2 (DECLINED_STOLEN_CARD)\nActual Result: %d (DECLINED_STOLEN_CARD)\n", tdb[0].cardHolderData.cardHolderName, tdb[0].cardHolderData.primaryAccountNumber, tdb[0].cardHolderData.cardExpirationDate, res);
	res = 0;
	 res = recieveTransactionData(tdb);
	printf("\nTest case 2 \nInput Data: (%s\t%s\t%s ) \nExpected Result: 1 (DECLINED_INSUFFECIENT_FUND)\nActual Result: %d (DECLINED_INSUFFECIENT_FUND)\n", tdb[1].cardHolderData.cardHolderName, tdb[1].cardHolderData.primaryAccountNumber, tdb[1].cardHolderData.cardExpirationDate, res);
	
	res = 0;
	res = recieveTransactionData(tdb);
	printf("\nTest case 3 \nInput Data: (%s\t%s\t%s ) \nExpected Result: 3 (FRAUD_CARD)\nActual Result: %d (FRAUD_CARD)\n", tdb[2].cardHolderData.cardHolderName, tdb[2].cardHolderData.primaryAccountNumber, tdb[2].cardHolderData.cardExpirationDate, res);
	
	res = 0;
	res = recieveTransactionData(tdb);
	printf("\nTest case 4 \nInput Data: (%s\t%s\t%s ) \nExpected Result: 0 (APPROVED)\nActual Result: %d (APPROVED)\n", tdb[3].cardHolderData.cardHolderName, tdb[3].cardHolderData.primaryAccountNumber, tdb[3].cardHolderData.cardExpirationDate, res);


}
void listSavedTransactions(void) 
{
	for (int i = 0; i <= 255; i++) 
	{
		if (transactionDB[i].cardHolderData.cardHolderName[0] == 0) 
		{
		}
		else
		{
			printf("\n#########################\nTransaction Sequence Number: %d\nTransaction Date: %s\nTransaction Amount: %f\nTransaction State: %d\nTerminal Max Amount: %f\nCardholder Name: %s\nPAN: %s\nCard Expiration Date: %s\n#########################\n"
				,transactionDB[i].transactionSequenceNumber, transactionDB[i].terminalData.transactionDate, transactionDB[i].terminalData.transAmount, transactionDB[i].transState, transactionDB[i].terminalData.maxTransAmount,
				transactionDB[i].cardHolderData.cardHolderName,transactionDB[i].cardHolderData.primaryAccountNumber, transactionDB[i].cardHolderData.cardExpirationDate);
		}
	}
}



/*int main() 
{
	
	saveTransactionTest();


	//printf("\n\n\n\n########################################\n\n\n\n");
	//isValidAccountTest();
	//printf("\n\n\n\n########################################\n\n\n\n");

	//isBlockedAccountTest();
	//printf("\n\n\n\n########################################\n\n\n\n");

	//isAmountAvailableTest();
	//printf("\n\n\n\n########################################\n\n\n\n");

	
	//recieveTransactionDataTest();
	//listSavedTransactions();
	//recieveTransactionData(transactionDB);
	//ST_cardData_t Card = { "Mahmoud Zain Salah Diab","5807007076043875","02/24" };
	//isValidAccount(&Card, accountsDB);
	//saveTransaction(transactionDB);
	//recieveTransactionData(transactionDB);
}*/