#include <stdio.h>
#include <stdint.h>

#include "stm32f0xx.h"          // File name depends on device used
#include "RTE_Components.h"      // Component selection 

extern void stdout_init (void);
extern void SetSysClock(void);
extern void Delay(uint32_t dlyTicks);


#define Key_1 1
#define Key_2 2
#define Key_3 3
#define Key_4 4
#define Key_default 10

#define Stop_Song				'0'
#define Happy_Song 			'1'
#define LittleBee_Song	'2'
#define LittleStar_Song	'3'
#define New_Song		  	'4'




#define Car_Stop 		'p'
#define Car_Forward	'f'
#define Car_Left		'l'
#define Car_Right		'r'
#define Car_Back		'b'


