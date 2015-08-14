#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"

void RCC_Configuration()
{
	// RCC Configuration
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
}

void GPIO_Configuration()
{
	// GPIO Configuration
	// LED3 (Green): GPIO_Pin_13, LED4 (Red): GPIO_Pin_14
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}


void ILI9341_Configuration()
{
	/** 
  	* @brief ILI9341 Configuration
  	* 		 Do not change the following except you had known all 
  	*	 	 configurations.
  	*/ 
	LCD_Init();
	IOE_Config();
	LTDC_Cmd( ENABLE );
	LCD_LayerInit();
	LCD_SetLayer(LCD_FOREGROUND_LAYER );
	LCD_Clear(LCD_COLOR_BLACK );
	LCD_SetTextColor(LCD_COLOR_RED );


	/** 
  	* @brief Display String
  	* 		 
  	*/ 
	{
		LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"WELCOME TO TKU");
		LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)"ASFL");
	}
	
}

int main()
{
	RCC_Configuration();
	GPIO_Configuration();
	ILI9341_Configuration(1);

	// Demo Code: switch flashing two LED 
	GPIO_ToggleBits(GPIOG, GPIO_Pin_14);

	while(1)
	{
		GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
		GPIO_ToggleBits(GPIOG, GPIO_Pin_14);

		for(int i=0; i<500000; i++);
	}

	return 0;

}