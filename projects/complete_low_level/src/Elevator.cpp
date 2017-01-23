/**
 * Gestion de l'ascenseur
 *
 * Pins du moteur:
 * Sens: PE9|PE11 en OUT
 * Marche: PB15 (TIM12CH2)
 *
 *
 * Pins de sens du moteur (bornier gauche):
 * PE9(INPUT1) PE11(INPUT2) en mode OUTPUT
 *
 * SENS: en vue du dessus: PE9/PE11
 * Trigonométrique:1/0
 * Antitrigonomètrique:0/1
 *
 */

#include <delay.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx.h>
#include "Elevator.h"

Elevator::Elevator(void) {
	sens = UP;
}

void Elevator::initTimer(void)  //Initialise le timer
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	
	TIM_TimeBaseInitTypeDef timTimeBaseInitTypeDef;
	TIM_TimeBaseStructInit(&timTimeBaseInitTypeDef);
	
	uint16_t prescaler = (uint16_t)((SystemCoreClock / 2) / 256000) - 1; //CF Motor.cpp
	
	//if(moving){ //Pour utiliser le même timer que les roues, en attendant de lier une autre pin au moteur de l'ascenseur
		timTimeBaseInitTypeDef.TIM_Period=1000;
		timTimeBaseInitTypeDef.TIM_ClockDivision=TIM_CKD_DIV1;
	//}
	/*else{
		timTimeBaseInitTypeDef.TIM_Period=10;
		timTimeBaseInitTypeDef.TIM_ClockDivision=0;
	}*/
	timTimeBaseInitTypeDef.TIM_Prescaler=prescaler;
	timTimeBaseInitTypeDef.TIM_CounterMode=TIM_CounterMode_Up;
	//Configuration du TIMER 3
	
	TIM_TimeBaseInit(TIM12, &timTimeBaseInitTypeDef);
}

void Elevator::initPWM() //Initialise le PWM
{
	TIM_OCInitTypeDef timOcInitTypeDef;
	
	timOcInitTypeDef.TIM_OCMode=TIM_OCMode_PWM1;
	timOcInitTypeDef.TIM_OutputState=TIM_OutputState_Enable;
	timOcInitTypeDef.TIM_OCPolarity=TIM_OCPolarity_High;
	timOcInitTypeDef.TIM_Pulse=0; //PWM initial nul
	
	TIM_OC3Init(TIM12, &timOcInitTypeDef); //Canal 2N de TIM1
	TIM_OC3PreloadConfig(TIM12, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM12, ENABLE);
	
	TIM_Cmd(TIM12, ENABLE); //Active le TIM
}

void Elevator::initPins(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef gpioPinInitStruct;
	GPIO_StructInit(&gpioPinInitStruct);
	
	gpioPinInitStruct.GPIO_Pin=GPIO_Pin_9 + GPIO_Pin_11;  //Pins de sens
	gpioPinInitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_Init(GPIOE, &gpioPinInitStruct);
	
	gpioPinInitStruct.GPIO_Pin=GPIO_Pin_0;                  //Pin de pwm
	gpioPinInitStruct.GPIO_Mode=GPIO_Mode_AF;
	
	gpioPinInitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	gpioPinInitStruct.GPIO_OType=GPIO_OType_PP;
	gpioPinInitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &gpioPinInitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM12);
	
	//Initialise le moteur dans le sens montant
	
	GPIO_SetBits(GPIOE, GPIO_Pin_11 + GPIO_Pin_9);
	GPIO_ResetBits(GPIOE, GPIO_Pin_11);
}

void Elevator::setSens(Sens sensToSet) { //Change la direction dans le sens souhaité(UP ou DOWN)
	if (sensToSet==UP){
		sens=UP;
		GPIO_SetBits(GPIOE, GPIO_Pin_11);
		GPIO_ResetBits(GPIOE, GPIO_Pin_9);
	}
	else if (sensToSet==DOWN){
		sens=DOWN;
		GPIO_SetBits(GPIOE, GPIO_Pin_9);
		GPIO_ResetBits(GPIOE, GPIO_Pin_11);
	}
}

void Elevator::stop(void){
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	//TIM12->CCR2=0;
	if(sens==UP){
		sens=DOWN;
	}
	else if(sens==DOWN){
		sens=UP;
	}
}

void Elevator::run() {//Tourne dans le sens de sens(a déterminer empiriquement)
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	//TIM12->CCR2=200;
}

void Elevator::initialize(void){
	//initTimer();
	initPins();
	//initPWM();
}