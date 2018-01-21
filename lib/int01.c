/** 
 * Interrupts
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       14.01.2018
 * @file        int01.c
 * @tested      AVR Atmega16
 * @inspiration 
 *
 */
#include "int01.h"
/***
 * @description Init Switch Interrupts INT0, INT1
 *
 * @param  Void
 * @return Void
 */
void Int01Init(void)
{
  // PD2 PD3 as input
  INT_DDR &= ~((1 << INT_PIN1) | (1 << INT_PIN0));
  // pull up activated
  INT_PORT |= (1 << INT_PIN1) | (1 << INT_PIN0);
  // INT1 - rising edge
  INT_MCUCR |= (1 << INT_ISC11) | (1 << INT_ISC10) | (1 << INT_ISC01) | (1 << INT_ISC00);
  // enable interrupts INT1
  INT_GICR |= (1 << INT_INT1) | (1 << INT_INT0);
}
