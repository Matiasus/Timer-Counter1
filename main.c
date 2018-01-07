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
#include "lib/timer1.h"
#include "lib/pcd8544.h"
#include <util/delay.h>

/**
 * @description Main function
 *
 * @param   void
 * @return  int
 */
int main(void)
{
/*
  DDRD |= (1 << PD4);
  DDRD |= (1 << PD5);

  OCR1A = 250;

  ICR1 = 500;

  TCCR1A = 0xA2;  // A
  TCCR1B = 0x19;  // fpwm, icr1, 256
*/

  char str[20];

  // select mode
  unsigned short int mode = MODE_15;
  // set selected mode
  TIMER1_MODE(mode);
  // set output pin
  set_output(OUTPUT_A01);

  // get values for desired frequency
  if (1 == req_frequency(31900, mode)) {
    // init lcd
    Pcd8544Init();
    // vymazenie obrazovky
    ClearScreen();

    // pozicia textu riadok, stlpec
    SetTextPosition(0, 0);
    // int to string
    itoa(TC1_TCCR1A, str, 10);
    // vypis retazca
    DrawString("TCCR1A: ");
    // vypis retazca
    DrawString(str);

    // pozicia textu riadok, stlpec
    SetTextPosition(1, 0);
    // int to string
    itoa(TC1_TCCR1B, str, 10);
    // vypis retazca
    DrawString("TCCR1B: ");
    // vypis retazca
    DrawString(str);

    // pozicia textu riadok, stlpec
    SetTextPosition(3, 0);
    // int to string
    itoa(TC1_TCCR1A, str, 10);
    // vypis retazca
    DrawString("TCCR1A: ");
    // vypis retazca
    DrawString(str);

    // pozicia textu riadok, stlpec
    SetTextPosition(2, 0);
    // vypis retazca
    DrawString("MODE: ");
    // vypis retazca
    DrawString(_str_mode);

    // pozicia textu riadok, stlpec
    SetTextPosition(3, 0);
    // check which one is zero
    if (TC1_ICR1 == 0) {
      // int to string
      itoa(TC1_OCR1A, str, 10);
      // vypis retazca
      DrawString(_str_top);
    } else {
      // int to string
      itoa(TC1_ICR1, str, 10);
      // vypis retazca
      DrawString(_str_top);
    }
    // vypis retazca
    DrawString(": ");
    // vypis retazca
    DrawString(str);

    // vypis - update pamate
    UpdateScreen();
  }


  // return value
  return 0;
}
