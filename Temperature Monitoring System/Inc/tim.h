/**
 * Name: Jesse Propst
 * Course: CPE2610
 * File: tim.h
 * Dependencies: None
 * Description: Header file for timer function
 */


#ifndef TIM_H_
#define TIM_H_

#include <stdint.h>

#define TIM2_BASE 0x40000000
#define TIM3_BASE 0x40000400
#define TIM4_BASE 0x40000800
#define TIM5_BASE 0x40000C00

#define RCC_APB1_ENR 0x40023840

typedef struct TIMx_t {
	uint32_t CR1;
	uint32_t CR2;
	uint32_t SMCR;
	uint32_t DIER;
	uint32_t SR;
	uint32_t EGR;
	uint32_t CCMR1; //Different register layouts for different modes
	uint32_t CCMR2; //Different register layouts for different modes
	uint32_t CCER;
	uint32_t CNT;
	uint32_t PSC;
	uint32_t ARR;
	uint32_t RESERVED0; //Reserved, do not use
	uint32_t CCR1;
	uint32_t CCR2;
	uint32_t CCR3;
	uint32_t CCR4;
	uint32_t RESERVED1; //Reserved, do not use
	uint32_t DCR;
	uint32_t DMAR;
	uint32_t OR; //Only for tim2 and tim5
} TIMx_t;

#endif /* TIM_H_ */
