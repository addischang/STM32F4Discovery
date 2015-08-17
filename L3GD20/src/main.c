/**
  ******************************************************************************
  * @file    main.c
  * @author  Chang, Wei-Chieh
  * @version V1.0.0
  * @date    17-Augest-2015
  * @brief   This main file to read gyroscope value. 
  ******************************************************************************
  * @license
  *
  * The MIT License
  *
  * Copyright (c) 2015 Avionics and Flight Simulation Laboratory
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy 
  * of this software and associated documentation files (the "Software"), to deal 
  * in the Software without restriction, including without limitation the rights 
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
  * of the Software, and to permit persons to whom the Software is furnished to do so, 
  * subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all 
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
  * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
  * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE 
  * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
  * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
  * OR OTHER DEALINGS IN THE SOFTWARE.
  *
  ******************************************************************************
  */

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


void L3GD20_Configuration()
{
	/*
	* @brief The L3GD20, 3-Axis MEMS gyroscope, configurations.
	*
	*/
	L3GD20_InitTypeDef L3GD20_InitStructure;
	L3GD20_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;
	L3GD20_InitStructure.Output_DataRate = L3GD20_OUTPUT_DATARATE_1;
	L3GD20_InitStructure.Axes_Enable = L3GD20_AXES_ENABLE;
	L3GD20_InitStructure.Band_Width = L3GD20_BANDWIDTH_4;
	L3GD20_InitStructure.BlockData_Update = L3GD20_BlockDataUpdate_Continous;
	L3GD20_InitStructure.Endianness = L3GD20_BLE_LSB;
	L3GD20_InitStructure.Full_Scale = L3GD20_FULLSCALE_250;
	L3GD20_Init(&L3GD20_InitStructure);

	/*
	* @brief The reboot command
	*		
	*/
	L3GD20_RebootCmd();

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
	uint8_t tmpreg;
	tmpreg = L3GD20_GetDataStatus();
	/** 
  	* @brief Display String
  	* 		 
  	*/ 

	char str_convert_buffer[16];
	char str_output_buffer[32];

	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"WELCOME TO TKU");
    LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"ASFL");

	strcpy(str_output_buffer, "X-AXIS:");
    sprintf(str_convert_buffer, "%d", tmpreg );
    strcat(str_output_buffer, str_convert_buffer); 
    LCD_ClearLine(LCD_LINE_6);
    LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*)str_output_buffer); 

	strcpy(str_output_buffer, "Y-AXIS:");
    sprintf(str_convert_buffer, "%d", 10 );
    strcat(str_output_buffer, str_convert_buffer); 
    LCD_ClearLine(LCD_LINE_7);
    LCD_DisplayStringLine(LCD_LINE_7, (uint8_t*)str_output_buffer); 

   	strcpy(str_output_buffer, "Z-AXIS:");
    sprintf(str_convert_buffer, "%d", 10 );
    strcat(str_output_buffer, str_convert_buffer); 
    LCD_ClearLine(LCD_LINE_8);
    LCD_DisplayStringLine(LCD_LINE_8, (uint8_t*)str_output_buffer); 

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
		ILI9341_Show();

		GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
		GPIO_ToggleBits(GPIOG, GPIO_Pin_14);

		for(int i=0; i<1000000; i++);
	}

	return 0;

}

