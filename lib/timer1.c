/** 
 * @desription  16 bits Timer Counter generator
 * ---------------------------------------------------
 * Copyright (C) 2017 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       22.12.2017
 * @file        timer1.c
 * @tested      
 * @inspiration 
 */
#include <stdio.h>
#include <avr/io.h>
#include "timer1.h"

/**
 * @description Required frequency
 *
 * @param   long int - required frequency
 * @return  int - value of OCR1, ICR1
 */
unsigned int req_frequency(unsigned long int req_freq)
{
  // CTC mode, OCR1A
  if ((TC1_TCCR1A & 0x08) != 0) {
    // calculate OCR1A value
    TC1_OCR1A = calc_freq_ctc(req_freq);
  // CTC mode, ICR1
  } else if ((TC1_TCCR1A & 0x18) != 0){
    // calculate OCR1A value
    TC1_ICR1 = calc_freq_ctc(req_freq);
  }
  // return
  return 0;
}

/**
 * @description Calculate frequency for CTC
 *
 * @param   unsigned int
 * @return  unsigned long int
 */
unsigned int calc_freq_ctc(unsigned long int req_freq)
{
  // value
  unsigned int value;
  // index, 1 more because decrement before evaluation in condition
  unsigned short int i = N_OF_PRES-2;
  // prescalers
  unsigned char prescalers[N_OF_PRES-3] = {PRES_0001, PRES_0008, PRES_0064, PRES_0256, PRES_1024};
  // loop
  while (--i) {
    // calculate potential value
    value = F_CPU/(2*prescalers[i]*req_freq);
    // check if value is in the range
    if ((value > 0) && (value < MAX_16)) {
      // success, return finded value
      return value;
    }
  }
  // if no possible value exists
  return 0;
}
