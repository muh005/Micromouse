#define USARTx                             UART5 
#define USARTx_IRQn                        UART5_IRQn
#define USARTx_IRQHandler                  UART5_IRQHandler
#define USARTx_CLK                         RCC_APB1Periph_UART5
#define USARTx_CLOCKCMD                    RCC_APB1PeriphClockCmd 
#define USARTx_BAUDRATE                    9600

#define USARTx_RX_GPIO_PORT                GPIOD 
#define USARTx_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOD 
#define USARTx_RX_PIN                      GPIO_Pin_2
#define USARTx_RX_AF                       GPIO_AF_UART5 
#define USARTx_RX_SOURCE                   GPIO_PinSource2   

#define USARTx_TX_GPIO_PORT                GPIOC 
#define USARTx_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOC 
#define USARTx_TX_PIN                      GPIO_Pin_12 
#define USARTx_TX_AF                       GPIO_AF_UART5 
#define USARTx_TX_SOURCE                   GPIO_PinSource12