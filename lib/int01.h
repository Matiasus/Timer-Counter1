/** 
 * Interrupts
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       14.01.2018
 * @file        int01.h
 * @tested      AVR Atmega16
 * @inspiration 
 *
 */
#include <avr/io.h>

/** @definition */
#ifndef __INT01_H__
  #define __INT01_H__

  // define registers for 16 bits timer/counter 1
  #if defined(__AVR_ATmega16__)
    #define INT_DDR   DDRD
    #define INT_PORT  PORTD
    #define INT_PIN0  PD2
    #define INT_PIN1  PD3
    #define INT_INT0  INT0
    #define INT_INT1  INT1
    #define INT_ISC00 ISC00
    #define INT_ISC01 ISC01
    #define INT_ISC10 ISC10
    #define INT_ISC11 ISC11
    #define INT_GICR  GICR
    #define INT_MCUCR MCUCR
  #endif  

  /***
   * @description Init Switch Interrupts INT0, INT1
   *
   * @param  Void
   * @return Void
   */
  void Int01Init(void);

#endif
