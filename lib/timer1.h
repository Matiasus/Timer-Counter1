
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

#endif
