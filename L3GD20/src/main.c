#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_l3gd20.h"
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

void L3GD20_Configuration()
{
	// L3GD20 Configuration
	L3GD20_InitTypeDef L3GD20_InitStructure;
	L3GD20_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;
	L3GD20_InitStructure.Output_DataRate = L3GD20_OUTPUT_DATARATE_1;
	L3GD20_InitStructure.Axes_Enable = L3GD20_AXES_ENABLE;
	L3GD20_InitStructure.Band_Width = L3GD20_BANDWIDTH_4;
	L3GD20_InitStructure.BlockData_Update = L3GD20_BlockDataUpdate_Continous;
	L3GD20_InitStructure.Endianness = L3GD20_BLE_LSB;
	L3GD20_InitStructure.Full_Scale = L3GD20_FULLSCALE_250;
	L3GD20_Init(&L3GD20_InitStructure);

	// L3GD20 Filter Configuration
	L3GD20_FilterConfigTypeDef L3GD20Filter_InitStructure;
	L3GD20Filter_InitStructure.HighPassFilter_Mode_Selection = L3GD20_HPM_NORMAL_MODE;
	L3GD20Filter_InitStructure.HighPassFilter_CutOff_Frequency = L3GD20_HPFCF_4;
	L3GD20_FilterConfig(&L3GD20Filter_InitStructure);

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
	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"WELCOME TO TKU");
	LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"ASFL");
	LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)"X: ");
	LCD_DisplayStringLine(LCD_LINE_5, (uint8_t *)"Y: ");
	LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *)"Z: ");

}

static char* itoa(int value, char* result, int base)
{
	if (base < 2 || base > 36) {
		*result = '\0';
		return result;
	}
	char *ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while (value);

	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

int main()
{
	RCC_Configuration();
	GPIO_Configuration();
	L3GD20_Configuration();
	ILI9341_Configuration();

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

