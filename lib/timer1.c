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
const unsigned short int PRESCALERS[N_OF_PRES-3] = {
  PRES_0001, 
  PRES_0008, 
  PRES_0064, 
  PRES_0256, 
  PRES_1024
};
/** @const Top values */
const char *MODES[NO_MODES] = {
  "CTC", 
  "F-PWM", 
  "P-PWM", 
  "PF-PWM"
};
/** @const Prescalers */
const char *TOPS[NO_TOPS] = {
  "255", 
  "512", 
  "1024", 
  "OCR1A", 
  "OCR1B", 
  "ICR1"
};

// top value
const char *_str_top;
// mode of operation
const char *_str_mode;

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
  
  if ((mode == MODE_04) || (mode == MODE_14)) {            // MODE 04 -> CTC, OCR1A
    // returned[0] - prescaler
    // returned[1] - value
    returned = calc_freq(req_freq, mode);
    // set prescaler
    TIMER1_PRES(PRESCALERS[*(returned)]); 
    // set OCR1A vaue
    TC1_OCR1A = *(returned+1);
    // assign value
    _str_top = TOPS[3];
    // ctc
    if (mode == MODE_04) {
      // clear time on compare match
      _str_mode = MODES[0];
    } else {
      // fast PWM
      _str_mode = MODES[1];
    }
    // free memory
    free(returned);
    // success
    return 1;
  } 
  // MODE 12, 15 -> ICR1 for CTC, FPWM
  else if ((mode == MODE_12) || (mode == MODE_15)) {
    // returned[0] - prescaler
    // returned[1] - value
    returned = calc_freq(req_freq, mode);
    // set prescaler
    TIMER1_PRES(PRESCALERS[*(returned)]); 
    // set ICR1 value
    TC1_ICR1 = *(returned+1);
    // assign value
    _str_top = TOPS[5];
    // ctc
    if (mode == MODE_12) {
      // clear time on compare match
      _str_mode = MODES[0];
    } else {
      // fast PWM
      _str_mode = MODES[1];
    }
    // free memory
    free(returned);
    // success
    return 1;
  }
  // MODE 08, 09, 10, 11 -> OCR1A, ICR1 for P&F PWM, P PWM
  else if ((mode == MODE_08) || (mode == MODE_09) ||    
           (mode == MODE_10) || (mode == MODE_11)) {
    // returned[0] - prescaler
    // returned[1] - value
    returned = calc_freq(req_freq, mode);
    // set prescaler
    TIMER1_PRES(PRESCALERS[*(returned)]);
    // OCR1A top value
    if ((mode == MODE_09) || (mode == MODE_11)) {
      // set OCR1A vaue
      TC1_OCR1A = *(returned+1);
      // assign value
      _str_top = TOPS[3];
    } else {
      // set ICR1 value
      TC1_ICR1 = *(returned+1);
      // assign value
      _str_top = TOPS[5];
    }
    // Phase & Freq Correct PWM
    if ((mode == MODE_08) || (mode == MODE_09)) {
      // Phase & Freq Correct PWM
      _str_mode = MODES[3];
    } else {
      // Phase Correct PWM
      _str_mode = MODES[2];
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
  // Phase & Frequency Correct PWM and Phase Correct PWM - OCR1A, ICR1
  // ---------------------------------------------
  else if ((mode == MODE_08) || (mode == MODE_09) ||
           (mode == MODE_10) || (mode == MODE_11)) {
    // loop
    while (--i) {
      // prescaler
      *(value) = i;
      // calculate potential value
      calc = (F_CPU/(2*prescalers[i]*req_freq));
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
