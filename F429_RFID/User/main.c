  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "rc522_config.h"
#include "rc522_function.h"
#include <stdbool.h>
#include "./font/fonts.h"
#include "./lcd/bsp_lcd.h"
#include "./systick/bsp_SysTick.h"
#include "./led/bsp_led.h"
#include "./beep/bsp_beep.h"
#include "./sdram/bsp_sdram.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


	
int person=0;
int mode=0;
int i,j,k,l;
int z=0;
int g;
void delay(){
	for( i=0;i<=0x1fffff; i++){	
		
	}
}

void Delay(__IO uint32_t nCount)	 
{
	for(; nCount != 0; nCount--);
}
void IC_test ( void )
{
	//char cStr [ 30 ];
	char cStr2 [ 40 ];
	char cStr3 [ 4 ];
  u8 ucArray_ID [ 4 ]; /*save IC card UID*/                                                                                            
	u8 ucStatusReturn;   /*Return UID*/                                                                                           
	u8 status_check[10];
	int status_loop=0;
  static u8 ucLineCount = 0;
	
	for (j=0;j<10;j++){
		status_check[j]=0;
	}
		
  while ( 1 )
  { 
    /*Find Card*/
		
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )  
      /*Faild find again*/
			ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		                                                

		if ( ucStatusReturn == MI_OK  ){
      /*Prevent conflict*/
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK ){				
				/*
				sprintf ( cStr, "%02X%02X%02X%02X",
                  ucArray_ID [ 0 ],
                  ucArray_ID [ 1 ],
                  ucArray_ID [ 2 ], 
                  ucArray_ID [ 3 ]
									);
				*/
				if(ucArray_ID[ 0 ] == 52 & ucArray_ID[ 1 ]==198& ucArray_ID[ 2 ]==102& ucArray_ID[ 3 ]==146 ){
					LCD_SetTextColor(LCD_COLOR_GREEN);
					LCD_DisplayStringLine(LINE(ucLineCount), (uint8_t* )"ID checked");
				}
				else{
					LCD_SetTextColor(LCD_COLOR_RED);
					LCD_DisplayStringLine(LINE(ucLineCount), (uint8_t* )"Unknow  ID");
				}
			  
				ucLineCount ++;
				if ( ucLineCount == 12 ) ucLineCount = 0;
			}
			
		}
		
		
		
		if(ucArray_ID[ 0 ] == 52 & ucArray_ID[ 1 ]==198& ucArray_ID[ 2 ]==102& ucArray_ID[ 3 ]==146 ){
			
			status_check[status_loop]=1;
			g= status_loop;
			if(g==0){
				g=9;
			}
			if(status_check[g-1]==0){
				
				if(mode==1){
					mode=0;
				}							
				else if(mode==0){
					mode=1;
					
				}
				
			}
			for (k=0;k<4;k++){
				ucArray_ID[k]=0;
			}
		}
		else{
			status_check[status_loop]=0;
			
		}
		
		status_loop++;
		if ( status_loop == 10 ) status_loop = 0;
		
		sprintf ( cStr2, "%X%X%X%X%X%X%X%X%X%X",
                  status_check [ 0 ],
                  status_check [ 1 ],
                  status_check [ 2 ], 
                  status_check [ 3 ], 
                  status_check [ 4 ], 
                  status_check [ 5 ], 
                  status_check [ 6 ], 
                  status_check [ 7 ], 
                  status_check [ 8 ], 
                  status_check [ 9 ]
									);
		sprintf ( cStr3, "%d",mode);
		//LCD_DisplayStringLine(LINE(15), (uint8_t* )cStr2);
		if(mode==1){
			LCD_SetTextColor(LCD_COLOR_RED);
			LCD_DisplayStringLine(LINE(16), (uint8_t* )"Alert  Mode");
		}
		else if(mode==0){
			LCD_SetTextColor(LCD_COLOR_GREEN);
			LCD_DisplayStringLine(LINE(16), (uint8_t* )"Normal Mode");
		}
		
		if(person==1&mode==1){
		  LED_RED;
			delay();
			printf("Alert\n");
			BEEP_TOGGLE;
			Delay(0xFFFFFF);
		}
		if(person==2 || mode==0){
			LED_GREEN;
			BEEP_OFF;
		  delay();
		}
		//status_check[status_loop]=0;
  }	
}


	void My_Usart1_Printf(char *string){
    while(*string){
        /* ????? USART3 */
        USART_SendData(USART1, (unsigned short int) *string++);
        /* ???????? */
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
}
	
void My_Uart1_Init(){

    /******** ?? USART3 ********/
    USART_Cmd(USART1, ENABLE);  
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    /* ?? USART3 ?? */
    NVIC_EnableIRQ(USART1_IRQn);
}
	
/*
 * ????? USART3 ?????
 */
char buff [100] = "";

 
char * My_Usart1_ReadLine(){    
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){         
        char c = USART_ReceiveData(USART1);         
        if(c != '\r' && c != '\n'){
            sprintf (buff, "%s%c", buff, c);
        }else{
            char buff2 [100] = "";					
            memcpy(buff2,buff, strlen(buff)); // ? buff ??? buff2
            memset(buff, 0, strlen(buff)); // ?? buff ???????            
            return buff2;
        }
    }
    return "";
}

void USART1_IRQHandler(){
	  char * data = My_Usart1_ReadLine();
		if(!strcmp(data, "1")){       
       //My_Usart1_Printf("Red LED on\r\n");
			 person=1;
		} 
    if(!strcmp(data, "2")){       
       //My_Usart1_Printf("Green LED on\r\n");
			 person=2;
		}		
}

int main ( void )
{
  SysTick_Init();   
  Debug_USART_Config();   
  RC522_Init ();     
  LCD_Init();
  LCD_LayerInit();	
	LED_GPIO_Config();
	My_Uart1_Init();
	BEEP_GPIO_Config();
	
  printf ( "WF-RC522 Test \r \n" );

  LTDC_Cmd(ENABLE);
	
	/*set LCD background color*/
  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_Clear(LCD_COLOR_BLACK);
	
  /*initialization LCD OREGROUND_LAYER*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
  
	/*set LCD background transparency*/
  LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BLACK);
  
	/*LCD_SetLayer(LCD_FOREGROUND_LAYER)
	set LCD_SetLayer*/		
  LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  
  LCD_SetTextColor(LCD_COLOR_BLUE);

  LCD_DisplayStringLine(LINE(14),(uint8_t*)"WF-RC522 antenna area" );

  PcdReset ();
  M500PcdConfigISOType ( 'A' );/*set work mode*/

  while ( 1 ){	
    //
		LED_WHITE;
		IC_test ();/*IC card check*/	
		
  }	
}



/*********************************************END OF FILE**********************/

