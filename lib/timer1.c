/** 
 * @desription  16 bits Timer Counter generator
 * ---------------------------------------------------
 * Copyright (C) 2017 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       22.12.2017
 * @update      05.01.2018
 * @file        timer1.c
 * @tested      
 * @inspiration 
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "timer1.h"
#include <util/delay.h>
#include "pcd8544.h"

/** @const Prescalers */
const unsigned short int PRESCALERS[N_OF_PRES-3] = {PRES_0001, PRES_0008, PRES_0064, PRES_0256, PRES_1024};

// top value
char *_str_top;
// mode of operation
char *_str_mode;

/**
 * @description Required frequency
 *
 * @param   unsigned long int
 * @param   unsigned short int
 * @return  unsigned short int
 */
unsigned short int req_frequency(unsigned long int req_freq, unsigned short int mode)
{
  // returned values
  unsigned int *returned = calloc(2, sizeof(unsigned int));
  
  if (mode == MODE_04 || MODE_14) {            // MODE 04 -> CTC, OCR1A
    // returned[0] - prescaler
    // returned[1] - value
    returned = calc_freq(req_freq, mode);
    // set prescaler
    TIMER1_PRES(PRESCALERS[*(returned)]); 
    // set OCR1A vaue
    TC1_OCR1A = *(returned+1);
    // alloc memory
    _str_top = calloc(5, sizeof(char));
    // assign value
    _str_top = "OCR1A";
    // alloc memory
    _str_mode = calloc(5, sizeof(char));
    // ctc
    if (mode == MODE_04) {
      // clear time on compare match
      _str_mode = "CTC";
    } else {
      // fast PWM
      _str_mode = "FPWM";
    }
    // free memory
    free(returned);
    // success
    return 1;
  } 
  else if (mode == MODE_12 || MODE_15) {       // MODE 12 -> CTC, ICR1
    // returned[0] - prescaler
    // returned[1] - value
    returned = calc_freq(req_freq, mode);
    // set prescaler
    TIMER1_PRES(PRESCALERS[*(returned)]); 
    // set OCR1A vaue
    TC1_ICR1 = *(returned+1);
    // alloc memory
    _str_top = calloc(4, sizeof(char));
    // assign value
    _str_top = "ICR1";
    // alloc memory
    _str_mode = calloc(5, sizeof(char));
    // ctc
    if (mode == MODE_12) {
      // clear time on compare match
      _str_mode = "CTC";
    } else {
      // fast PWM
      _str_mode = "FPWM";
    }
    // free memory
    free(returned);
    // success
    return 1;
  }

  // unsuccess
  return 0;
}

/**
 * @description Calculate frequency for CTC
 *
 * @param   unsigned int
 * @param   unsigned short int
 * @return  unsigned int
 */
unsigned int *calc_freq(unsigned long int req_freq, unsigned short int mode)
{
  unsigned long int calc;
  // return values
  unsigned int *value = calloc (2, sizeof (unsigned int));
  // index, 1 more because decrement before evaluation in condition
  unsigned short int i = N_OF_PRES-2;
  // prescalers
  int prescalers[N_OF_PRES-3] = {1, 8, 64, 256, 1024};

  // CTC modes - OCR1A, ICR1
  // ---------------------------------------------
  if ((mode == MODE_04) || (mode == MODE_12)) {
    // loop
    while (--i) {
      // prescaler
      *(value) = i;
      // calculate potential value
      calc = (F_CPU/(2*prescalers[i]*req_freq)) - 1;
      // check if value is in the range
      if ((calc > 0) && (calc < MAX_16)) {
        // get value
        *(value+1) = (unsigned int) calc;
        // success, return finded value
        return value;
      }
    }
  }
  // Fast PWM modes - OCR1A, ICR1
  // ---------------------------------------------
  else if ((mode == MODE_14) || (mode == MODE_15)) {
    // loop
    while (--i) {
      // prescaler
      *(value) = i;
      // calculate potential value
      calc = (F_CPU/(prescalers[i]*req_freq)) - 1;
      // check if value is in the range
      if ((calc > 0) && (calc < MAX_16)) {
        // get value
        *(value+1) = (unsigned int) calc;
        // success, return finded value
        return value;
      }
    }
  }
  // if no possible value exists
  return value;
}
