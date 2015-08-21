#include "main.h"

#define L3G_Sensitivity_250dps     (float)114.285f        /*!< gyroscope sensitivity with 250 dps full scale [LSB/dps]  */
#define L3G_Sensitivity_500dps     (float)57.1429f        /*!< gyroscope sensitivity with 500 dps full scale [LSB/dps]  */
#define L3G_Sensitivity_2000dps    (float)14.285f         /*!< gyroscope sensitivity with 2000 dps full scale [LSB/dps] */




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
	/* L3GD20 Configuration */
	L3GD20_InitTypeDef L3GD20_InitStructure;
	L3GD20_FilterConfigTypeDef L3GD20_FilterStructure;
  	
  	/* Configure for L3GD20 */
	L3GD20_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;
	L3GD20_InitStructure.Output_DataRate = L3GD20_OUTPUT_DATARATE_1;
	L3GD20_InitStructure.Axes_Enable = L3GD20_AXES_ENABLE;
	L3GD20_InitStructure.Band_Width = L3GD20_BANDWIDTH_4;
	L3GD20_InitStructure.BlockData_Update = L3GD20_BlockDataUpdate_Continous;
	L3GD20_InitStructure.Endianness = L3GD20_BLE_LSB;
	L3GD20_InitStructure.Full_Scale = L3GD20_FULLSCALE_250; 
	L3GD20_Init(&L3GD20_InitStructure);
	
	/* Configure High-Pass Filter for L3GD20 */
	L3GD20_FilterStructure.HighPassFilter_Mode_Selection =L3GD20_HPM_NORMAL_MODE_RES;
	L3GD20_FilterStructure.HighPassFilter_CutOff_Frequency = L3GD20_HPFCF_0;
	L3GD20_FilterConfig(&L3GD20_FilterStructure);

	/* Configure High-Pass Filter for L3GD20 */	
	L3GD20_FilterCmd(L3GD20_HIGHPASSFILTER_ENABLE);
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
    LCD_ClearLine(LCD_LINE_3);
    LCD_DisplayStringLine(LCD_LINE_3, (uint8_t*)str_output_buffer); 

	strcpy(str_output_buffer, "Y-AXIS:");
    sprintf(str_convert_buffer, "%4.2f", 12.0 );
    strcat(str_output_buffer, str_convert_buffer); 
    LCD_ClearLine(LCD_LINE_4);
    LCD_DisplayStringLine(LCD_LINE_4, (uint8_t*)str_output_buffer); 

   	strcpy(str_output_buffer, "Z-AXIS:");
    sprintf(str_convert_buffer, "%4.2f", 14.0 );
    strcat(str_output_buffer, str_convert_buffer); 
    LCD_ClearLine(LCD_LINE_5);
    LCD_DisplayStringLine(LCD_LINE_5, (uint8_t*)str_output_buffer); 

}

void L3GD20_ReadGyro(float* pfData)
{
  uint8_t tmpbuffer[6] ={0};
  int16_t RawData[3] = {0};
  uint8_t tmpreg = 0;
  float sensitivity = 0;
  int i =0;
  
  L3GD20_Read(&tmpreg,L3GD20_CTRL_REG4_ADDR,1);
  
  L3GD20_Read(tmpbuffer,L3GD20_OUT_X_L_ADDR,6);
  
  /* check in the control register 4 the data alignment (Big Endian or Little Endian)*/
  if(!(tmpreg & 0x40))
  {
    for(i=0; i<3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i+1] << 8) + tmpbuffer[2*i]);
    }
  }
  else
  {
    for(i=0; i<3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i] << 8) + tmpbuffer[2*i+1]);
    }
  }
  
  /* Switch the sensitivity value set in the CRTL4 */
  switch(tmpreg & 0x30)
  {
  case 0x00:
    sensitivity=L3G_Sensitivity_250dps;
    break;
    
  case 0x10:
    sensitivity=L3G_Sensitivity_500dps;
    break;
    
  case 0x20:
    sensitivity=L3G_Sensitivity_2000dps;
    break;
  }
  /* divide by sensitivity */
  for(i=0; i<3; i++)
  {
  pfData[i]=(float)RawData[i]/sensitivity;
  }
}

void L3GD20_ReadBias(float* pfData)
{
   float Bias[3] = {0};
   float GyroData[3] = {0};

   for( int i = 0; i < 500; i++ )
   {
    	L3GD20_ReadGyro(GyroData);

    	for (int j = 0; j < 3; j++ )
   		{
   			Bias[j] += GyroData[j];
   		}
   }

   for (int k = 0; k < 3; k++ )
   {
   		pfData[k] = (float)Bias[k] / 100.0;
   }
}




int main()
{

	float GyroData[3];
	char str_convert_buffer[16];
	char str_output_buffer[32];

	RCC_Configuration();
	GPIO_Configuration();
	ILI9341_Configuration();
	
	ILI9341_Show();
	L3GD20_ReadBias(GyroData);

	strcpy(str_output_buffer, "X-BIAS:");
    sprintf(str_convert_buffer, "%4.2f", GyroData[0] );
    strcat(str_output_buffer, str_convert_buffer); 
    LCD_ClearLine(LCD_LINE_7);
    LCD_DisplayStringLine(LCD_LINE_7, (uint8_t*)str_output_buffer); 

    strcpy(str_output_buffer, "Y-BIAS:");
    sprintf(str_convert_buffer, "%4.2f", GyroData[1] );
    strcat(str_output_buffer, str_convert_buffer); 
    LCD_ClearLine(LCD_LINE_8);
    LCD_DisplayStringLine(LCD_LINE_8, (uint8_t*)str_output_buffer); 

    strcpy(str_output_buffer, "Z-BIAS:");
    sprintf(str_convert_buffer, "%4.2f", GyroData[2] );
    strcat(str_output_buffer, str_convert_buffer); 
    LCD_ClearLine(LCD_LINE_9);
    LCD_DisplayStringLine(LCD_LINE_9, (uint8_t*)str_output_buffer); 

	// Demo Code: switch flashing two LED 
	GPIO_ToggleBits(GPIOG, GPIO_Pin_14);

	while(1)
	{
		//show();
		GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
		GPIO_ToggleBits(GPIOG, GPIO_Pin_14);


		for(int i=0; i<500000; i++);
	}

	return 0;

}