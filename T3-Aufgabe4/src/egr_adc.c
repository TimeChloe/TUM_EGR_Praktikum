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


void ADC_Init(int length, int mode)
{
    // 1.activate ADC module
    activate_adc();
    // 选择ADC的时钟源（内部时钟）   这个要在STM32G4 Series advanced Arm®-based 32-bit MCUs - Reference manual的文件中才能找到  P328
    select_adc_clock();
    
    // 激活GPIO模块并配置引脚为模拟模式
    activate_gpio_a();
    gpio_mode(GPIOA_BASE,Brightness1_Pin , GPIO_ANALOG);
    gpio_mode(GPIOA_BASE,Brightness2_Pin , GPIO_ANALOG);
    gpio_mode(GPIOA_BASE,Brightness3_Pin , GPIO_ANALOG);

    // 配置ADC
    ADC_configuration(length, mode);

    // ADC calibration
    ADC_Calibration();
}





// 1.activate ADC module
void activate_adc(void)
{
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + 0x4C); //0x40021000 is the base address of the RCC module, 0x4C is the offset for the AHB2ENR register
    *adresse |= (1 << 13); //RCC->AHB2ENR |= RCC_AHB2ENR_ADC22EN;
}


// 2. 选择ADC的时钟源（内部时钟）   这个要在STM32G4 Series advanced Arm®-based 32-bit MCUs - Reference manual的文件中才能找到  P328
//  RCC->CCIPR &= ~RCC_CCIPR_ADC22SEL; // 清除ADC22SEL位（默认为00）  //这里可以看到有两种时钟？系统时钟和PLL “P”时钟，都是内部时钟吗？（还没整明白）
//Bit 29:28 ADC22SEL[1:0]: ADC22 clock source selection 01: PLL “P” clock (PLLCLK) selected as ADC clock    10: System clock selected as ADC clock
void select_adc_clock(void)
{
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + 0x88); //0x40021000 is the base address of the RCC module, 0x88 is the offset for the CCIPR register
    //RCC->CCIPR &= ~RCC_CCIPR_ADC22SEL;
    *adresse |= (1 << 29); //
    *adresse &= ~(1 << 28); 
}



// ADC configuration
void ADC_configuration(int length, int mode)     // P687
{
    // deep power down mode deactivation
    uint32_t volatile *adresse = (uint32_t *)(ADC2_BASE + ADC_CR); //0x08 is the offset for the ADC control register
    *adresse &= ~(1 << 29); //ADC2->CFGR &= ~(1 << 29); //禁用ADC的深度睡眠模式

    // ADC voltage regulator enable
    adresse = (uint32_t *)(ADC2_BASE + ADC_CR); //0x08 is the offset for the ADC control register
    *adresse |= (1 << 28); 
    delay_ms(10); // wait for the voltage regulator to stabilize

    // ADC disable
    adresse = (uint32_t *)(ADC2_BASE + ADC_CR); //0x08 is the offset for the ADC control register
    *adresse &= ~(1 << 0); //ADC2->CR &= ~(1 << 0); //禁用ADC

    // set length of sequence
    // set_sequence_length(length);
    adresse = (uint32_t *)(ADC2_BASE + 0x30); //0x30 is the offset for the ADC regular sequence register
    // Clear the bits first
    *adresse &= ~(0xF << 0);
    // Set the length
    *adresse |= ((length) << 0);


    // Assuming 3 channel (e.g., channel 1, channel 2, channel 4) are used
    // Set the channel number for each conversion
    adresse = (uint32_t *)(ADC2_BASE + 0x30);
    *adresse &= ~(0xF << 6); // Clear the bits 
    *adresse |= (1 << 6);   // Set the desired channel number 
    *adresse &= ~(0xF << 12); // Clear the bits 
    *adresse |= (2 << 12);   // Set the desired channel number
    *adresse &= ~(0xF << 18); // Clear the bits 
    *adresse |= (4 << 18);   // Set the desired channel number
    //PA0   ADC2_IN1; PA1   ADC2_IN2; PA7   ADC2_IN4

    // ADC calibration  //Bit 30 ADCALDIF: Differential mode for calibration
    adresse = (uint32_t *)(ADC2_BASE + ADC_CR); //0x08 is the offset for the ADC control register
    *adresse &= ~(1 << 30); 


    if (mode == MODE_SINGLE)
    {
        // ADC single conversion mode
        adresse = (uint32_t *)(ADC2_BASE + ADC_CFGR); 
        *adresse &= ~(1 << 13); 
    }
    else if (mode == MODE_CONTINUOUS)
    {
        // ADC continuous conversion mode
        adresse = (uint32_t *)(ADC2_BASE + ADC_CFGR); 
        *adresse |= (1 << 13); 
    }
}

// void set_sequence_length(uint32_t length) {
//     uint32_t *adresse = (uint32_t *)(ADC2_BASE + 0x30); //0x30 is the offset for the ADC regular sequence register

//     // Clear the bits first
//     *adresse &= ~(0xF << 0);

//     // Set the length
//     *adresse |= ((length) << 0);
// }

void ADC_Calibration(void)
{
    // Ensure ADC peripheral is disabled
    uint32_t volatile *adresse = (uint32_t *)(ADC2_BASE + ADC_CR); //0x50040000 is the base address of the ADC module, 0x08 is the offset for the ADC control register
    *adresse &= ~(1 << 0); //ADC2->CR &= ~(1 << 0); //禁用ADC

    // Start ADC calibration
    adresse = (uint32_t *)(ADC2_BASE + ADC_CR); //0x08 is the offset for the ADC control register
    *adresse |= (1 << 31); //ADC2->CR |= (1 << 31); //启动ADC校准

    // Wait for calibration to finish
    adresse = (uint32_t *)(ADC2_BASE + ADC_CR); //0x08 is the offset for the ADC control register

    //while (((ADC2->CR >> 31) & 1) != 0); //等待校准完成
    while (((*adresse >> 31) & 1) != 0); //while (((ADC2->CR >> 31) & 1) != 0)

    
    // Clear ADRDY bit
    //ADC2->ISR |= ADC_ISR_ADRDY;
    adresse = (uint32_t *)(ADC2_BASE + ADC_ISR); 
    *adresse |= (1 << 0);

    // Wait for at least 4 clock cycles (1ms delay for safety)
    delay_ms(1);
}



// Start ADC

void ADC_start(int mode)
{
    // Set ADEN bit to enable ADC
    //ADC2->CR |= ADC_CR_ADEN;
    uint32_t volatile *adresse = (uint32_t *)(ADC2_BASE + ADC_CR); //0x08 is the offset for the ADC control register
    *adresse |= (1 << 0);
    
    // Wait for ADRDY bit to be set by hardware
    //while (!(ADC2->ISR & ADC_ISR_ADRDY));     ADRDY: ADC ready
    adresse = (uint32_t *)(ADC2_BASE + ADC_ISR); // ADC interrupt and status register
    while (!(*adresse & (1 << 0)));

    // // ADC2->CR |= ADC_CR_ADSTART;
    // adresse = (uint32_t *)(ADC2_BASE + ADC_CR); 
    // *adresse |= (1 << 2);

    if (mode == MODE_CONTINUOUS)
    {
        // Set ADSTART bit to start conversion
        //ADC2->CR |= ADC_CR_ADSTART;
        adresse = (uint32_t *)(ADC2_BASE + ADC_CR); 
        *adresse |= (1 << 2);
    }
}

// Read ADC value in single mode
uint32_t *read_adc_value_single_mode(uint32_t *results,int length)
{
    uint32_t volatile *adresse;

    // ADC2->CR |= ADC_CR_ADSTART;
    adresse = (uint32_t *)(ADC2_BASE + ADC_CR); 
    *adresse |= (1 << 2);


    for (int i = 0; i < length; i++)
    {   
        // Wait for EOC bit to be set by hardware
        adresse = (uint32_t *)(ADC2_BASE + ADC_ISR); // ADC interrupt and status register
        while (!(*adresse & (1 << 2)));

        // Read conversion result
        adresse = (uint32_t *)(ADC2_BASE + ADC_DR); // ADC regular data register
        results[i] = *adresse;

    }
    // Clear EOS bit
    adresse = (uint32_t *)(ADC2_BASE + ADC_ISR); // ADC interrupt and status register
    *adresse |= (1 << 3);

    return results;
}

// 





//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++