#include "built_in.h"
#include "keybrd.h"
#include "lcd_driver.h"
#include "global.h"
//-----------------------------------------------------------------------------
extern char s_tk_1_125[];
extern char s_tk_1_230[];
extern char s_testshort[];
extern char s_vga_gen[];
//-----------------------------------------------------------------------------
//таймера настроены по образу генераторов - описалово там
//-----------------------------------------------------------------------------
void MenuTestShortStart(char OCR_A){
 //Hi freq
 OCR0A = OCR_A;
 OCR0B = (OCR_A>>1);
 //clk hi
 TCCR0B |= (0<<CS22)|(0<<CS21)|(1<<CS20);
 //Lo freq
 //ICNC1 ICES1 Ц WGM13 WGM12 CS12 CS11 CS10
 TCCR1B = 0b00011000;
 //COM1A1 COM1A0 COM1B1 COM1B0 Ц Ц WGM11 WGM10
 TCCR1A = 0b10000010;
 //set top value to ICR1
 ICR1H = 0x3E;     //
 ICR1L = 0x7F;    //
 //
 OCR1AH = 0x1F;
 OCR1AL = 0x40;
 //clk/1 low
 TCCR1B |= (1<<CS10);
 L3310_WriteStringAtCenter(5, s_vga_gen, 0);
}
//-----------------------------------------------------------------------------
void MenuTestShort(){
 unsigned short sel, inv, key;
 unsigned short res;
 SREG.B7 = 0;
 DDRB = 0b11100111;
 DDRD = 0b11111111;
 DDRC = 0b11111111;
 PORTD.B5 = 0;
 PORTB.B1 = 0;
 sel = 0;
 while(1){
_start:;
  ResetTimers();
  TCCR0A |= (1<<COM0B1)|(1<<COM0B0)|(1<<WGM00)|(1<<WGM01);
  TCCR0B |= (1<<WGM02);
  //
  L3310_ClearScreen();
  L3310_WriteStringAtCenter(0, s_testshort, 0);
  if(sel == 0){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(1, s_tk_1_125, inv);
  if(sel == 1){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(2, s_tk_1_230, inv);
  //
_working:;
  do{
  }while( (key = GetPresedKey()) == key_nothing);
//  key = key_select;
  //select
  if (key == key_select){
   switch(sel){
    //  1/125
    case 0:
     MenuTestShortStart(128);
     goto _working;
    break;
    // 1/230
    case 1:
     MenuTestShortStart(70);
     goto _working;
    break;
   }
  }
  //up
  else if (key == key_up){
   if (sel >= 1){
    sel --;
    goto _start;
   }
  }
  //down
  else if (key == key_down){
   if (sel < 1){
    sel ++;
    goto _start;
   }
  }//
  else if (key == key_select_long){
   ResetTimers();
   return;
  }
  else if (key == key_down_long){
   goto _start;
  }//
 }
}
//-----------------------------------------------------------------------------