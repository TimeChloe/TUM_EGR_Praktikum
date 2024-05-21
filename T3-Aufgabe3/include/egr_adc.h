/*
 * ehr_adc.h
 *
 *  Created on: 17.05.2024
 *      Author: Yue Zhang
 * Last Change: 17.05.2024
 */

#ifndef EGR_ADC_H
#define EGR_ADC_H

#include <stdint.h>


//ADC Base Addresses
#define ADC1_BASE 0x50000000
#define ADC2_BASE 0x50000100

//ADC Registers
#define ADC_ISR 0x00
#define ADC_IER 0x04
#define ADC_CR 0x08
#define ADC_CFGR 0x0C
#define ADC_CFGR2 0x10
#define ADC_SMPR1 0x14
#define ADC_SMPR2 0x18
#define ADC_TR1 0x20
#define ADC_TR2 0x24
#define ADC_TR3 0x28
#define ADC_SQR1 0x30
#define ADC_SQR2 0x34
#define ADC_SQR3 0x38
#define ADC_SQR4 0x3C
#define ADC_DR 0x40
#define ADC_JSQR 0x4C
#define ADC_OFR1 0x60
#define ADC_OFR2 0x64
#define ADC_OFR3 0x68
#define ADC_OFR4 0x6C
#define ADC_JDR1 0x80
#define ADC_JDR2 0x84
#define ADC_JDR3 0x88
#define ADC_JDR4 0x8C
#define ADC_AWD2CR 0xA0
#define ADC_AWD3CR 0xA4
#define ADC_DIFSEL 0xB0
#define ADC_CALFACT 0xB4
#define ADC_CCR 0x308

#define Potentiometer_Pin 0

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ADC FUNCTIONS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ADC_Init(void);

void activate_adc(void);
void select_adc_clock(void);
void ADC_configuration(void);
void ADC_Calibration(void);
void ADC_start(void);

uint32_t read_adc_value_single_mode(void);






#endif /* EGR_ADC_H */
// Path: src/egr_adc.c
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++