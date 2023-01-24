/*
 * DS1302.h
 *
 *  Created on: 2023. 1. 24.
 *      Author: kkk
 */

#ifndef INC_DS1302_H_
#define INC_DS1302_H_



#endif /* INC_DS1302_H_ */
#include "main.h"

void DS1302_HALT(GPIO_TypeDef *GPIO_SCLK, uint16_t GPIO_Pin_SCLK, GPIO_TypeDef *GPIO_I_O, uint16_t GPIO_Pin_I_O, GPIO_TypeDef *GPIO_CE, uint16_t GPIO_Pin_CE, uint8_t flag);
uint8_t commandRead(GPIO_TypeDef *GPIO_SCLK, uint16_t GPIO_Pin_SCLK, GPIO_TypeDef *GPIO_I_O, uint16_t GPIO_Pin_I_O);
void commandWrite(GPIO_TypeDef *GPIO_SCLK, uint16_t GPIO_Pin_SCLK, GPIO_TypeDef *GPIO_I_O, uint16_t GPIO_Pin_I_O,uint8_t value);
void readTime(uint8_t *time);
uint8_t decToformat(uint8_t c);
void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
