/**
 * Name: Dr. Livingston
 * Course: CPE2610
 * File: gpio.h
 * Dependencies: None
 * Description: Header file for GPIO Function access
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

//RCC Base Addresses
#define RCC_AHB1_ENR 0x40023830
#define RCC_AHB2_ENR 0x40023834
#define RCC_APB1_ENR 0x40023840
#define RCC_APB2_ENR 0x40023844

//GPIO Base Addresses
#define GPIOA_BASE 0x40020000
#define GPIOB_BASE 0x40020400
#define GPIOC_BASE 0x40020800
#define GPIOD_BASE 0x40020C00
#define GPIOE_BASE 0x40021000

//External Interrupt Base Addresses
#define EXTI_BASE 0x40013C00
#define SYSCFG_BASE 0x40013800

//SPI Base Addresses
#define SPI1_BASE 0x40013000
#define SPI2_BASE 0x40003800
#define SPI3_BASE 0x40003C00
#define SPI4_BASE 0x40013400
#define SPI5_BASE 0x40015000

//GPIO Enable defines for the RCC
#define RCC_GPIOA_EN 0
#define RCC_GPIOB_EN 1
#define RCC_GPIOC_EN 2
#define RCC_GPIOD_EN 3

typedef struct GPIO_t {
	uint32_t moder; //0x00
	uint16_t otyper;
	uint16_t rsvd_0;  //Avoid using (has no effect)
	uint32_t ospeedr;
	uint32_t pupdr; //0x0c
	uint16_t idr; //0x10
	uint16_t rsvd_1; //Avoid using (has no effect)
	uint16_t odr; //0x14
	uint16_t rsvd_2; //Avoid using (has no effect)
	uint32_t bsrr;
	uint32_t lckr;
	uint32_t afrl;
	uint32_t afrh;
} GPIO_t;

typedef struct EXTI_t {
	uint32_t imr;
	uint32_t emr;
	uint32_t rtsr;
	uint32_t ftsr;
	uint32_t swier;
	uint32_t pr;
} EXTI_t;

typedef struct SYSCFG_t {
	uint32_t memrmp;
	uint32_t pmc;
	uint32_t exticr1;
	uint32_t exticr2;
	uint32_t exticr3;
	uint32_t exticr4;
	uint32_t cmpcr;
} SYSCFG_t;

typedef struct SPI_t {
	uint32_t CR1;
	uint32_t CR2;
	uint32_t SR;
	uint32_t DR;
	uint32_t CRCPR;
	uint32_t RXCRCR;
	uint32_t TXCRCR;
	uint32_t I2SCFGR;
	uint32_t I2SPR;
} SPI_t;

#endif /* GPIO_H_ */
