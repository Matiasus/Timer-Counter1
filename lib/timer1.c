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
 * @description Set output pins
 *
 * @param   unsigned short int
 * @return  void
 */  
void set_output(unsigned short int out_mode) 
{
  // output pin A
  unsigned short int out_pin_a = (out_mode & 0x30);
  // output pin B
  unsigned short int out_pin_b = (out_mode & 0xC0);
  // check if only OC1B pin is required as output
  // PIN OC1B
  // ---------------------------
  if (out_pin_a && !out_pin_b) {
    // set direction as output for OC1B
    TC1_OC_DDR |= (1 << TC1_OC1B);
  }
  // PIN OC1A
  // ---------------------------
  else if (!out_pin_a && out_pin_b) {
    // set direction as output for OC1A
    TC1_OC_DDR |= (1 << TC1_OC1A);
  }
  // PIN OC1A, OC1B
  // ---------------------------
  else {
    // set direction as output for OC1A
    TC1_OC_DDR |= (1 << TC1_OC1A);
    // set direction as output for OC1B
    TC1_OC_DDR |= (1 << TC1_OC1B);
  } 
  // clear COM1A0:1, COM1B0:1
  TC1_TCCR1A &= 0x0F;
  // set required bits
  TC1_TCCR1A |= out_mode;
}

/**
 * @description Required frequency
 *
 * @param   unsigned long int
 * @param   unsigned short int
 * @return  unsigned short int
 */
unsigned long int req_frequency(unsigned long int req_freq, unsigned short int mode)
{
  unsigned long int real;
  // returned values
  unsigned int *returned = calloc(2, sizeof(unsigned int));
  // prescalers - shifts to right
  int presc[N_OF_PRES-3] = {1, 8, 64, 256, 1024};

  // check if mode is in possible range
  if ((mode < MODE_00) && (mode > MODE_15)) {
    // unsuccess
    return 0;
  }
  // returned[0] - prescaler, returned[1] - value
  returned = calc_freq(req_freq, mode);
/*
  // if no possible value found
  if (*(returned) == 0) {
    // set prescaler
    TIMER1_PRES(PRES_0000);
    // unsuccess
    return 0;
  }
*/
  // set prescaler
  TIMER1_PRES(PRESCALERS[*(returned)]);

  // TOP VALUE - OCR1A, ICR1
  // ------------------------------------------------  
  // OCR1A
  // ------------------------
  if ((mode == MODE_04) || (mode == MODE_14) ||
      (mode == MODE_09) || (mode == MODE_11)) {
    // set OCR1A vaue
    TC1_OCR1A = *(returned+1);
    // assign value
    _str_top = TOPS[3];
  } 
  // ICR1
  // ------------------------
  else if ((mode == MODE_12) || (mode == MODE_15) ||
           (mode == MODE_08) || (mode == MODE_10)) {
    // set ICR1 value
    TC1_ICR1 = *(returned+1);
    // assign value
    _str_top = TOPS[5];
  }

  // MODE OF OPERATION
  // -------------------------------------------------
  // Clear timer on compare match
  // ------------------------
  if ((mode == MODE_04) || (mode == MODE_12)) {
    // clear time on compare match
    _str_mode = MODES[0];
    // real frequency
    real = F_CPU/(2.00 * presc[*(returned)] * (*(returned+1)+1));
    // return real
    return real;
  }
  // Fast PWM
  // ------------------------
  else if ((mode == MODE_14) || (mode == MODE_15)) {
    // fast PWM
    _str_mode = MODES[1];
    // real frequency
    real = F_CPU/(1.00 * presc[*(returned)] * (*(returned+1)+1));
    // return real
    return real;
  }
  // Phase and freq correct PWM
  // ------------------------
  else if ((mode == MODE_08) || (mode == MODE_09)) {
    // phase and freq correct PWM
    _str_mode = MODES[3];
    // real frequency
    real = F_CPU/(2.00 * presc[*(returned)] * (*(returned+1)));
    // return real
    return real;
  }
  // Phase correct PWM
  // ------------------------
  else if ((mode == MODE_10) || (mode == MODE_11)) {
    // phase correct PWM
    _str_mode = MODES[2];
    // real frequency
    real = F_CPU/(2.00 * presc[*(returned)] * (*(returned+1)));
    // return real
    return real;
  }
  // free memory
  free(returned);
  // success
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
  long int calc;
  // return values
  unsigned int *value = calloc (2, sizeof (unsigned int));
  // index, 1 more because decrement before evaluation in condition
  unsigned short int i = 5;
  // prescalers - shifts to right
  int presc[N_OF_PRES-3] = {0, 3, 6, 8, 10};

  // CTC modes - OCR1A, ICR1
  // ---------------------------------------------
  if ((mode == MODE_04) || (mode == MODE_12)) {
    // loop
    while ((--i) >= 0) {
      // prescaler
      *(value) = i;
      // calculate potential value
      calc = (((F_CPU >> presc[i]) >> 1)/req_freq)-1;
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
      calc = ((F_CPU >> presc[i])/req_freq)-1;
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
      calc = (((F_CPU >> presc[i]) >> 1)/req_freq);
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
