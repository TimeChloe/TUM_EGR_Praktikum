/*
 * egr_adc.c
 *
 *  Created on: 17.05.2024
 *      Author: Yue Zhang
 * Last Change: 17.05.2024
 */

#include "egr_adc.h"
#include "egr_gpio.h"
#include "Delay_ms.h"

void ADC_Init(void)
{
    // 1.activate ADC module
    activate_adc();
    // 选择ADC的时钟源（内部时钟）   这个要在STM32G4 Series advanced Arm®-based 32-bit MCUs - Reference manual的文件中才能找到  P328
    select_adc_clock();
    
    // 激活GPIO模块并配置引脚为模拟模式
    activate_gpio_b();
    gpio_mode(GPIOB_BASE, Potentiometer_Pin, GPIO_ANALOG);

    // 配置ADC
    ADC_configuration();

    // ADC calibration
    ADC_Calibration();
}

// 1.activate ADC module
void activate_adc(void)
{
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + 0x4C); //0x40021000 is the base address of the RCC module, 0x4C is the offset for the AHB2ENR register
    *adresse |= (1 << 13); //RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN;
}

// 2. 选择ADC的时钟源（内部时钟）   这个要在STM32G4 Series advanced Arm®-based 32-bit MCUs - Reference manual的文件中才能找到  P328
//  RCC->CCIPR &= ~RCC_CCIPR_ADC12SEL; // 清除ADC12SEL位（默认为00）  //这里可以看到有两种时钟？系统时钟和PLL “P”时钟，都是内部时钟吗？（还没整明白）
//Bit 29:28 ADC12SEL[1:0]: ADC12 clock source selection 01: PLL “P” clock (PLLCLK) selected as ADC clock    10: System clock selected as ADC clock
void select_adc_clock(void)
{
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + 0x88); //0x40021000 is the base address of the RCC module, 0x88 is the offset for the CCIPR register
    //RCC->CCIPR &= ~RCC_CCIPR_ADC12SEL;
    *adresse |= (1 << 29); //
    *adresse &= ~(1 << 28); 
}



// ADC configuration
void ADC_configuration(void)     // P687
{
    // deep power down mode deactivation
    uint32_t volatile *adresse = (uint32_t *)(ADC1_BASE + ADC_CR); //0x08 is the offset for the ADC control register
    *adresse &= ~(1 << 29); //ADC1->CFGR &= ~(1 << 29); //禁用ADC的深度睡眠模式

    // ADC voltage regulator enable
    adresse = (uint32_t *)(ADC1_BASE + ADC_CR); //0x08 is the offset for the ADC control register
    *adresse |= (1 << 28); 
    delay_ms(10); // wait for the voltage regulator to stabilize

    // ADC disable
    adresse = (uint32_t *)(ADC1_BASE + ADC_CR); //0x08 is the offset for the ADC control register
    *adresse &= ~(1 << 0); //ADC1->CR &= ~(1 << 0); //禁用ADC

    // set length of sequence
    adresse = (uint32_t *)(ADC1_BASE + 0x30); //0x30 is the offset for the ADC regular sequence register
    *adresse &= ~(1 << 0); //0000: 1 conversion
    *adresse &= ~(1 << 1);
    *adresse &= ~(1 << 2);
    *adresse &= ~(1 << 3);

    // Assuming only one channel (e.g., channel 15) is used for single conversion.
    *adresse &= ~(0xF << 6); // Clear the bits first
    *adresse |= (15 << 6);   // Set the desired channel number (e.g., 15)
    //PB0 ADC1_IN15  15：01111

    // ADC calibration  //Bit 30 ADCALDIF: Differential mode for calibration
    adresse = (uint32_t *)(ADC1_BASE + ADC_CR); //0x08 is the offset for the ADC control register
    *adresse &= ~(1 << 30); 

    // ADC single conversion mode
    adresse = (uint32_t *)(ADC1_BASE + ADC_CFGR); 
    *adresse &= ~(1 << 13); 
}

void ADC_Calibration(void)
{
    // Ensure ADC peripheral is disabled
    uint32_t volatile *adresse = (uint32_t *)(ADC1_BASE + ADC_CR); //0x50040000 is the base address of the ADC module, 0x08 is the offset for the ADC control register
    *adresse &= ~(1 << 0); //ADC1->CR &= ~(1 << 0); //禁用ADC

    // Start ADC calibration
    adresse = (uint32_t *)(ADC1_BASE + ADC_CR); //0x08 is the offset for the ADC control register
    *adresse |= (1 << 31); //ADC1->CR |= (1 << 31); //启动ADC校准

    // Wait for calibration to finish
    adresse = (uint32_t *)(ADC1_BASE + ADC_CR); //0x08 is the offset for the ADC control register

    //while (((ADC1->CR >> 31) & 1) != 0); //等待校准完成
    while (((*adresse >> 31) & 1) != 0); //while (((ADC1->CR >> 31) & 1) != 0)

    
    // Clear ADRDY bit
    //ADC1->ISR |= ADC_ISR_ADRDY;
    adresse = (uint32_t *)(ADC1_BASE + ADC_ISR); 
    *adresse |= (1 << 0);

    // Wait for at least 4 clock cycles (1ms delay for safety)
    delay_ms(1);
}



// Start ADC
void ADC_start(void)
{
    // Set ADEN bit to enable ADC
    //ADC1->CR |= ADC_CR_ADEN;
    uint32_t volatile *adresse = (uint32_t *)(ADC1_BASE + ADC_CR); //0x08 is the offset for the ADC control register
    *adresse |= (1 << 0);
    
    // Wait for ADRDY bit to be set by hardware
    //while (!(ADC1->ISR & ADC_ISR_ADRDY));     ADRDY: ADC ready
    adresse = (uint32_t *)(ADC1_BASE + ADC_ISR); // ADC interrupt and status register
    while (!(*adresse & (1 << 0)));

    // // ADC1->CR |= ADC_CR_ADSTART;
    // adresse = (uint32_t *)(ADC1_BASE + ADC_CR); 
    // *adresse |= (1 << 2);
}





//Singal mode for one channel
uint32_t read_adc_value_single_mode(void)
{
    uint32_t volatile *adresse;

    // ADC1->CR |= ADC_CR_ADSTART;
    adresse = (uint32_t *)(ADC1_BASE + ADC_CR); 
    *adresse |= (1 << 2);

    // Wait for EOC bit to be set by hardware
    adresse = (uint32_t *)(ADC1_BASE + ADC_ISR); // ADC interrupt and status register
    while (!(*adresse & (1 << 2)));

    // Read conversion result
    adresse = (uint32_t *)(ADC1_BASE + ADC_DR); // ADC regular data register
    uint32_t result = *adresse;

    // Clear EOS bit
    adresse = (uint32_t *)(ADC1_BASE + ADC_ISR); // ADC interrupt and status register
    *adresse |= (1 << 3);
    
    return result;
}





//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++