#include "built_in.h"
#include "keybrd.h"
#include "lcd_driver.h"
#include "global.h"
//-----------------------------------------------------------------------------
extern char s_freq[];
extern char s_freqlow[];
extern char s_dutylow[];
extern char s_genhi[];
extern char s_05m[];
extern char s_1m[];
extern char s_2m[];
extern char s_4m[];
extern char s_8m[];
//-----------------------------------------------------------------------------
//генерация идет по выовду PD5 (11) таймера0 8-ти битного
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void GenHiRun(short reg, short top){
 unsigned short key;
 char txt[4];
 short duty;
 while(1){
_start:;
  duty = 100 - ((OCR0B * 100) / top);
  L3310_ClearScreen();
  L3310_WriteStringAtCenter(0, s_genhi, 0);
  L3310_WriteStringAtCenter(2, s_freqlow, 0);
  switch(reg){
   case 0:
    L3310_WriteStringAtCenter(3, s_05m, 1);
   break;
   case 1:
    L3310_WriteStringAtCenter(3, s_1m, 1);
   break;
   case 2:
    L3310_WriteStringAtCenter(3, s_2m, 1);
   break;
   case 3:
    L3310_WriteStringAtCenter(3, s_4m, 1);
   break;
   case 4:
    L3310_WriteStringAtCenter(3, s_8m, 1);
   break;
  }
  //скважность
  L3310_WriteStringAtCenter(4, s_dutylow, 0);
  ByteToStr(duty, txt);
  L3310_WriteStringAtCenter(5, txt, 1);
  //
  //
  do{
  }while( (key = GetPresedKey()) == key_nothing);
//  key = key_select;
  //select
  if (key == key_select){
  }
  //up
  else if (key == key_up){
   if (OCR0B > 2)
    OCR0B --;
  }
  //down
  else if (key == key_down){
   if (OCR0B < top)
    OCR0B ++;
  }//
  else if (key == key_select_long){
   TCCR0B &= (0<<CS22)|(0<<CS21)|(0<<CS20);
   return;
  }
  else if (key == key_down_long){
   OCR0B = top / 2;
   goto _start;
  }//
 }
}
//-----------------------------------------------------------------------------
void MenuGenHi(){
 unsigned short sel, inv, key;
 unsigned short res;
 SREG.B7 = 0;
 DDRB = 0b11100111;
 DDRD = 0b11111111;
 DDRC = 0b11111111;
 PORTD = 0;
 sel = 0;

 while(1){
_start:;
  ResetTimers();
  TCCR0A |= (1<<COM0B1)|(1<<COM0B0)|(1<<WGM00)|(1<<WGM01);
  TCCR0B |= (1<<WGM02);
  //
  L3310_ClearScreen();
  L3310_WriteStringAtCenter(0, s_genhi, 0);
  if(sel == 0){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(1, s_05m, inv);
  if(sel == 1){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(2, s_1m, inv);
  if(sel == 2){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(3, s_2m, inv);
  if(sel == 3){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(4, s_4m, inv);
  if(sel == 4){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(5, s_8m, inv);
  //
  do{
  }while( (key = GetPresedKey()) == key_nothing);
//  key = key_select;
  //select
  if (key == key_select){
   switch(sel){
    //0.5 MGz
    case 0:
     OCR0A = 32;
     OCR0B = 16;
     //clk
     TCCR0B |= (0<<CS22)|(0<<CS21)|(1<<CS20);
     GenHiRun(sel, 32);
     goto _start;
    break;
    //1 MGz
    case 1:
     OCR0A = 16;
     OCR0B = 8;
     //clk
     TCCR0B |= (0<<CS22)|(0<<CS21)|(1<<CS20);
     GenHiRun(sel, 16);
     goto _start;
    break;
    //2 MGz
    case 2:
     OCR0A = 8;
     OCR0B = 4;
     //clk
     TCCR0B |= (0<<CS22)|(0<<CS21)|(1<<CS20);
     GenHiRun(sel, 8);
     goto _start;
    break;
    //4 MGz
    case 3:
     OCR0A = 4;
     OCR0B = 2;
     //clk
     TCCR0B |= (0<<CS22)|(0<<CS21)|(1<<CS20);
     GenHiRun(sel, 4);
     goto _start;
    break;
    //8 MGz
    case 4:
     OCR0A = 2;
     OCR0B = 1;
     //clk
     TCCR0B |= (0<<CS22)|(0<<CS21)|(1<<CS20);
     GenHiRun(sel, 2);
     goto _start;
    break;
   }
  }
  //up
  else if (key == key_up){
   if (sel >= 1)
    sel --;
  }
  //down
  else if (key == key_down){
   if (sel < 4)
    sel ++;
  }//
  else if (key == key_select_long){
   TCCR0B &= (0<<CS22)|(0<<CS21)|(0<<CS20);
   return;
  }
  else if (key == key_down_long){
   goto _start;
  }//
 }
}
//-----------------------------------------------------------------------------