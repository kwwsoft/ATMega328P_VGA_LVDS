#include "built_in.h"
#include "keybrd.h"
#include "lcd_driver.h"
#include "global.h"
//-----------------------------------------------------------------------------
extern char s_testopto[];
float const Uref = 5.05;
float const Ri = 514.0;
float const Ro = 512.0;
//ראד אצן
float adc_step = (Uref / 256.0);
//-----------------------------------------------------------------------------
short GetADCSample(short can){
//
 short count = 7;
 unsigned int summa = 0;
 //
 if (can == 0){
  MUX0_bit = 0;
 }
 else{
  MUX0_bit = 1;
 }
 //
 do{
  Delay_500us();
  ADCSRA |= (1 << ADSC);
   do{
   }while (ADCSRA & (1 << ADSC));
   summa += ADCH;
  }while(count-- > 0);
 //
//  ByteToStr(adcin, str);
//  FloatToStr((float)adcin, str);
//  L3310_WriteStringAtPos(0 , 5, str , 0);
 return summa / 8;
}
//-----------------------------------------------------------------------------
void MenuTestOpto(){
 unsigned short key;
 float adcU;
 unsigned short adcin;
 float current;
 char txt[5];
 SREG.B7 = 0;
 DDRB = 0b11100111;
 DDRD = 0b11111111;
 DDRC = 0b11111100;
 PORTD = 0;
 ResetTimers();
 //
 ADMUX = 0;
 ADCSRA = 0;
 ADCSRB = 0;
 DIDR0 = 0;
 //
 ADCSRA |= (1<<ADEN);
 //REFS1 REFS0 Voltage Reference Selection
 //  0     1   AVCC with external capacitor at AREF pin
 //ADLAR: ADC Left Adjust Result
 ADMUX |= (0<<REFS1) | (1<<REFS0) | (1<<ADLAR);
 //1 1 1 128
 ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
 //
 L3310_ClearScreen();
 L3310_WriteStringAtCenter(0, s_testopto, 0);
 //                            01234567890123
 L3310_WriteStringAtPos(0, 1, "    OFF    On ", 0);
 L3310_WriteStringAtPos(0, 2, "Ui  -.--  -.--", 0);
 L3310_WriteStringAtPos(0, 3, "Ii  -.--  -.--", 0);
 L3310_WriteStringAtPos(0, 4, "Uo  -.--  -.--", 0);
 L3310_WriteStringAtPos(0, 5, "Io  -.--  -.--", 0);
  //
 do{
  key = GetPresedKey();
  //Uin = 0     Uin        opto closed
  PORTB1_bit = 0;
  //
  Delay_ms(200);
  adcin = GetADCSample(1);
  adcU = adc_step * adcin;
  ConvertFloatToString(adcU , txt);
  L3310_WriteStringAtPos(4*6 , 2, txt , 0);
  //Uin = 0     Iin
  L3310_WriteStringAtPos(4*6 , 3, "0.00" , 0);
  //Uin = 0     Uout
  adcin = GetADCSample(0);
  adcU = adc_step * adcin;
  ConvertFloatToString(adcU , txt);
  L3310_WriteStringAtPos(4*6 , 4, txt , 0);
  //Uin = 0     Iout
  current = ((Uref - adcU) / Ro) * 1000 ;
  ConvertFloatToString( current , txt);
  L3310_WriteStringAtPos(4*6 , 5, txt , 0);
  //
  //
  //Uin = 1     Uin        opto opened
  PORTB1_bit = 1;
  //
  Delay_ms(200);
  adcin = GetADCSample(1);
  adcU = adc_step * adcin;
  ConvertFloatToString(adcU , txt);
  L3310_WriteStringAtPos(10*6 , 2, txt , 0);
  //Uin = 0     Iin
  current = ((Uref - adcU) / Ri) * 1000 ;
  ConvertFloatToString( current , txt);
  L3310_WriteStringAtPos(10*6 , 3, txt , 0);
  //Uin = 0     Uout
  adcin = GetADCSample(0);
  adcU = adc_step * adcin;
  ConvertFloatToString(adcU , txt);
  L3310_WriteStringAtPos(10*6 , 4, txt , 0);
  //Uin = 0     Iout
  current = ((Uref - adcU) / Ro) * 1000 ;
  ConvertFloatToString( current , txt);
  L3310_WriteStringAtPos(10*6 , 5, txt , 0);
 }while( key != key_select_long);
}
//-----------------------------------------------------------------------------