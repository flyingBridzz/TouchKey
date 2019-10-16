#ifndef _TPAD_H_
#define _TPAD_H_

#include "main.h"

extern TIM_HandleTypeDef htim2;
extern volatile uint8_t startIC_State;
extern volatile uint8_t finishIC_State;
extern volatile uint32_t getValue;


uint32_t TPAD_Get_MaxVal(uint8_t n);
uint32_t TouchKeyScan(uint8_t Mode);

#endif

