#include "stm32f0xx.h"
#include "main.h" 
#include <string.h>

extern volatile uint8_t send , CMD_length , String_length;
volatile uint8_t BT_Tx_complete, BT_Rx_Complete, BT_send_number, BT_data_length;
volatile uint8_t BT_temp;
char string1[]="keypad 1 is pressed.\n\r";
#define BufferMax 54
volatile char BT_Buffer[37],BT_receivePtr;
extern volatile char receiveBuffer[BufferMax],receivePtr;
//-------------------------------------
void Config_PA9PA10(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &= ~(GPIO_MODER_MODER9 | GPIO_MODER_MODER10);
	GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;
	GPIOA->AFR[1] &= ~ ( 0xful << 4 | 0xful << 4*2);	
	GPIOA->AFR[1] |= 0x1ul << 4 | 0x1ul << 4*2;
}
//---------------------------
void Config_Usart1(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->BRR = 48000000/9600;
	USART1->CR3 |= USART_CR3_OVRDIS;
	//USART1->CR1 = USART_CR1_UE |  USART_CR1_TE | USART_CR1_TCIE;
	USART1->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE | USART_CR1_TCIE;
	USART1->ICR = USART_ICR_TCCF;
	NVIC_ClearPendingIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn,3);
	NVIC_EnableIRQ(USART1_IRQn);
}

/*-----------------------------------------------
Send AT CMD to BT
-----------------------------------------------*/
void Send_String_Usart1(char *String_ptr)
{
	BT_Tx_complete = 0;
	BT_send_number = 0;
	
	BT_data_length = strlen(String_ptr);
	do{
		if(BT_send_number == BT_data_length){
			BT_Tx_complete = 1;
		}
		else if(USART1->ISR & USART_ISR_TXE)
		{
			USART1->TDR = *(String_ptr++);
			BT_send_number++;
		}
	}while(BT_Tx_complete==0);
}
//-------------------------------

void USART1_IRQHandler(char *BTATCMD_ptr)
{
	if((USART1->ISR & (USART_ISR_TC))==(USART_ISR_TC))
	{
		USART1->ICR |= USART_ISR_TC;
		if(BT_send_number == BT_data_length)
		{
			BT_Tx_complete = 1;
		}
	}
	if((USART1->ISR & (USART_ISR_RXNE))==(USART_ISR_RXNE))
	{
		BT_temp = (uint8_t) USART1->RDR;
		USART2->TDR = BT_temp;
		BT_Buffer[BT_receivePtr]=BT_temp;
		BT_receivePtr++;
		if(BT_receivePtr >= BufferMax)
		{
			BT_receivePtr = 0;
			BT_Rx_Complete = 1;
		}
	}
}
//-------------------------------
void BT_send(void)
{/*
	for(int i=0;i<sizeof(receiveBuffer);i++)
	{
		printf("%c",receiveBuffer[i]);
	}*/
	for(int i=18;i<sizeof(receiveBuffer);i++)
	{
		BT_Buffer[i-17]=receiveBuffer[i];
		printf("%c",BT_Buffer[i-18]);
	}
	Send_String_Usart1( (char*) BT_Buffer);
}
