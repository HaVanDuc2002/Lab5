#include <stdint.h>
#include "dac.h"
#include "key.h"
#include "sound.h"

#define noteD 	425 //key0, PB6
#define noteGb 	338 //key1, PB7
#define noteA 	284 //key2, PB8
#define noteB 	253 //key3, PB9
#define stop		1

void SystemClock_Config(void);

int main(void)
{

  HAL_Init();
  SystemClock_Config();
	Key_Init();
	Sound_Init();
	
	while(1){
		
		switch (Key_In()){																
			case 0x01:
				Sound_Start(noteD);
				break;
			case 0x02:
				Sound_Start(noteGb);
				break;
			case 0x04:
				Sound_Start(noteA);
				break;
			case 0x08:
				Sound_Start(noteB);
				break;
			default:
				Sound_Start(stop);
		}
	}
}

void SystemClock_Config(void)
{
	
  RCC_OscInitTypeDef RCC_OscInitStruct ;
  RCC_ClkInitTypeDef RCC_ClkInitStruct ;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
 
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
  
}



  

