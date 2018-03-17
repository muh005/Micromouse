#include "usart.h"

void USART_Config(void) 
{  
  GPIO_InitTypeDef GPIO_InitStructure;  
  USART_InitTypeDef USART_InitStructure;  
  RCC_AHB1PeriphClockCmd(USARTx_RX_GPIO_CLK|USARTx_TX_GPIO_CLK,ENABLE); 
  USARTx_CLOCKCMD(USARTx_CLK, ENABLE); 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz ; //50
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
  GPIO_InitStructure.GPIO_Pin =  USARTx_TX_PIN  ; 
  GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure); 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
  GPIO_InitStructure.GPIO_Pin =  USARTx_RX_PIN; 
  GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure); 
  GPIO_PinAFConfig(USARTx_RX_GPIO_PORT,USARTx_RX_SOURCE,USARTx_RX_AF); 
  GPIO_PinAFConfig(USARTx_TX_GPIO_PORT,USARTx_TX_SOURCE,USARTx_TX_AF);
  USART_InitStructure.USART_BaudRate = USARTx_BAUDRATE; 
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  USART_Init(USARTx, &USART_InitStructure);
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);  
  USART_Cmd(USARTx, ENABLE); 
}

int fputc(int ch, FILE *f)  
{ 
    USART_SendData(USARTx, (uint8_t) ch);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) ;  
    return (ch); 
}