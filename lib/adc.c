/** 
 * @descrition  A/D Converter
 *
 * Copyright (C) 2018 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       14.01.2018
 * @file        adc.c
 * @tested      AVR Atmega16
 * @inspiration 
 *
 */
#include <avr/io.h>
#include "adc.h"

/***
 * @description Init Switch Interrupts INT0, INT1
 *
 * @param  Void
 * @return Void
 */
void AdcInit(void)
{
  // adc init
  // ADC result to left
  // ADC chanel 1
  ADMUX = (1 << REFS0)|
          (1 << ADLAR)|
          (1 << ADATE)|
          (1 << MUX0);
  // free running mode
  SFIOR &= ~((1 << ADTS2)|
             (1 << ADTS1)|
             (1 << ADTS0));
  // set mandatory values
  ADCSRA = (1 << ADEN) |
           (1 << ADIE) |
           (1 << ADPS2)|
           (1 << ADPS1);
  // start conversion
  ADCSRA |= (1 << ADSC);
}
