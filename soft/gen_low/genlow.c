#include "built_in.h"
#include "keybrd.h"
#include "lcd_driver.h"
#include "global.h"
//-----------------------------------------------------------------------------
extern char s_500[];
extern char s_1000[];
extern char s_2000[];
extern char s_5000[];
extern char s_15000[];
extern char s_freqlow[];
extern char s_freq[];
extern char s_gen[];
extern char s_dutylow[];
//-----------------------------------------------------------------------------
//
short i_gen_low_max = 41;
//
char *s_gen_low[] = {"500","1 000",
                     "2 000", "4 000", "6 000", "8 000", "10 000",
                     "12 003","14 010","16 000","18 018","20 000",
                     "22 008","24 024","26 016","28 021","30 018",
                     "32 000","34 042","36 036","38 005","40 000",
                     "42 105","44 077","46 109","48 048","50 000",
                     "52 117","54 054","56 140","58 182","60 150",
                     "70 175","80 000","90 395","100 000","110 345",
                     "121 121","131 148","140 351","150 943","160 000"};
//
unsigned int i_gen_low[] = {31999,15999,
                            7999, 3999, 2665, 1999, 1599,
                            1332, 1141, 999, 887, 799,
                            726, 665, 614, 570, 532,
                            499, 469, 443, 420, 399,
                            379, 362, 346, 332, 319,
                            306, 295, 284, 274, 265,
                            227, 199, 176, 159, 144,
                            132, 122, 113, 105, 99};
//-----------------------------------------------------------------------------
volatile unsigned short periodh, periodl, dutyh, dutyl;
unsigned int period, duty;
//-----------------------------------------------------------------------------
//генераци€ идет по выовду PB1 (15) таймера1 16-ти битного
/*
 Fclk = 16000000
      Npresc = 1
             ICR1 = 9          F = 1 600 000        0.625 mks
             ICR1 = 65535      F = 244              4.098 ms
      Npresc = 8
             ICR1 = 9          F = 200 000          5 mks
             ICR1 = 65535      F = 30               33 ms
      Npresc = 64
             ICR1 = 9          F = 25 000           40 mks
             ICR1 = 65535      F = 3.8              263 ms
      Npresc = 256
             ICR1 = 9          F = 6 250            1.6 ms
             ICR1 = 65535      F = 0.95             1.05 s
      Npresc = 1024
             ICR1 = 9          F = 1 562            64 ms
             ICR1 = 65535      F = 0.24             4.16 s
 volatile unsigned short periodh, periodl, dutyh, dutyl;
 unsigned int period, duty;
 //period = Fclk / (Fpwm * Npresc) - 1
 //period = 16000000 / (250 * 256) - 1      = 249
 period = 249;
 periodl = Lo(period);
 periodh = Hi(period);
 duty = period / 2;
 dutyl = Lo(duty);
 dutyh = Hi(duty);
 //ICNC1 ICES1 Ц WGM13 WGM12 CS12 CS11 CS10
 TCCR1B = 0b00011000;
 //COM1A1 COM1A0 COM1B1 COM1B0 Ц Ц WGM11 WGM10
 TCCR1A = 0b10000010;
 //set top value to ICR1
 ICR1H = periodh;     //
 ICR1L = periodl;    //
 //
 OCR1AH = dutyh;
 OCR1AL = dutyl;
 //clk/256
 TCCR1B |= (1<<CS12);
*/
//-----------------------------------------------------------------------------
void GenLowRun(short reg){
 unsigned short sel, key;
 char txt[4];
 sel = 50;
 while(1){
_start:;
  duty = (int)((float)period * ( sel / 100.0));
  L3310_ClearScreen();
  L3310_WriteStringAtCenter(0, s_gen, 0);
  L3310_WriteStringAtCenter(2, s_freqlow, 0);
  L3310_WriteStringAtCenter(3, s_gen_low[reg], 1);
  //скважность
  L3310_WriteStringAtCenter(4, s_dutylow, 0);
  ByteToStr(sel, txt);
  L3310_WriteStringAtCenter(5, txt, 1);
  //
  dutyl = Lo(duty);
  dutyh = Hi(duty);
  OCR1AH = dutyh;
  OCR1AL = dutyl;
  //
  do{
  }while( (key = GetPresedKey()) == key_nothing);
//  key = key_select;
  //select
  if (key == key_select){
  }
  //up
  else if (key == key_up){
   if (sel < 98)
    sel += 2;
  }
  //down
  else if (key == key_down){
   if (sel > 4)
    sel -= 2;
  }//
  else if (key == key_select_long){
   //set bits to 0
   TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));
   return;
  }
  else if (key == key_down_long){
   sel = 50;
   goto _start;
  }//
 }
}
//-----------------------------------------------------------------------------
void SetupAndRunGenerator(short sel){
 period = i_gen_low[sel];
// period = 31999;
 periodl = Lo(period);
 periodh = Hi(period);
 duty = (int)((float)period * 0.5);
 dutyl = Lo(duty);
 dutyh = Hi(duty);
 //ICNC1 ICES1 Ц WGM13 WGM12 CS12 CS11 CS10
 TCCR1B = 0b00011000;
 //COM1A1 COM1A0 COM1B1 COM1B0 Ц Ц WGM11 WGM10
 TCCR1A = 0b10000010;
 //set top value to ICR1
 ICR1H = periodh;     //
 ICR1L = periodl;    //
 //
 OCR1AH = dutyh;
 OCR1AL = dutyl;
 //clk/1
 TCCR1B |= (1<<CS10);
}
//-----------------------------------------------------------------------------
void MenuGenLow(){
 unsigned short sel, key;
 unsigned short res;
 SREG.B7 = 0;
 DDRB = 0b11100111;
 DDRD = 0b11111111;
 DDRC = 0b11111100;
 PORTD = 0;
 sel = EEPROM_Read(0);
 if (sel > i_gen_low_max)
  sel = 0;
 while(1){
_start:;
  ResetTimers();
  L3310_ClearScreen();
  L3310_WriteStringAtCenter(0, s_gen, 0);
  L3310_WriteStringAtCenter(3, s_gen_low[sel], 1);
  L3310_WriteStringAtCenter(5, s_freq, 0);
  //
  do{
  }while( (key = GetPresedKey()) == key_nothing);
//  key = key_select;
  //select
  if (key == key_select){
   EEPROM_Write(0, sel);
   SetupAndRunGenerator(sel);
   GenLowRun(sel);
  }
  //up
  else if (key == key_up){
   if (sel < i_gen_low_max)
    sel ++;
   else
    sel = 0;
  }
  //down
  else if (key == key_down){
   if (sel > 0)
    sel --;
   else
    sel = i_gen_low_max;
  }//
  else if (key == key_select_long){
   //set bits to 0
   TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));
   return;
  }
  else if (key == key_down_long){
   goto _start;
  }//
 }
}
//-----------------------------------------------------------------------------