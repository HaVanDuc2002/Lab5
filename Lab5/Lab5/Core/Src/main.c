// connect LEDs to PA5-PA0, switches to PB5-3, walk LED PA67
// east/west red light connected to bit 5
// east/west yellow light connected to bit 4
// east/west green light connected to bit 3
// north/south facing red light connected to bit 2
// north/south facing yellow light connected to bit 1
// north/south facing green light connected to bit 0
// north/south car detector connected bit 5 (1=car present)
// east/west car detector connected to bit 4 (1=car present)
// pedestrian detector connected to bit 3 (1=pedestrian present)
// "walk" light connected to PA7 (built-in green LED)
// "don't walk" light connected to PA6 (built-in red LED)
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"

void SystemClock_Config(void);
static void GPIO_Config(void);

struct State{
	
	uint32_t 	output;
	uint32_t 	time;
	uint32_t 	next[8];

};

typedef struct State StateType;

#define 		goW			0 // go west
#define 		waitW		1 // wait west
#define 		as1			2 // all stop 1
#define 		goS			3 // go south
#define 		waitS		4 // go south
#define 		as2			5 // all stop 2
#define 		walk		6
#define 		on1			7
#define 		off1		8
#define 		on2			9
#define 		off2		10
#define 		as3			11 // allstop 3

StateType FSM[12] = {
	{0x4C, 5000, {goW,waitW,goW,waitW,waitW,waitW,waitW,waitW}},
	{0x54, 1000, {goW,as1,as1,as1,as1,as1,as1,as1}},
	{0x64, 1000, {goS,walk,goS,walk,goS,goS,goS,goS}},
	{0x61, 5000, {goS,waitS,waitS,waitS,goS,waitS,waitS,waitS}},
	{0x62, 1000, {goS,as2,as2,as2,as2,as2,as2,as2}},
	{0x64, 1000, {goW,walk,goW,walk,goW,walk,goW,walk}},
	{0xA4, 5000, {on1,walk,on1,on1,on1,on1,on1,on1}},
	{0x64, 500,  {off1,off1,off1,off1,off1,off1,off1,off1}},
	{0x24, 500,  {on2,on2,on2,on2,on2,on2,on2,on2}},
	{0x64, 500,  {off2,off2,off2,off2,off2,off2,off2,off2}},
	{0x24, 500,  {as3,walk,as3,as3,as3,as3,as3,as3}},
	{0x64, 1000, {goW,walk,goW,goW,goS,goS,goW,goW}}
};

int main(void)
{

  HAL_Init();
  SystemClock_Config();
  GPIO_Config();

	uint32_t S = goW;
	uint32_t input;
  while (1)
  {
		//write output to port A
		GPIOA->ODR = FSM[S].output;
		
		// set time 
		HAL_Delay(FSM[S].time);
		
		//get input
		input = ((GPIOB->IDR)>>7)&0x07;
		
		//change state
		S = FSM[S].next[input];

  }

}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();


  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);


  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, 5);
  

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


static void GPIO_Config(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	

}

