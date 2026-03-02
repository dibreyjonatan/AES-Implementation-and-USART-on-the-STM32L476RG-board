#pragma once 

#include <stdio.h>
#include "stm32l476xx.h"
#include <string.h>
#include <stdint.h>

void USART3_Init(void);
void USART3_SendChar(char c);
void USART3_SendString(const char *s);
void USART3_SendHex(const uint8_t *buf, uint16_t len); 
