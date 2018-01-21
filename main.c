/** 
 * @desription  16 bits Timer Counter generator
 * ---------------------------------------------------
 * Copyright (C) 2017 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       22.12.2017
 * @update      05.01.2018
 * @file        main.c
 * @tested      
 * @inspiration 
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lib/timer1.h"
#include "lib/adc.h"
#include "lib/int01.h"
#include "lib/pcd8544.h"
#include <util/delay.h>

volatile unsigned short int _buffer[3];
volatile unsigned short int _index = 0;
volatile unsigned short int _push_int0 = 0;
volatile unsigned short int _push_int1 = 0;

/**
 * @description INT0 interrupt
 *
 * @param  INT0_vect
 * @return Void
 */
ISR(INT0_vect) 
{
  // string
  if (_push_int0 < 1) {
    // set push
    _push_int0 = 1;
  } else {
    // set push
    _push_int0 = 0;
  }
}

/**
 * @description INT1 interrupt
 *
 * @param  INT1_vect
 * @return Void
 */
ISR(INT1_vect) 
{
  // string
  if (_push_int1 < 1) {
    // set push
    _push_int1 = 1;
  }
}

/**
 * @description ADC end converting
 *
 * @param  ADC_vect
 * @return Void
 */
ISR(ADC_vect) 
{
  // max 3 values
  if (_index < 3) {
    // store values
    _buffer[_index++] = ADCH;
  }
  // start conversion
  ADCSRA |= (1 << ADSC);
}

/**
 * @description Main function
 *
 * @param   void
 * @return  int
 */
int main(void)
{
  char str[20];
  //char *realmode;
  unsigned long int real = 0;
  unsigned short int mode = 0;
  unsigned short int temp_mode = mode;
  unsigned long int frequency = 1000;
  unsigned long int temp_frequency = frequency;

  // init lcd
  Pcd8544Init();

  // interupt init
  Int01Init();

  // adc init
  AdcInit();

  // enable interrupt
  sei();

  /// Clear screen
  ClearScreen();

  // update creen
  UpdateScreen();

  while (1) {
    // if 3 values store
    if (_index >= 3) {
      // check if three values the same
      if ((_buffer[0] == _buffer[1]) && 
          (_buffer[0] == _buffer[2])) 
      {
        /// Clear screen
        ClearScreen();
        // text position
        SetTextPosition(0, 0);
        // string
        DrawString("PULSEGENERATOR", NORMAL);

        // text position
        SetTextPosition(4, 0);
        // string
        DrawString("R[Hz]: ", NORMAL);
        // int to str
        itoa(real, str, 10);
        // string
        DrawString(str, NORMAL);

        // if pushed button
        if (_push_int0 == 0) {
          // mode
          mode = (_buffer[0] >> 4);
          // store mode
          temp_mode = mode;
          // text position
          SetTextPosition(2, 0);
          // string
          DrawString("MODES", INVERT);
          // string
          DrawString(": ", NORMAL);
          // mode
          itoa(mode, str, 10);
          // string
          DrawString(str, NORMAL);
          // text position
          SetTextPosition(2, 10);
          // string
          DrawString(_str_mode, NORMAL);

          // text position
          SetTextPosition(3, 0);
          // string
          DrawString("F[Hz]: ", NORMAL);
          // int to str
          itoa(temp_frequency, str, 10);
          // string
          DrawString(str, NORMAL);

          // if pushed button
          if (_push_int1 == 1) {
            // MODE 0 - Normal
            // --------------------------------------------------
            // switch mode
             if (mode == 0) {
              // set selected mode
              TIMER1_MODE(MODE_00);
              // set output pin
              set_output(OUTPUT_000);
              // real frequency
              real = req_frequency(frequency, MODE_00);
            }
            // --------------------------------------------------

            // MODE 4 - CTC
            // --------------------------------------------------
            // switch mode
            else if (mode == 4) {
              // set selected mode
              TIMER1_MODE(MODE_04);
              // set output pin
              set_output(OUTPUT_A01);
              // real frequency
              real = req_frequency(frequency, MODE_04);
            }
            // --------------------------------------------------

            // MODE 12 - CTC
            // --------------------------------------------------
            // switch mode
            else if (mode == 12) {
              // set selected mode
              TIMER1_MODE(MODE_12);
              // set output pin
              set_output(OUTPUT_A01);
              // real frequency
              real = req_frequency(frequency, MODE_12);
            }
            // --------------------------------------------------
            // reset push button
            _push_int1 = 0;
          }
        }
        // FREQUENCY SETTINGS
        // --------------------------------------------------        
        else if (_push_int0 == 1) {
          // frequency
          frequency = _buffer[0] << 2;
          // store frequency
          temp_frequency = frequency;          
          // text position
          SetTextPosition(2, 0);
          // string
          DrawString("MODES", NORMAL);
          // string
          DrawString(": ", NORMAL);
          // mode
          itoa(temp_mode, str, 10);
          // string
          DrawString(str, NORMAL);
          // text position
          SetTextPosition(2, 10);
          // string
          DrawString(_str_mode, NORMAL);

          // text position
          SetTextPosition(3, 0);
          // string
          DrawString("F[Hz]", INVERT);
          // string
          DrawString(": ", NORMAL);
          // int to str
          itoa(frequency, str, 10);
          // string
          DrawString(str, NORMAL);
        }

        // update creen
        UpdateScreen();
        // delay
        _delay_ms(1000);
      }
    // null
    _index = 0;
   }
  }

  // return value
  return 0;
}
