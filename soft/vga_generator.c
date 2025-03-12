#include "global.h"
#include "lcd_driver.h"
#include "keybrd.h"
#include "640_480_60.h"
#include "800_600_60.h"
#include "1024_768_60.h"
//------------------------------------------------------------------------------
extern char s_vga[];
extern char s_vga_640_480_60[];
extern char s_vga_800_600_60[];
extern char s_vga_1024_768_60[];
extern char *s_scaler_m[];
//------------------------------------------------------------------------------
void MenuVGAGenerator(){
 unsigned short sel, inv, key;
 unsigned short res;
 SREG.B7 = 0;
 DDRB = 0b11100111;
 DDRD = 0b11111111;
 DDRC = 0b11111111;
 PORTD = 0;
 sel = 0;
 //вывод типа разрешения матрицы и установка перемычек
 res = EEPROM_Read(0);
 if (res > 22)
  res = 10;
 SetScalerResolution(res);
 //
 while(1){
_start:;
  L3310_ClearScreen();
  if(sel == 0){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(0, s_vga_640_480_60, inv);
  if(sel == 1){inv = 1;}else{inv = 0;}
  L3310_WriteStringAtCenter(1, s_vga_800_600_60, inv);
//  if(sel == 2){inv = 1;}else{inv = 0;}
 // L3310_WriteStringAtCenter(2, s_vga_1024_768_60, inv);
  //вывод типа разрешения матрицы и установка перемычек
  L3310_WriteStringAtCenter(5, s_scaler_m[res], 0);
  //
  do{
  }while( (key = GetPresedKey()) == key_nothing);
//  key = key_select;
  //select
  if (key == key_select){
   switch(sel){
    //vga_640_480_60
    case 0:
     Menu_640_480_60();
     goto _start;
    break;
    //vga_800_600_60
    case 1:
     Menu_800_600_60();
    break;
    //vga_1024_768_60
    case 2:
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
   if (sel < 1)
    sel ++;
  }//
  else if (key == key_select_long){
   return;
  }
  else if (key == key_down_long){
   ChangeScalerResolution();
   res = EEPROM_Read(0);
   if (res > 22)
    res = 10;
   SetScalerResolution(res);
   goto _start;
  }//
 }
}
//------------------------------------------------------------------------------