/*
 * File:   card.c
 * Author: mahmoud zain el abidean salah
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "card.h"


EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	
	int chr_len = 0;
	// make sure structure variable is empty 
	if (cardData->cardHolderName[0] == 0 || cardData->cardHolderName[0] == "\0")
	{
	printf("\nPlease Enter The Cardholder's Name: ");
	scanf_s("%[^\n]%*c", cardData->cardHolderName, 25);//%[^\n]%*c
	}

	chr_len = (int)strlen(cardData->cardHolderName);
	// make sure char length is not equal to 0 or bigger than 24 or lesser than20 
	if (chr_len == 0 || chr_len > 24 || chr_len < 20) 
	{
		return WRONG_NAME;
	}
	else 
	{
		return CARD_OK;
	}
}



EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {

	int dat_len = 0;
	uint8_t form[] = "/";
	// make sure structure variable is empty 
	if (cardData->cardExpirationDate[0] == 0) 
	{
		printf("\nPlease Enter The card expiry date: (MM/YY)\n");
		scanf_s("%[^\n]%*c",cardData->cardExpirationDate, 6);
	}

	dat_len = (int)strlen(cardData->cardExpirationDate);

	// make sure that the string third  index = to / &  string length = to 5
	if (cardData->cardExpirationDate[2] == form[0] && dat_len == 5) 
	{
		return CARD_OK;
	}
	else
	{
		return WRONG_EXP_DATE;
	}
}



EN_cardError_t getCardPAN(ST_cardData_t* cardData) {

	int chr_len = 0;
	// make sure structure variable is empty 
	if (cardData->primaryAccountNumber[0] == 0) 
	{
	printf("\nPlease Enter The Card's Primary Account Number: ");
	scanf_s("%[^\n]%*c", cardData->primaryAccountNumber,20);
	}

	chr_len = (int)strlen(cardData->primaryAccountNumber);
	// make string length bigger than 0 & lesser than or = to 19 & bigger than or = to 16
	if (chr_len > 0 && chr_len <= 19 && chr_len >=16 ) {
		return CARD_OK;
	}
	else {
		return WRONG_PAN;
	}

}

void getCardHolderNameTest(void) {
	printf("\nTester Name: Mahmoud zain salah\nFunction Name: getCardHolderName\n");
	// already declared Name
	ST_cardData_t Case_1 = { "ahmed", "", "" };
	// save the fuction output in a variable and present it's output in the resute
	EN_cardError_t ACT_R1= getCardHolderName(&Case_1);
	printf("\nTest case 1 \nInput Data: %s (5 characters) \nExpected Result: 1 (WRONG_NAME)\nActual Result: %d (WRONG_NAME)\n",Case_1.cardHolderName, ACT_R1);
	printf("\n-----------------\n");

	ST_cardData_t Case_2 = { "mahmoud zainn elabiden di", "", "" };
	EN_cardError_t ACT_R2 = getCardHolderName(&Case_2);
	printf("\nTest case 2 \nInput Data: %s (25 characters) \nExpected Result: 1 (WRONG_NAME)\nActual Result: %d (WRONG_NAME)\n",Case_2.cardHolderName, ACT_R2);
	printf("\n-----------------\n");

	ST_cardData_t Case_3 = { " ", "", "" };
	EN_cardError_t ACT_R3 = getCardHolderName(&Case_3);
	printf("\nTest case 3 \nInput Data: %s (0 characters) \nExpected Result: 1 (WRONG_NAME)\nActual Result: %d (WRONG_NAME)\n", Case_3.cardHolderName, ACT_R3);
	printf("\n-----------------\n");

	ST_cardData_t Case_4 = { "mahmoud zain elabiden", "", "" };
	EN_cardError_t ACT_R4 = getCardHolderName(&Case_4);
	printf("\nTest case 4 \nInput Data: %s (21 characters) \nExpected Result: 0 (CARD_OK)\nActual Result: %d (CARD_OK)\n", Case_4.cardHolderName, ACT_R4);
}
void getCardExpiryDateTest(void) {
	printf("\nTester Name: Mahmoud zain salah\nFunction Name: getCardExpiryDate\n");
	ST_cardData_t Case_1 = { "", "", " " };
	EN_cardError_t ACT_R1 = getCardExpiryDate(&Case_1);
	printf("\nTest case 1 \nInput Data: %s (0 characters) \nExpected Result: 2 (WRONG_EXP_DATE)\nActual Result: %d (WRONG_EXP_DATE)\n", Case_1.cardExpirationDate, ACT_R1);
	printf("\n-----------------\n");

	ST_cardData_t Case_2 = { "", "", "17/202" };
	EN_cardError_t ACT_R2 = getCardExpiryDate(&Case_2);
	printf("\nTest case 2 \nInput Data: 17/202 (6 characters) \nExpected Result: 2 (WRONG_EXP_DATE)\nActual Result: %d (WRONG_EXP_DATE)\n", ACT_R2);
	printf("\n-----------------\n");

	ST_cardData_t Case_3 = { "", "", "7/23" };
	EN_cardError_t ACT_R3 = getCardExpiryDate(&Case_3);
	printf("\nTest case 3 \nInput Data: %s (4 characters) \nExpected Result: 2 (WRONG_EXP_DATE)\nActual Result: %d (WRONG_EXP_DATE)\n", Case_3.cardExpirationDate, ACT_R3);
	printf("\n-----------------\n");

	ST_cardData_t Case_4 = { "", "", "1/723" };
	EN_cardError_t ACT_R4 = getCardExpiryDate(&Case_4);
	printf("\nTest case 4 \nInput Data: %s (5 characters & WrongFormat) \nExpected Result: 2 (WRONG_EXP_DATE)\nActual Result: %d (WRONG_EXP_DATE)\n", Case_4.cardExpirationDate, ACT_R4);
	
	ST_cardData_t Case_5 = { "", "", "10/23" };
	EN_cardError_t ACT_R5 = getCardExpiryDate(&Case_5);
	printf("\nTest case 4 \nInput Data: %s (5 characters & Correct Format) \nExpected Result: 0 (CARD_OK)\nActual Result: %d (CARD_OK)\n", Case_5.cardExpirationDate, ACT_R5);
}
void getCardPANTest(void) {
	printf("\nTester Name: Mahmoud zain salah\nFunction Name: getCardPAN\n");
	ST_cardData_t Case_1 = { "", " ", "" };
	EN_cardError_t ACT_R1 = getCardPAN(&Case_1);
	printf("\nTest case 1 \nInput Data: %s (0 characters) \nExpected Result: 3 (WRONG_PAN)\nActual Result: %d (WRONG_PAN)\n", Case_1.primaryAccountNumber, ACT_R1);
	printf("\n-----------------\n");

	ST_cardData_t Case_2 = { "", "123456789101112", "" };
	EN_cardError_t ACT_R2 = getCardPAN(&Case_2);
	printf("\nTest case 2 \nInput Data: %s (15 characters) \nExpected Result: 3 (WRONG_PAN)\nActual Result: %d (WRONG_PAN)\n",Case_2.primaryAccountNumber, ACT_R2);
	printf("\n-----------------\n");

	ST_cardData_t Case_3 = { "", "12345678910111237893", "" };
	EN_cardError_t ACT_R3 = getCardPAN(&Case_3);
	printf("\nTest case 3 \nInput Data: %s (21 characters) \nExpected Result: 3 (WRONG_PAN)\nActual Result: %d (WRONG_PAN)\n", Case_3.primaryAccountNumber, ACT_R3);
	printf("\n-----------------\n");

	ST_cardData_t Case_4 = { "", "1234567891011123789", "" };
	EN_cardError_t ACT_R4 = getCardPAN(&Case_4);
	printf("\nTest case 4 \nInput Data: %s (19 characters) \nExpected Result: 0 (CARD_OK)\nActual Result: %d (CARD_OK)\n", Case_4.primaryAccountNumber, ACT_R4);

	ST_cardData_t Case_5 = { "", "1234567891011123", "" };
	EN_cardError_t ACT_R5 = getCardPAN(&Case_5);
	printf("\nTest case 4 \nInput Data: %s (16 characters) \nExpected Result: 0 (CARD_OK)\nActual Result: %d (CARD_OK)\n", Case_5.primaryAccountNumber, ACT_R5);
}

/*int main() {
	//ST_cardData_t x = { "","","" };

	//ST_cardData_t y = { "ahmed","","" };
	
	//getCardHolderName(&x);

	//printf("\n\n\n\n########################################\n\n\n\n");
	//getCardHolderNameTest();
	//printf("\n\n\n\n########################################\n\n\n\n");

	//getCardExpiryDate(&x);
	//printf("\n\n\n\n########################################\n\n\n\n");

	//getCardExpiryDateTest();
//	printf("\n\n\n\n########################################\n\n\n\n");

	//getCardPAN(&x);
//	printf("\n\n\n\n########################################\n\n\n\n");

	//getCardPANTest();
//	printf("\n\n\n\n########################################\n\n\n\n");

	//return 0;
}*/