
/** 
 * @desription  16 bits Timer Counter generator
 * ---------------------------------------------------
 * Copyright (C) 2017 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       22.12.2017
 * @file        timer1.h
 * @tested      
 * @inspiration 
 */
// definitiion of library for frequency and PWM
// (fast, phase correct, and phase and frequency correct)
#ifndef _TIMER1_H_
  #define _TIMER1_H_

// define clock foe Atmega 8
  #if defined(__AVR_ATMEGA8__)
    #define _FCPU 8000000
  // define clock for atmega 16
  #elif defined(__AVR_ATMEGA16__)
    #define _FCPU 16000000
  #endif
  // define registers for 16 bits timer/counter 1
  #if defined(__AVR_ATMEGA8__) || defined(__AVR_ATMEGA16__)
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
  #define MODE_12 0x18  // CTC, ICR1A
  #define MODE_13 0x19  // Reserved
  #define MODE_14 0x1A  // Fast PWM, ICR1
  #define MODE_15 0x1B  // Fast PWM, OCR1A
  // mode select
  #define TIMER1_MODE(MODE) {TC1_TCCR1A &= 0xFC; TC1_TCCR1A |= (0x03 & MODE); TC1_TCCR1B &= 0xE7; TC1_TCCR1B |= (0x18 & MODE);}

#endif