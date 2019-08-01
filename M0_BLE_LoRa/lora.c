#include "stm32f0xx.h"
#include "main.h" 
#include <string.h>

#define BufferMax 54
volatile uint8_t receiveChar=0;
volatile uint8_t Tx_complete,Rx_Complete, send_number, data_length,receiveBuffer[BufferMax],receivePtr;
volatile uint8_t temp;
volatile char ATCommand1[]="AT\n\r";
volatile char ATCommand2[]="AT+IPR=9600\n\r";
volatile char ATCommand3[]="AT+PARAMETER=12,3,1,7\n\r";
volatile char ATCommand4[]="AT+BAND=89600000\n\r";
volatile char ATCommand5[]="AT+IPR=9600\n\r";
volatile char ATCommand6[]="AT+ADDRESS=2\n\r";
volatile char ATCommand7[]="AT+NETWORKID=6\n\r";
//char ATCommand[]="AT+PARAMETER=10,7,1,7\n\r";  for shortrange <3km
//char ATCommand[]="AT+PARAMETER=12,3,1,7\n\r";  for longrange >11km
extern volatile void Send_String_Usart1(char *String_ptr);
extern volatile void USART1_IRQHandler(char *BTATCMD_ptr);
extern void BT_send(void);
//--------------------------------------
void Config_USART3_GPIOB10B11(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER &= ~(GPIO_MODER_MODER10 | GPIO_MODER_MODER11);
	GPIOB->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;
	GPIOB->AFR[1] &= ~(0xful << 2*4 | 0xful << 3*4);
	GPIOB->AFR[1] |= 0x4ul << 2*4 | 0x4ul << 3*4;
}
//--------------------------------------
void Config_Usart3(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	USART3->BRR = 48000000/ 9600;
	USART3->CR3 |= USART_CR3_OVRDIS;
	USART3->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE | USART_CR1_TCIE;
	USART3->ICR = USART_ICR_TCCF;
	NVIC_ClearPendingIRQ(USART3_4_IRQn);
	NVIC_SetPriority(USART3_4_IRQn,4);
	NVIC_EnableIRQ(USART3_4_IRQn);
}
//-------------------------------
void USART3_4_IRQHandler(void)
{	
	if((USART3->ISR & (USART_ISR_TC))==(USART_ISR_TC))
	{
		USART3->ICR |= USART_ISR_TC;
		if(send_number == data_length)
		{
			Tx_complete = 1;
		}
	}
	if((USART3->ISR & (USART_ISR_RXNE))==(USART_ISR_RXNE))
	{
		temp = (uint8_t) USART3->RDR;
		//USART2->TDR = temp;
		receiveBuffer[receivePtr]=temp;
		receivePtr++;
		if(receivePtr >= sizeof(receiveBuffer))
		{
			BT_send();
			receivePtr = 0;
			Rx_Complete = 1;
		}
	}
}

//-------------------------------
void Send_String_Usart3(char *String_ptr)
{
	
	uint8_t length;

	Tx_complete = 0;
	send_number = 0;
	length = strlen (String_ptr);
	length--;
	do
	{
		if(send_number == length)
		{
			Tx_complete = 1;
		}
		else if(USART3->ISR & USART_ISR_TXE )
		{
			 USART3->TDR = *(String_ptr);
			 String_ptr++;
			 send_number++;
		}
	} while (Tx_complete ==0);
}
//-------------------------------
void LoRa_ATCommand_Setting(void)
{
	Send_String_Usart3( (char*) ATCommand1);
	Send_String_Usart3( (char*) ATCommand2);
	Send_String_Usart3( (char*) ATCommand3);
	Send_String_Usart3( (char*) ATCommand4);
	Send_String_Usart3( (char*) ATCommand5);
	Send_String_Usart3( (char*) ATCommand6);
	Send_String_Usart3( (char*) ATCommand7);
}

