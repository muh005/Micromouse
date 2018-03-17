#include "bsp_internalFlash.c"
/*
* @brief main
* @param None
* @retval None
*/
int main(void)
{
	/*initialize usart, mode: 115200*/
	Debug_USART_Config(); 
	LED_GPIO_Config();
	LED_BLUE;
	/*Using printf，after redefining fputc，printf will print to serial port*/ 
	printf("this is a usart printf demo. \r\n");
	 
	printf("Read and Test FLASH\r\n");
	
	if (InternalFlash_Test()==0) { 
		LED_GREEN;
		printf("Test read and write FLASH successful\r\n");
	} 
	else {
		printf("Test read and write FLASH failed\r\n"); 
		LED_RED;
	}
}
