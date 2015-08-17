#include "main.h"

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
	LTDC_Cmd( ENABLE );
	LCD_LayerInit();
	LCD_SetLayer(LCD_FOREGROUND_LAYER );
	LCD_Clear(LCD_COLOR_BLACK );
	LCD_SetTextColor(LCD_COLOR_BLACK );

}


void ILI9341_Show()
{

	/** 
  	* @brief Display String
  	* 		 
  	*/ 

	char str_convert_buffer[16];
	char str_output_buffer[32];

	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"WELCOME TO TKU");
    LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"ASFL");

	strcpy(str_output_buffer, "X-AXIS:");
    sprintf(str_convert_buffer, "%4.2f",10.0 );
    strcat(str_output_buffer, str_convert_buffer); 
    LCD_ClearLine(LCD_LINE_6);
    LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*)str_output_buffer); 

	strcpy(str_output_buffer, "Y-AXIS:");
    sprintf(str_convert_buffer, "%4.2f", 12.0 );
    strcat(str_output_buffer, str_convert_buffer); 
    LCD_ClearLine(LCD_LINE_7);
    LCD_DisplayStringLine(LCD_LINE_7, (uint8_t*)str_output_buffer); 

   	strcpy(str_output_buffer, "Z-AXIS:");
    sprintf(str_convert_buffer, "%4.2f", 14.0 );
    strcat(str_output_buffer, str_convert_buffer); 
    LCD_ClearLine(LCD_LINE_8);
    LCD_DisplayStringLine(LCD_LINE_8, (uint8_t*)str_output_buffer); 

}

int main()
{
	RCC_Configuration();
	GPIO_Configuration();
	ILI9341_Configuration();
	ILI9341_Show();
	// Demo Code: switch flashing two LED 
	GPIO_ToggleBits(GPIOG, GPIO_Pin_14);

	while(1)
	{
		//show();
		GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
		GPIO_ToggleBits(GPIOG, GPIO_Pin_14);

		for(int i=0; i<10000000; i++);
	}

	return 0;

}

