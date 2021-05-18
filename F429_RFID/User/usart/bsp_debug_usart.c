/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  ******************************************************************************
  */ 
  
#include "./usart/bsp_debug_usart.h"


 /**
  * @brief  �t�m�O�M�V�q���_���NVIC
  * @param  �L
  * @retval �L
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* �O�M�V�q���_����տ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* �t�mUSART�����_�� */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �m�_�u�����Ǭ�1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �l�u�����Ǭ�1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* �ϯत�_ */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��l�ưt�mNVIC */
  NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  DEBUG_USART GPIO �t�m,�u�@�Ҧ��t�m�C115200 8-N-1 �A���_�����Ҧ�
  * @param  �L
  * @retval �L
  */
void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK,ENABLE);

  /* �ϯ� USART ���� */
  RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
  
  /* GPIO��l�� */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* �t�mTx�޸}���ƥΥ\��  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN  ;  
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* �t�mRx�޸}���ƥΥ\�� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* �s�� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE,DEBUG_USART_RX_AF);

  /*  �s�� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);
  
  /* �t�m��DEBUG_USART �Ҧ� */
  /* ��C�ǿ�t�v�]�m�GDEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
  /* �r��(��Ʀ줸+����줸)�G8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* �����G1�Ӱ���� */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* ������ܡG���ϥή��� */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* �w��y����G���ϥεw��y */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USART�Ҧ�����G�P�ɨϯ౵���M�o�e */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* ����USART��l�ưt�m */
  USART_Init(DEBUG_USART, &USART_InitStructure); 
	
  /* �O�M�V�q���_���NVIC�t�m */
	NVIC_Configuration();
  
	/* �ϯ��C�𱵦����_ */
	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);
	
  /* �ϯ��C�� */
  USART_Cmd(DEBUG_USART, ENABLE);
}

/*****************  �o�e�@�Ӧr�� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* �o�e�@�Ӧ줸�ո�ƨ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* ���ݵo�e��ƼȦs������ */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �o�e�r�� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* ���ݵo�e���� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  �o�e�@��16��� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ���X���K�� */
	temp_h = (ch&0XFF00)>>8;
	/* ���X�C�K�� */
	temp_l = ch&0XFF;
	
	/* �o�e���K�� */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* �o�e�C�K�� */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///���w�Vc�w���printf���C��A���w�V��i�ϥ�printf���
int fputc(int ch, FILE *f)
{
		/* �o�e�@�Ӧ줸�ո�ƨ��C�� */
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
		/* ���ݵo�e���� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///���w�Vc�w���scanf���C��A���g�V��i�ϥ�scanf�Bgetchar�����
int fgetc(FILE *f)
{
		/* ���ݦ�C���J��� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART);
}
/*********************************************END OF FILE**********************/
