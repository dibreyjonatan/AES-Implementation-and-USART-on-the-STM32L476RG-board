#include "stm32l476xx.h"

int main(void){

    /* Enable GPIOA clock and read back to ensure it's active */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    (void)RCC->AHB2ENR;

    /* Configure PA5 as general purpose output (01) */
    GPIOA->MODER &= ~(1<<10 | 1<<11);
    // or 
    //GPIOA->MODER &= ~(1<<10 );
    //GPIOA->MODER &=~(1<<11);
    GPIOA->MODER |=  (1U << 10);

   
    while(1){
        GPIOA->BSRR = (1U << 5);               // set PA5
        for(volatile uint32_t i = 0; i < 200000; ++i) __NOP();  

        GPIOA->BSRR = (1U << (5 + 16));       // reset PA5
        for(volatile uint32_t i = 0; i < 200000; ++i) __NOP();  
}

}