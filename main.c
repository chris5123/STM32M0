#include <string.h>
#include "main.h" 
#include "Board_LED.h"                 
#include "Board_Buttons.h"   
#include "stm32f0xx.h"

extern void Config_USART3_GPIOB10B11(void);
extern void Config_Usart3(void);
extern void Send_BTATCMD(void);
extern void USART3_4_IRQHandler(void);
extern void Config_PA9PA10(void);
extern void Config_Usart1(void);
extern void USART1_IRQHandler(void);
extern void Send_String_Usart3(void);
extern void LoRa_ATCommand_Setting(void);
extern volatile uint8_t Rx_Complete;
extern volatile uint32_t button_number;
extern void BT_send(void);


//--------------------------------------------------------------------------------------

int main(void)
{
	int32_t max_num = LED_GetCount();
  int32_t num = 0;
	
	SetSysClock();
	
	stdout_init();
	
	SysTick_Config(SystemCoreClock/1000);
	NVIC_SetPriority(SysTick_IRQn,1);
	printf("\n\rMain function\n\r");
	
  LED_Initialize();
  Buttons_Initialize();
	
	Config_Usart1();
	Config_PA9PA10();
	Config_USART3_GPIOB10B11();
	Config_Usart3();
	LoRa_ATCommand_Setting();
	for(;;)
	{
		if((USART3->ISR & (USART_ISR_RXNE))!=(USART_ISR_RXNE))
		{
				LED_On(num);                                           // Turn specified LED on
				Delay(1000);                                            // Wait 1000ms
				//BT_send();
				LED_Off(num);                                          // Turn specified LED off
				Delay(300);
		}
		else
		{
			printf("\n\rThe received data is error.\n\r");						//lora not detected
			Delay(5000);
		}
	}
}
