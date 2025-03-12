//clk = 0.0625 mks
//------------------------------------------------------------------------------
#include "built_in.h"
#include "keybrd.h"
#include "lcd_driver.h"
#include "global.h"
#include "vga_generator.h"
#include "genlow.h"
#include "gen_hi.h"
#include "testopto.h"
//#include "testshort.h"
//-----------------------------------------------------------------------------
extern char s_vga[];
extern char s_gen[];
extern char s_genhi[];
extern char s_testopto[];
extern char s_testshort[];
//-----------------------------------------------------------------------------
//видеорежим
extern short vr;
//0 - 640x480x60 color rows
//1 - 640x480x60 red
//2 - 640x480x60 green
//3 - 640x480x60 blue

//4 - 800x600x60 color rows
//5 - 800x600x60 red
//6 - 800x600x60 green
//7 - 800x600x60 blue


//8 - 1024x768x60 color rows
//-----------------------------------------------------------------------------
void T1Overflow() iv IVT_ADDR_TIMER1_OVF ics ICS_AUTO {
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//Hsync
void T2Overflow() iv IVT_ADDR_TIMER2_OVF ics ICS_AUTO {
}
//-----------------------------------------------------------------------------
// called by interrupt service routine when incoming data arrives
void T2CompB() iv IVT_ADDR_TIMER2_COMPB ics ICS_AUTO {
 if (vr == 0){
//  Delay_us(1);
  PORTD |= red;
  Delay_us(width_640);
  PORTD |= green;
  Delay_us(width_640);
  PORTD |= blue;
  Delay_us(width_640);
  PORTD &= ~white;
  asm nop
  PORTD |= green;
  Delay_us(width_640);
  PORTD |= blue;
  Delay_us(width_640);
  PORTD &= ~white;
  asm nop
  PORTD |= blue;
  Delay_us(width_640);
  PORTD |= red;
  Delay_us(width_640);
  PORTD &= ~white;
  return;
 }
 //red
 else if (vr == 1){
  PORTD = red;
  Delay_us(24);
  PORTD &= ~white;
  return;
 }
 //green
 else if (vr == 2){
  PORTD |= green;
  Delay_us(24);
  PORTD &= ~white;
  return;
 }
 //blue
 else if (vr == 3){
  PORTD |= blue;
  Delay_us(23);
  PORTD &= ~white;
  return;
 }
 
 
 else if (vr == 4){
  Delay_us(1);
  PORTD |= red;
  Delay_us(width_800);
  PORTD |= green;
  Delay_us(width_800);
  PORTD |= blue;
  Delay_us(width_800);
  PORTD &= ~white;
  asm nop  asm nop
  PORTD |= green;
  Delay_us(width_800);
  PORTD |= blue;
  Delay_us(width_800);
  PORTD &= ~white;
  asm nop  asm nop
  PORTD |= blue;
  Delay_us(width_800);
  PORTD |= red;
  Delay_us(width_800);
  PORTD &= ~white;
  return;
 }
 //red
 else if (vr == 5){
  PORTD |= red;
  Delay_us(16);
  PORTD &= ~white;
  return;
 }
 //green
 else if (vr == 6){
  PORTD |= green;
  Delay_us(16);
  PORTD &= ~white;
  return;
 }
 //blue
 else if (vr == 7){
  PORTD |= blue;
  Delay_us(16);
  PORTD &= ~white;
  return;
 }
}
//-----------------------------------------------------------------------------
void main() {
 short sel, inv, key;
 SREG.B7 = 0;
 InitMega();
 Delay_ms(2000);
 Sound_Play(1500, 100);
 sel = 0;
 while(1){
_start:;
  L3310_ClearScreen();
  if(sel == 0){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(0, s_vga, inv);
  if(sel == 1){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(1, s_gen, inv);
  if(sel == 2){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(2, s_genhi, inv);
  if(sel == 3){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(3, s_testopto, inv);
//  if(sel == 4){inv = 1;}else{inv = 0;}
//  L3310_WriteStringAtCenter(4, s_testshort, inv);
  do{
  }while( (key = GetPresedKey()) == key_nothing);
//  key = key_select;
//  sel = 4;
  //select
  if (key == key_select){
   switch(sel){
    //vga генератор
    case 0:
     MenuVGAGenerator();
     goto _start;
    break;
    //генератор НЧ
    case 1:
     MenuGenLow();
     goto _start;
    break;
    //генератор ВЧ
    case 2:
     MenuGenHi();
     goto _start;
    break;
    //тестер оптопар
    case 3:
     MenuTestOpto();
     goto _start;
    break;
    //тестер кабелей на короткое
 //    case 4:
 //    MenuTestShort();
 //    goto _start;
 //   break;
   }
  }
  //up
  else if (key == key_up){
   if (sel >= 1)
    sel --;
  }
  //down
  else if (key == key_down){
   if (sel < (main_menu - 1))
    sel ++;
  }//
  else if (key == key_select_long){
  }
 }
}