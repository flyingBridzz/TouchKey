#include "TPAD.h"

volatile uint8_t startIC_State = 0;
volatile uint8_t finishIC_State = 0;
volatile uint32_t getValue = 0;

void TPAD_Reset(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	GPIO_Initure.Pin=GPIO_PIN_5;            //PA5
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
	GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
   
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);	//PA5���0���ŵ�
	HAL_Delay(5);
	__HAL_TIM_CLEAR_FLAG(&htim2,TIM_FLAG_CC1|TIM_FLAG_UPDATE);   //�����־λ
	__HAL_TIM_SET_COUNTER(&htim2,0); //������ֵ��0
 
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //���츴��
	GPIO_Initure.Pull=GPIO_NOPULL;          //����������
	GPIO_Initure.Alternate=GPIO_AF1_TIM2;   //PA5����ΪTIM2ͨ��1
	HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
}

uint32_t TPAD_Get_Val(void)
{
	if(startIC_State == 0){
		TPAD_Reset();
		startIC_State = 1;
		return 0;
	}else{
		if(finishIC_State == 1){
			finishIC_State = 0;
			startIC_State = 0;
			return getValue;
		}
		return 0;
	}
}

uint32_t TPAD_Get_MaxVal(uint8_t n)
{
	static uint8_t tmpCnt = 0;
	static uint32_t maxVal = 0;
	uint32_t tmpVal = 0;
	
	if(tmpCnt>0){
		tmpVal = TPAD_Get_Val();
		if(tmpVal != 0){
			tmpCnt--;
			if(tmpVal > maxVal){
				maxVal = tmpVal;
			}
			if(tmpCnt == 0){
				return maxVal;
			}
		}
	}else{
		tmpCnt = n;
		maxVal = 0;
	}
	return 0;
}

uint32_t TouchKeyScan(uint8_t Mode)
{
	static uint8_t keyen=0;	//0,���Կ�ʼ���;>0,�����ܿ�ʼ���	 
	uint8_t res=0;
	uint8_t sample=3;	//Ĭ�ϲ�������Ϊ3��	 
	uint32_t rval;
	if(Mode)
	{
		sample=6;	//֧��������ʱ�����ò�������Ϊ6��
		keyen=0;	//֧������	  
	}
	rval=TPAD_Get_MaxVal(sample);
	if(rval != 0){
		if(rval>300&&rval<3400){						 
			if(keyen==0)res=1;	//keyen==0,��Ч 	     	    					   
			keyen=3;				//����Ҫ�ٹ�3��֮����ܰ�����Ч   
		} 
		if(keyen)keyen--;	
	}	
	return res;
}



