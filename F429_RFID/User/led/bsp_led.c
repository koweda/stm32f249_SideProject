/**
  ******************************************************************************
  * @file    bsp_led.c
  *
  ******************************************************************************
  */
  
#include "./led/bsp_led.h"   

 /**
  * @brief  ��l�Ʊ���LED��IO
  * @param  �L
  * @retval �L
  */
void LED_GPIO_Config(void)
{		
		/*�w�q�@��GPIO_InitTypeDef���������c��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*�}��LED������GPIO�~�]�ɯ�*/
		RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|LED2_GPIO_CLK|LED3_GPIO_CLK|LED4_GPIO_CLK, ENABLE); 

		/*��ܭn���GPIO�޸}*/															   
		GPIO_InitStructure.GPIO_Pin = LED1_PIN;	

		/*�]�m�޸}�Ҧ�����X�Ҧ�*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*�]�m�޸}����X������������X*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*�]�m�޸}���W�ԼҦ��A�w�]LED�G*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*�]�m�޸}�t�v��50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*�եήw��ơA�ϥΤW���t�m��GPIO_InitStructure��l��GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
    
    /*��ܭn���GPIO�޸}*/															   
		GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
    
    /*��ܭn���GPIO�޸}*/															   
		GPIO_InitStructure.GPIO_Pin = LED3_PIN;	
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);	
		
		/*��ܭn���GPIO�޸}*/															   
		GPIO_InitStructure.GPIO_Pin = LED4_PIN;	
    GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStructure);	
		
		/*����RGB�O*/
		LED_RGBOFF;
		
		/*���ܿO�q�{�}��*/
		LED4(ON);
		
}
/*********************************************END OF FILE**********************/
