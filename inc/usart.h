#pragma once 

#include <stdio.h>
#include "stm32l476xx.h"

void USART3_Init(void);
void USART3_SendChar(char c);
void USART3_SendString(const char *s);
