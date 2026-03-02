#include "usart.h"



void USART3_Init(void){
  // 1. Activer horloges GPIOB et USART3
    RCC->AHB2ENR  |= RCC_AHB2ENR_GPIOBEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART3EN;

    // 2. Configurer PB10 (TX) et PB11 (RX) en mode alternatif AF7
    GPIOB->MODER &= ~((3U << (10 * 2)) | (3U << (11 * 2))); // Clear bits
    GPIOB->MODER |=  ((2U << (10 * 2)) | (2U << (11 * 2))); // Mode AF

    GPIOB->AFR[1] &= ~((0xF << ((10 - 8) * 4)) | (0xF << ((11 - 8) * 4)));
    GPIOB->AFR[1] |=  ((7U << ((10 - 8) * 4)) | (7U << ((11 - 8) * 4))); // AF7 = USART3

    // 3. Configurer USART3
    USART3->CR1 &= ~USART_CR1_UE;       // Désactiver USART
    USART3->BRR = 4000000 / 115200;      // Baud rate (si HSI = 4 MHz)
    USART3->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;       // Activer la transmission seulement
   
 	NVIC_SetPriority(USART3_IRQn, 2);  
    NVIC_EnableIRQ(USART3_IRQn);
	
	USART3->CR1 |= USART_CR1_UE;        // Activer USART3

}
void USART3_SendChar(char c){
 while (!(USART3->ISR & USART_ISR_TXE)); // Attente TXE=1
    USART3->TDR = c;
}
void USART3_SendString(const char *s){
  while (*s) USART3_SendChar(*s++);
}