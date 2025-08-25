/*
 * File:   terminal.c
 * Author: mahmoud zain el abidean salah
 */
#define _CRT_SECURE_NO_WARNINGS // To disable deprecation (localtime function unsafe error )
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "terminal.h"
#include <time.h>


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	if (termData->transactionDate[0] == 0)
	{
		char CurrentDate[11];
		time_t now = time(NULL);

		struct tm* tm = localtime(&now);

		sprintf(CurrentDate, "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);

		strcpy(termData->transactionDate, CurrentDate);
	}
	
	if (strlen(termData->transactionDate) == 0 || strlen(termData->transactionDate) < 10)
	{
		return WRONG_DATE;
	}
	else
	{
		return TERMINAL_OK;
	}
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {
	uint8_t cardDig0[6] = {"\0"};
	uint8_t cardDig1[6] = {"\0"};
	uint8_t cardDig3[6] = {"\0"};
	uint8_t cardDig4[6] = {"\0"};
	uint8_t cExpDate[6] = {"\0"};
	uint8_t Form[]= { "/" };
	uint8_t curDateDig3[11] = {"\0"};
	uint8_t curDateDig4[11] = {"\0"};
	uint8_t curDateDig8[11] = {"\0"};
	uint8_t curDateDig9[11] = {"\0"};
	uint8_t currentDate[11] = {"\0"};
	int cardMonth = 0;
	int cardYear = 0;
	int curMonth = 0;
	int curYear = 0;

	strcpy(cExpDate, cardData->cardExpirationDate); // copy date string from cardData
	strcpy(currentDate, termData->transactionDate); // copy current date from teamData
	
	if (cExpDate[2]== Form[0] && strlen(cExpDate) == 5) {
	
	}
	else {
		printf("\n\"You Have Entered A Wrong CardExpiryDate Format\"\n");
		exit(EXIT_SUCCESS);// stop excuting
	}
	/* break the card date down to reform it  */
	cardDig0[0] = cExpDate[0]; 
	cardDig1[0] = cExpDate[1];
	cardDig3[0] = cExpDate[3];
	cardDig4[0] = cExpDate[4]; 
	/* break the current date down to reform it  */
	curDateDig3[0] = currentDate[3];
	curDateDig4[0] = currentDate[4];
	curDateDig8[0] = currentDate[8];
	curDateDig9[0] = currentDate[9];

	strcat(cardDig0, cardDig1); // concat the card Month digets together 
	strcat(cardDig3, cardDig4); // concat the card year digets together 

	cardMonth = atoi(cardDig0); // converting card Month string to integer
	cardYear = atoi(cardDig3); // converting card year string to integer

	strcat(curDateDig3, curDateDig4);// concat the current Month digets together 
	strcat(curDateDig8, curDateDig9);// concat the current Month digets together 

	curMonth = atoi(curDateDig3); // converting current Month string to integer
	curYear = atoi(curDateDig8); // converting current year string to integer

	
	if (cardYear > curYear) 
	{
		return TERMINAL_OK;
	}
	else if (cardYear = curYear) 
	{
		if (cardMonth > curMonth)
		{
			return TERMINAL_OK;
		}
		else 
		{
			return EXPIRED_CARD;
		}
	}
	else
	{
		return EXPIRED_CARD;
	}
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	
	if (termData->transAmount == 0) 
	{
	printf("\nPlease Enter The Transaction Amount: ");
	scanf_s("%f", &termData->transAmount);
	}

	
	if (termData->transAmount <= 0) {
		return INVALID_AMOUNT;
	}
	else {
		return TERMINAL_OK;
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->transAmount > termData->maxTransAmount) 
	{
		return EXCEED_MAX_AMOUNT;
	}
	else 
	{
		return TERMINAL_OK;
	}
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount) {
	termData->maxTransAmount = maxAmount;
	if (termData->maxTransAmount <= 0) {
		return INVALID_MAX_AMOUNT;
	}
	else {
		return TERMINAL_OK;
	}
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData) {
		
		
		uint8_t cpy[20] = { '\0' };

		strcpy(cpy, cardData->primaryAccountNumber);

		if ((cpy[0] == '0') && !(cpy[1])) {
			return INVALID_CARD;
		}
		if (cpy[0] == ' ' && cpy[1] == '0')
			return INVALID_CARD;

		int sum = 0;
		int flag = 0;
		int end = 0;
		while (cpy[end] != '\0')
			end++;

		for (int i = end - 1; i >= 0; i--) {
			if (cpy[i] != ' ') {
				if (cpy[i] >= '0' && cpy[i] <= '9') {
					if (flag == 0) {
						sum += cpy[i] - '0';
						flag = 1;
					}
					else {
						int tmp = (cpy[i] - '0') * 2;
						if (tmp > 9)
							sum += (tmp - 9);
						else
							sum += tmp;
						flag = 0;
					}
				}
				else
				{
					return INVALID_CARD;
				}

			}
		}
		if (sum % 10 == 0) 
		{
			return TERMINAL_OK;
		}
		else 
		{
		return INVALID_CARD;
		}
}
 
//Test Functions
void getTransactionDateTest(void) {
	printf("\nTester Name: Mahmoud zain salah\nFunction Name: getTransactionDate\n");	
	ST_terminalData_t x = { 0,0, "22/1/2023" };
	EN_terminalError_t res = getTransactionDate(&x);
	printf("\n\ncase1\nInputdata: %s(less than 10)\nExpected Result: 1 (WRONG_DATE)\n Actual Result: %d (WRONG_DATE)", x.transactionDate, res);
	res = 0;
	ST_terminalData_t y = { 0,0, "" };
	res = getTransactionDate(&y);
	printf("\n\ncase2\nInputdata: %s(equal to 10)\nExpected Result: 0 (TERMINAL_OK)\n Actual Result: %d (TERMINAL_OK)",y.transactionDate, res);
}
void isCardExpiredTest(void) {
	EN_cardError_t res = 0;
	printf("\n\nTester Name: Mahmoud zain salah\nFunction Name: isCardExpried\n");
	ST_terminalData_t x = { 0,0, "" };
	getTransactionDate(&x);
	ST_cardData_t case1 = { "","","01/23" };
	res = isCardExpired(&case1, &x);
	printf("\n\ncase1\nInputdata: %s(cardExpireDate) / %s(CurrentDate)\nExpected Result: 2 (EXPIRED_CARD)\n Actual Result: %d (EXPIRED_CARD)",case1.cardExpirationDate,x.transactionDate , res );
	res = 0; //clear res
	ST_cardData_t case2 = { "","","15/22" };
	res = isCardExpired(&case2, &x);
	printf("\n\ncase2\nInputdata: %s(cardExpireDate) / %s(CurrentDate)\nExpected Result: 2 (EXPIRED_CARD)\n Actual Result: %d (EXPIRED_CARD)", case2.cardExpirationDate, x.transactionDate, res);
	res = 0; //clear res
	ST_cardData_t case3 = { "","","02/24" };
	res = isCardExpired(&case3, &x);
	printf("\n\ncase3\nInputdata: %s(cardExpireDate) / %s(CurrentDate)\nExpected Result: 0 (TERMINAL_OK)\n Actual Result: %d (TERMINAL_OK)", case3.cardExpirationDate, x.transactionDate, res);
	res = 0; //clear res
	ST_cardData_t case4 = { "","","0/224" };
	printf("\n\ncase4\nInputdata: %s(cardExpireDate) / %s(CurrentDate)\nExpected Result: \"You Have Entered A Wrong CardExpiryDate Format\"\nActual Result:", case4.cardExpirationDate, x.transactionDate);
	isCardExpired(&case4, &x);
}
void getTransactionAmountTest(void) {
	printf("\n\nTester Name: Mahmoud zain salah\nFunction Name: getTransactionAmount\n");
	EN_terminalError_t res = 0;
	ST_terminalData_t case1 = { 1, 0, ""};
	res = getTransactionAmount(&case1);
	printf("\n\ncase1\nInputdata: %f \nExpected Result: 0 (TERMINAL_OK)\nActual Result: %d (INVALID_AMOUNT)", case1.transAmount, res);
	res = 0;
	ST_terminalData_t case2 = { -1,0, "" };
	res = getTransactionAmount(&case2);
	printf("\n\ncase2\nInputdata: %f \nExpected Result: 4 (INVALID_AMOUNT)\nActual Result: %d (INVALID_AMOUNT)\n\n", case2.transAmount, res);
}
void isBelowMaxAmountTest(void) {
	printf("\n\nTester Name: Mahmoud zain salah\nFunction Name: isBelowMaxAmount\n");
	EN_terminalError_t res = 0;
	ST_terminalData_t case1 = { 50,10, "" };
	res = isBelowMaxAmount(&case1);
	printf("\n\ncase1\nInputdata: %f (MaxAmount:10)\nExpected Result: 5 (EXCEED_MAX_AMOUNT)\nActual Result: %d (EXCEED_MAX_AMOUNT)", case1.transAmount, res);
	res = 0;
	ST_terminalData_t case2 = { 10,10, "" };
	res = isBelowMaxAmount(&case2);
	printf("\n\ncase2\nInputdata: %f (MaxAmount:10)\nExpected Result: 0 (TERMINAL_OK)\nActual Result: %d (TERMINAL_OK)", case2.transAmount, res);
	res = 0;
	ST_terminalData_t case3 = { 50,100, "" };
	res = isBelowMaxAmount(&case3);
	printf("\n\ncase3\nInputdata: %f (MaxAmount:100)\nExpected Result: 0 (TERMINAL_OK)\nActual Result: %d (TERMINAL_OK)\n\n", case3.transAmount, res);
}
void setMaxAmountTest(void) {
	printf("\n\nTester Name: Mahmoud zain salah\nFunction Name: setMaxAmount\n");
	EN_terminalError_t res = 0;
	float maxAmount = 0;
	ST_terminalData_t x = { 0,0, "" };
	res = setMaxAmount(&x, maxAmount);
	printf("\n\ncase1\nInputdata: %f (MaxAmount: 0)\nExpected Result: 6 (INVALID_MAX_AMOUNT)\nActual Result: %d (INVALID_MAX_AMOUNT)", maxAmount, res);
	res = 0;
	maxAmount = -1;
	res = setMaxAmount(&x, maxAmount);
	printf("\n\ncase2\nInputdata: %f (MaxAmount: -1)\nExpected Result: 6 (INVALID_MAX_AMOUNT)\nActual Result: %d (INVALID_MAX_AMOUNT)", maxAmount, res);
	res = 0;
	maxAmount = 1;
	res = setMaxAmount(&x, maxAmount);
	printf("\n\ncase3\nInputdata: %f (MaxAmount: 1)\nExpected Result: 0 (TERMINAL_OK)\nActual Result: %d (TERMINAL_OK)\n", maxAmount, res);

}
void isValidCardPANTest(void) 
{
	printf("\n\nTester Name: Mahmoud zain salah\nFunction Name: isValidCardPAN\n");
	EN_terminalError_t res = 0;
	ST_cardData_t case1 = { "","8273123273520569","" };
	res = isValidCardPAN(&case1);
	printf("\n\ncase1\nInputdata: %s (Invalid)\nExpected Result: 3 (INVALID_CARD)\nActual Result: %d (INVALID_CARD)", case1.primaryAccountNumber, res);
	res = 0;
	ST_cardData_t case2 = { "","4539319503436467","" };
	res = isValidCardPAN(&case2);
	printf("\n\ncase1\nInputdata: %s (Valid)\nExpected Result: 0 (TERMINAL_OK)\nActual Result: %d (TERMINAL_OK)\n", case2.primaryAccountNumber, res);
}



/*int main() {

	//printf("\n\n\n\n########################################\n\n\n\n");

	//isValidCardPANTest();

	//printf("\n\n\n\n########################################\n\n\n\n");

	//isBelowMaxAmountTest();

	//("\n\n\n\n########################################\n\n\n\n");

	getTransactionDateTest();

	//("\n\n\n\n########################################\n\n\n\n");

	//getTransactionAmountTest();

	//printf("\n\n\n\n########################################\n\n\n\n");

	//setMaxAmountTest();

	//printf("\n\n\n\n########################################\n\n\n\n");

	//isCardExpiredTest();

	//printf("\n\n\n\n########################################\n\n\n\n");
	
	//ST_terminalData_t x = {0,0, ""};
	//getTransactionDate(&x);
	//ST_cardData_t y = {"","8273123273520569",""}; // 8273123273520569(invalid) / 4539319503436467 (valid)
	// 
	//getTransactionAmount(&x);
	//getCardExpiryDate(&y);
	//isCardExpired(&y, &x);
	//setMaxAmount(&x, 1.50);
	//isBelowMaxAmount(&x);
	//isValidCardPAN(&y);
	return 0;

}*/
