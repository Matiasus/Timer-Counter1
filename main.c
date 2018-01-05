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

/**
 * @description Main function
 *
 * @param   void
 * @return  int
 */
int main(void)
{
  char str[20];

  unsigned short int mode = MODE_12;

  TIMER1_MODE(mode);

  // get values for desired frequency
  if (1 == req_frequency(100, mode)) {
    // init lcd
    Pcd8544Init();
    // vymazenie obrazovky
    ClearScreen();
    // pozicia textu riadok, stlpec
    SetTextPosition(0, 0);
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
    SetTextPosition(2, 0);
    // int to string
    itoa(TC1_OCR1A, str, 10);
    // vypis retazca
    DrawString((const char*) _str_top);
    // vypis retazca
    DrawString(":  ");
    // vypis retazca
    DrawString(str);
    // pozicia textu riadok, stlpec
    SetTextPosition(3, 0);
    // vypis retazca
    DrawString("MODE:   ");
    // vypis retazca
    DrawString(_str_mode);
    // vypis - update pamate
    UpdateScreen();
    // free memory
    free(_str_top);
    // free memory
    free(_str_mode);
  }
  // return value
  return 0;
}
