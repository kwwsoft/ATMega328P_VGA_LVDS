#include "global.h"
#include "lcd_driver.h"
#include "keybrd.h"
//-----------------------------------------------------------------------------
extern char s_vga_800_600_60[];
extern char s_vga_rows[];
extern char s_vga_red[];
extern char s_vga_green[];
extern char s_vga_blue[];
extern short vr;
extern char s_vga_gen[];
//-----------------------------------------------------------------------------
void setup_800_600_60(){
//   Timer 1 - vertical sync pulses
  ResetTimers();
  //set non inverted PWM on OC1A pin
  //and inverted on OC1B
  TCCR1A |= (0<<COM1A1)|(0<<COM1A0)|(1<<COM1B1)|(0<<COM1B0);
  //set top value to ICR1
  ICR1H = 16;     //
  ICR1L = 50;     //
  TCCR1A |= (0<<WGM10)|(1<<WGM11);
  TCCR1B |= (1<<WGM12)|(1<<WGM13);
  OCR1BH=0;
  OCR1BL=25;   //
  //clk/8      t= 4 mks
  TCCR1B |= (0<<CS12)|(1<<CS11)|(1<<CS10);

  TCCR2A |= (1<<COM2B1)|(0<<COM2B0)|(1<<WGM20)|(1<<WGM21);
  TCCR2B |= (1<<WGM22);
  OCR2A = 51;   //
  OCR2B = 5;    //
  TIFR2 = 0;
  TIMSK2 |= (1<<TOIE2)|(1<<OCIE2B);
  //clk/8      t = 0.5 mks
  TCCR2B |= (0<<CS22)|(1<<CS21)|(0<<CS20);
}  // end of setup
//-----------------------------------------------------------------------------
void Menu_800_600_60(){
 short sel, inv, key;
 SREG.B7 = 0;
 PORTD = 0;
 sel = 2;
 while(1){
_start:;
  Delay_ms(300);
  L3310_ClearScreen();
  L3310_WriteStringAtCenter(0, s_vga_800_600_60, 0);
  if(sel == 2){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(2, s_vga_rows, inv);
  if(sel == 3){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(3, s_vga_red, inv);
  if(sel == 4){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(4, s_vga_green, inv);
  if(sel == 5){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(5, s_vga_blue, inv);
  do{
  }while( (key = GetPresedKey()) == key_nothing);
//  key = key_select;
  //select
  Delay_100ms();
  if (key == key_select){
   switch(sel){
    //rows
    case 2:
next_2:;
     Delay_ms(300);
     vr = 4;
     //show current mode to screen
     L3310_ClearScreen();
     L3310_WriteStringAtCenter(0, s_vga_800_600_60, 1);
     L3310_WriteStringAtCenter(2, s_vga_gen, 0);
     L3310_WriteStringAtCenter(4, s_vga_rows, 0);
     setup_800_600_60();
     SREG.B7 = 1;
     //wait for interrupt generator
     do{
     }while( PINB4_bit == 1);
     SREG.B7 = 0;
     TCCR2B &= (0<<CS22)|(0<<CS21)|(0<<CS20);
     TCCR1B &= (0<<CS12)|(0<<CS11)|(0<<CS10);
     if (PINB4_bit == 0)
      if (PINB3_bit == 0)
       goto _start;
     goto next_3;
    break;
    //red
    case 3:
next_3:;
     Delay_ms(300);
     vr = 5;
     //show current mode to screen
     L3310_ClearScreen();
     L3310_WriteStringAtCenter(0, s_vga_800_600_60, 1);
     L3310_WriteStringAtCenter(2, s_vga_gen, 0);
     L3310_WriteStringAtCenter(4, s_vga_red, 0);
     setup_800_600_60();
     SREG.B7 = 1;
     //wait for interrupt generator
     do{
     }while( PINB4_bit == 1);
     SREG.B7 = 0;
     TCCR2B &= (0<<CS22)|(0<<CS21)|(0<<CS20);
     TCCR1B &= (0<<CS12)|(0<<CS11)|(0<<CS10);
     if (PINB4_bit == 0)
      if (PINB3_bit == 0)
       goto _start;
     goto next_4;
    break;
    //green
    case 4:
next_4:;
     vr = 6;
     Delay_ms(300);
     //show current mode to screen
     L3310_ClearScreen();
     L3310_WriteStringAtCenter(0, s_vga_800_600_60, 1);
     L3310_WriteStringAtCenter(2, s_vga_gen, 0);
     L3310_WriteStringAtCenter(4, s_vga_green, 0);
     setup_800_600_60();
     SREG.B7 = 1;
     //wait for interrupt generator
     do{
     }while( PINB4_bit == 1);
     SREG.B7 = 0;
     TCCR2B &= (0<<CS22)|(0<<CS21)|(0<<CS20);
     TCCR1B &= (0<<CS12)|(0<<CS11)|(0<<CS10);
     if (PINB4_bit == 0)
      if (PINB3_bit == 0)
       goto _start;
     goto next_5;
    break;
    //blue
    case 5:
next_5:;
     vr = 7;
     Delay_ms(300);
     //show current mode to screen
     L3310_ClearScreen();
     L3310_WriteStringAtCenter(0, s_vga_800_600_60, 1);
     L3310_WriteStringAtCenter(2, s_vga_gen, 0);
     L3310_WriteStringAtCenter(4, s_vga_blue, 0);
     setup_800_600_60();
     SREG.B7 = 1;
     //wait for interrupt generator
     do{
     }while( PINB4_bit == 1);
     SREG.B7 = 0;
     TCCR2B &= (0<<CS22)|(0<<CS21)|(0<<CS20);
     TCCR1B &= (0<<CS12)|(0<<CS11)|(0<<CS10);
     if (PINB4_bit == 0)
      if (PINB3_bit == 0)
       goto _start;
     goto next_2;
    break;
   }
  }
  //up
  else if (key == key_up){
   if (sel > 2)
    sel --;
  }
  //down
  else if (key == key_down){
   if (sel < 5)
    sel ++;
  }//
  else if (key == key_select_long){
   SREG.B7 = 0;
   return;
  }
 }
}
//-----------------------------------------------------------------------------