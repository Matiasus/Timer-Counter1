
/** 
 * @desription  16 bits Timer Counter generator
 * ---------------------------------------------------
 * Copyright (C) 2017 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       22.12.2017
 * @file        timer.c
 * @tested      
 * @inspiration 
 */
// define 
#ifndef _TIMER1_H_
  #define _TIMER1_H_

// define clock foe Atmega 8
  #if defined(__AVR_ATMEGA8__)
    #define _FCPU 8000000
  // define clock for atmega 16
  #elif defined(__AVR_ATMEGA16__)
    #define _FCPU 16000000
  #endif

#endif
