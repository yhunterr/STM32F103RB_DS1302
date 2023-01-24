/*
 * DS1302.c
 *
 *  Created on: 2023. 1. 24.
 *      Author: kkk
 */


#include "DS1302.h"
#include "main.h"

uint8_t decToformat(uint8_t c)
{
  return (c/10)<<4 | (c%10);
}



void commandWrite(GPIO_TypeDef *GPIO_SCLK, uint16_t GPIO_Pin_SCLK, GPIO_TypeDef *GPIO_I_O, uint16_t GPIO_Pin_I_O,uint8_t value)
{
  /* I_O PORT OUTPUT SETTING */

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_Pin_I_O;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIO_I_O, &GPIO_InitStruct);
  int i;

  /* WRITE SETTING */
  for(i=0;i<8;i++) //LSB
  {
     if( ((value>>i) & 0x01) ? 1 : 0 )
       HAL_GPIO_WritePin(GPIO_I_O, GPIO_Pin_I_O, 1);
     else
       HAL_GPIO_WritePin(GPIO_I_O, GPIO_Pin_I_O, 0);
     HAL_GPIO_WritePin(GPIO_SCLK, GPIO_Pin_SCLK, 1);
     HAL_GPIO_WritePin(GPIO_SCLK, GPIO_Pin_SCLK, 0);
  }
}

uint8_t commandRead(GPIO_TypeDef *GPIO_SCLK, uint16_t GPIO_Pin_SCLK, GPIO_TypeDef *GPIO_I_O, uint16_t GPIO_Pin_I_O)
{
  /* I_O PORT INPUT SETTING */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  HAL_GPIO_WritePin(GPIO_I_O, GPIO_Pin_I_O, 0);
  GPIO_InitStruct.Pin = GPIO_Pin_I_O;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIO_I_O, &GPIO_InitStruct);
  uint8_t value=0;
  uint8_t currentBit = 0;
  int i;

  /* INPUT SETTING */
  for(i=0;i<8;i++)
  {
    currentBit = HAL_GPIO_ReadPin(GPIO_I_O, GPIO_Pin_I_O);
    value |= (currentBit<<i);
    HAL_GPIO_WritePin(GPIO_SCLK, GPIO_Pin_SCLK, 1);
    HAL_GPIO_WritePin(GPIO_SCLK, GPIO_Pin_SCLK, 0);
  }
  return value;
}

void DS1302_HALT(GPIO_TypeDef *GPIO_SCLK, uint16_t GPIO_Pin_SCLK, GPIO_TypeDef *GPIO_I_O, uint16_t GPIO_Pin_I_O, GPIO_TypeDef *GPIO_CE, uint16_t GPIO_Pin_CE, uint8_t flag)
{
  HAL_GPIO_WritePin(GPIO_CE, GPIO_Pin_CE, 1);
  commandWrite(GPIO_SCLK, GPIO_Pin_SCLK, GPIO_I_O, GPIO_Pin_I_O, 0x80);
  commandWrite(GPIO_SCLK, GPIO_Pin_SCLK, GPIO_I_O, GPIO_Pin_I_O, 0x00 | flag<<7);
  HAL_GPIO_WritePin(GPIO_CE, GPIO_Pin_CE, 0);
}

void readTime(uint8_t *time)
{
  //array : 0: seconds, 1: minutes, 2: hours

  //seconds read
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x81);
  time[0] = commandRead(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin);
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);

  // minutes read
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x83);
  time[1] = commandRead(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin);
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);

  // hours read
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x85);
  time[2] = commandRead(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin);
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);
}



void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
  //second
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x80);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, decToformat(seconds));
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);
  //minutes
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x82);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, decToformat(minutes));
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);
  //hours
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x84);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, decToformat(hours));
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);
}














