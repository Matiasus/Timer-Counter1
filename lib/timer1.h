
/** 
 * @desription  16 bits Timer Counter generator
 * ---------------------------------------------------
 * Copyright (C) 2017 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       22.12.2017
 * @update      05.01.2018
 * @file        timer1.h
 * @tested      
 * @inspiration 
 */
#include <avr/io.h>
// definition of library for frequency and PWM
// (fast, phase correct, and phase and frequency correct)
#ifndef _TIMER1_H_
  #define _TIMER1_H_

  // define clock for Atmega 8
  #if defined(__AVR_ATmega8__)
    #define F_CPU 8000000
  // define clock for atmega 16
  #elif defined(__AVR_ATmega16__)
    #define F_CPU 16000000
    #define TC1_OC_DDR    DDRD
    #define TC1_OC1B      PD4
    #define TC1_OC1A      PD5
  #endif

  // define registers for 16 bits timer/counter 1
  #if defined(__AVR_ATmega8__) || defined(__AVR_ATmega16__)
    #define TC1_ICR1    ICR1
    #define TC1_OCR1A   OCR1A
    #define TC1_OCR1B   OCR1B
    #define TC1_TCCR1A  TCCR1A
    #define TC1_TCCR1B  TCCR1B
  #endif  

  // mode definition
  // -----------------------------------------------
  #define MODE_00 0x00  // normal
  #define MODE_01 0x01  // PWM, Phase Correct,  8bits (0x00FF)
  #define MODE_02 0x02  // PWM, Phase Correct,  9bits (0x01FF)
  #define MODE_03 0x03  // PWM, Phase Correct, 10bits (0x03FF)
  #define MODE_04 0x08  // CTC, OCR1A
  #define MODE_05 0x09  // Fast PWM,  8bits (0x00FF)
  #define MODE_06 0x0A  // Fast PWM,  9bits (0x01FF)
  #define MODE_07 0x0B  // Fast PWM, 10bits (0x01FF)
  #define MODE_08 0x10  // PWM, Phase & Frequency Correct,  ICR1
  #define MODE_09 0x11  // PWM, Phase & Frequency Correct,  OCR1A
  #define MODE_10 0x12  // PWM, Phase Correct, ICR1
  #define MODE_11 0x13  // PWM, Phase Correct, OCR1A
  #define MODE_12 0x18  // CTC, ICR1
  #define MODE_13 0x19  // Reserved
  #define MODE_14 0x1A  // Fast PWM, ICR1
  #define MODE_15 0x1B  // Fast PWM, OCR1A
  // mode select
  #ifndef TIMER1_MODE
    #define TIMER1_MODE(MODE) {TC1_TCCR1A &= 0xFC; TC1_TCCR1A |= (0x03 & MODE); TC1_TCCR1B &= 0xE7; TC1_TCCR1B |= (0x18 & MODE);}
  #endif
  
  // prescaler definition
  #define N_OF_PRES 8
  // possible values
  #define PRES_0000 0x00  // no pclock source
  #define PRES_0001 0x01  // clk/1
  #define PRES_0008 0x02  // clk/8
  #define PRES_0064 0x03  // clk/64
  #define PRES_0256 0x04  // clk/256
  #define PRES_1024 0x05  // clk/1024
  #define PRES_00T1 0x06  // external clock source on T1 pin, falling edge
  #define PRES_00T2 0x07  // external clock source on T1 pin, rising edge
  // prescaler select
  #ifndef TIMER1_PRES
    #define TIMER1_PRES(PRES) {TC1_TCCR1B &= 0xFB; TC1_TCCR1B |= PRES;}
  #endif

  // output definition
  // -----------------------------------------------
  #define OUTPUT_000 0x00  // normal port operatiomn, OC1A, OC1B disconnected
  #define OUTPUT_A01 0x40  // toggle OC1A
  #define OUTPUT_B01 0x10  // toggle OC1B
  #define OUTPUT_AB1 0x50  // toggle OC1A, OC1B
  #define OUTPUT_A02 0x80  // clear on compare match OC1A
  #define OUTPUT_B02 0x20  // clear on compare match OC1B
  #define OUTPUT_AB2 0xA0  // clear on compare match OC1A, OC1B
  #define OUTPUT_A03 0xC0  // set on compare match OC1A
  #define OUTPUT_B03 0x30  // set on compare match OC1B
  #define OUTPUT_AB3 0xFF  // set on compare match OC1A, OC1B
  
  // define max value for OCR1A, ICR1
  #define MAX_16 65535
  // number of string modes
  #define NO_MODES 4
  // numer of string tops
  #define NO_TOPS 6

  /** @const Prescalers */
  extern const unsigned short int PRESCALERS[];
  /** @const Modes */
  extern const char *MODES[NO_MODES];
  /** @const Top values */
  extern const char *TOPS[NO_TOPS];

  /** @var selected top value */ 
  extern const char *_str_top;
  /** @var selected mode of operation */ 
  extern const char *_str_mode;

  /**
   * @description Set output pins
   *
   * @param   unsigned short int
   * @return  void
   */  
  void set_output(unsigned short int);

  /**
   * @description Required frequency
   *
   * @param   unsigned long int
   * @param   unsigned short int
   * @return  unsigned short int
   */  
  unsigned short int req_frequency(unsigned long int, unsigned short int);

  /**
   * @description Calculate required frequency
   *
   * @param   unsigned long int
   * @param   unsigned short int
   * @return  unsigned int*
   */  
  unsigned int * calc_freq(unsigned long int, unsigned short int);

#endif
