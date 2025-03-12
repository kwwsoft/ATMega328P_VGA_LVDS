#include "built_in.h"
#include "lcd_driver.h"
#include "keybrd.h"
#include "global.h"
//------------------------------------------------------------------------------
char s_vga[] = "VGA √енератор";
char s_gen[] = "√енератор Ќ„";
char s_genhi[] = "√енератор ¬„";
char s_testopto[] = "“естер оптопар";
char s_testshort[] = "“ест  « кабел€";
char s_vga_640_480_60[] = " 640x480 60√ц ";
char s_vga_800_600_60[] = " 800x600 60√ц ";
char s_vga_1024_768_60[] = "1024x768 60√ц ";
char s_freqlow[] = "частота";
char s_freq[] = " √ц";
char s_dutylow[] = "заполнение";
char s_500[] = "500 √ц";
char s_1000[] = "1000 √ц";
char s_2000[] = "2000 √ц";
char s_5000[] = "5000 √ц";
char s_15000[] = "15 000 √ц";
char s_05m[] = "0.5 ћ√ц";
char s_1m[] = "1 ћ√ц";
char s_2m[] = "2 ћ√ц";
char s_4m[] = "4 ћ√ц";
char s_8m[] = "8 ћ√ц";
//
char s_tk_1_125[] = "1 / 125 к√ц";
char s_tk_1_230[] = "1 / 230 к√ц";


char s_vga_rows[] = "цветные полосы";
char s_vga_red[] = "красный";
char s_vga_green[] = "зеленый";
char s_vga_blue[] = "синий";
char s_vga_gen[] = "генераци€";
char *s_scaler_m[] = {"800x600-8-1",   "1024x600-6-2",  "1024x768-6-1",
                      "1024x768-6-2",  "1024x768-6-2",  "1024x768-8-1",
                      "1280x800-6-2",  "1280x1024-6-2", "1280x1024-8-1",
                      "1366x768-6-2",  "1366x768-8-1",  "1440x900-6-2",
                      "1440x900-8-1", "1440x1050-6-2", "1600x900-6-2",
                      "1600x900-8-1",  "1600x1200-8-1", "1680x1050-6-2",
                      "1680x1050-8-1", "1920x1080-6-2", "1920x1080-8-1",
                      "1920x1080-8-2", "1920x1200-6-2"};
short i_scaler_m[23] = {~0xdf, ~0xfe, ~0xbf, ~0xef, ~0xbe, ~0xf7, ~0x9f, ~0xaf, ~0xde,
                        ~0xfb, ~0xfd, ~0xbb, ~0xee, ~0xb7, ~0xbd, ~0x9e, ~0xae, ~0xb6,
                        ~0xf6, ~0xba, ~0xfc, ~0xfa, ~0xbc};
//------------------------------------------------------------------------------
//видеорежим
short vr;
//0 - 640x480x60 color rows
//1 - 640x480x60 red
//2 - 640x480x60 green
//3 - 640x480x60 blue

//4 - 800x600x60 color rows
//5 - 800x600x60 red
//6 - 800x600x60 green
//7 - 800x600x60 blue


//8 - 1024x768x60 color rows
//------------------------------------------------------------------------------
void InitMega(){
  DDRB = 0b11100111;
  DDRD = 0b11111111;
  DDRC = 0b11111111;
  PORTD = 0;
  //
  L3310_init_lcd();
  Sound_Init(&PORTB, 0);
  L3310_ClearScreen();
  L3310_WriteStringAtPos(1, 1, "“естер матриц", 0);
  L3310_WriteStringAtCenter(4, "(C) KWWsoft", 0);
  L3310_WriteStringAtCenter(5, "Ver 12.04.2020", 1);
}
//------------------------------------------------------------------------------
void ResetTimers(){
  TCCR0A = 0;
  TCCR0B = 0;
  OCR0A = 0;
  OCR0B = 0;
  TIFR0 = 0;
  TIMSK0 = 0;
  TCNT0 = 0;
  //
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1C = 0;
  TCNT1H = 0;
  TCNT1L = 0;
  OCR1AH = 0;
  OCR1AL = 0;
  OCR1BH = 0;
  OCR1BL = 0;
  ICR1H = 0;
  ICR1L = 0;
  TIMSK1 = 0;
  TIFR1 = 0;
  //
  TCCR2A = 0;
  TCCR2B = 0;
  OCR2A = 0;
  OCR2B = 0;
  TIFR2 = 0;
  TIMSK2 = 0;
  ASSR = 0;
  GTCCR = 0;
  TCNT2 = 0;
}
//------------------------------------------------------------------------------
void DebugShort(short y, short val){
 char buf[9];
 buf[8] = 0;
 if ( val & 0b10000000){buf[0] = '1';}else{buf[0] = '0';}
 if ( val & 0b01000000){buf[1] = '1';}else{buf[1] = '0';}
 if ( val & 0b00100000){buf[2] = '1';}else{buf[2] = '0';}
 if ( val & 0b00010000){buf[3] = '1';}else{buf[3] = '0';}
 if ( val & 0b00001000){buf[4] = '1';}else{buf[4] = '0';}
 if ( val & 0b00000100){buf[5] = '1';}else{buf[5] = '0';}
 if ( val & 0b00000010){buf[6] = '1';}else{buf[6] = '0';}
 if ( val & 0b00000001){buf[7] = '1';}else{buf[7] = '0';}
 L3310_WriteStringAtPos(1, y, buf, 0);
}
//------------------------------------------------------------------------------
void SetScalerResolution(short res){
 unsigned short i, val;
 //clear
 PORTC5_bit = 0;
 for (i = 0 ; i < 8; i++){
  PORTC4_bit = 1;
  Delay_500us();
  PORTC4_bit = 0;
  Delay_500us();
 }
 //set
 val = i_scaler_m[res];
 for (i = 0 ; i < 8; i++){
  if ( (val & 0x80) == 0x80 )
   PORTC5_bit = 1;
  else
   PORTC5_bit = 0;
  Delay_22us();
  PORTC4_bit = 1;
  Delay_500us();
  PORTC4_bit = 0;
  Delay_500us();
  val <<= 1;
 }
}
//------------------------------------------------------------------------------
void ChangeScalerResolution(){
 unsigned short key, res;
 SREG.B7 = 0;
 res = EEPROM_Read(0);
 if (res > 22){
  res = 10;
  EEPROM_Write(0, res);
 }
 while(1){
_start:;
  L3310_ClearScreen();
  //вывод типа разрешени€ матрицы и установка перемычек
  L3310_WriteStringAtCenter(0, "LVDS type", 0);
  //
  L3310_WriteStringAtCenter(3, s_scaler_m[res], 1);
  SetScalerResolution(res);
  if (res > 0)
   L3310_WriteStringAtCenter(2, s_scaler_m[res - 1], 0);
  if (res < 22)
   L3310_WriteStringAtCenter(4, s_scaler_m[res + 1], 0);
  //
  do{
  }while( (key = GetPresedKey()) == key_nothing);
//  key = key_select;
  //select
  if (key == key_select){
   EEPROM_Write(0, res);
   L3310_ClearScreen();
   L3310_WriteStringAtCenter(3, "LVDS saved !", 0);
   Delay_1sec();
   return;
  }
  //up
  else if (key == key_up){
   if (res >= 1)
    res --;
   SetScalerResolution(res);
 }
  //down
  else if (key == key_down){
   if (res < 22)
    res ++;
   SetScalerResolution(res);
  }//
  else if (key == key_select_long){
   return;
  }
 }
}
//------------------------------------------------------------------------------
void IntToString(unsigned int num, char *str){
/* char out[6];
 //
 WordToStr(num, out);
 str[0] = '0';  str[1] = '.';  str[2] = '0';  str[3] = '0';  str[4] = 0;
 // 99/10000 = 0.00
 if (num < 100){
  return;
 }
 // 999/10000 = 0.09
 // out xx2xx
 else if (num < 1000){
  str[3] = out[2];
  return;
 }
 // 9999/10000 = 0.99
 // out x12xx
 else if (num < 10000){
  str[2] = out[1];
  str[3] = out[2];
  return;
 }
 // 49999/10000 = 4.99
 // out 012xx
 else {
  str[0] = out[0];
  str[2] = out[1];
  str[3] = out[2];
  return;
 }*/
}
//------------------------------------------------------------------------------
void ConvertFloatToString(float num, char *str){
//
 char buf[15];
 str[0] = '0';  str[1] = '.';  str[2] = '0';  str[3] = '0';  str[4] = 0;
 //
 FloatToStr(num, buf);
 //1.23e-3
 if (num < 0.01){
 }
 //1.23e-2
 else if (num < 0.1){
  str[3] = buf[0];
 }
 //1.23e-1
 else if (num < 1){
  str[2] = buf[0];
  str[3] = buf[2];
 }
 //1.23
 else{
  str[0] = buf[0];
  str[2] = buf[2];
  str[3] = buf[3];
 }
}
//------------------------------------------------------------------------------