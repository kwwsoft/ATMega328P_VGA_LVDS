#include "global.h"
//------------------------------------------------------------------------------
short key_press;
unsigned long int key_delay;
//------------------------------------------------------------------------------
char GetPresedKey(){
// 7654 3210
// xxx1 1xxx - nothing
// xxx0 0xxx - Select
// xxx0 1xxx - up
// xxx1 0xxx - down
 //get first key
 key_press = PINB & 0b00011000;
 if ( key_press == 0b00011000){
  return 0;
 }
 //
 key_delay = 0;
 //
 do{
  key_delay ++;
  //long press
  if (key_delay > 200000){
   //если зависка непонятка
   if (key_delay > 900000){
    return key_nothing;
   }
   //select long key pressed
   if (key_press == 0b00000000){
    key_press = key_select_long;
   }
   //up long key pressed
   else if (key_press == 0b00001000){
    key_press = key_up_long;
   }
   //down long key pressed
   else{
    key_press = key_down_long;
   }
   Sound_Play(2000, 20);
   Sound_Play(3000, 10);
   Sound_Play(4000, 20);
   Sound_Play(2000, 10);
   //wait key release
   do{
   }while((PINB & 0b00011000) != 0b00011000);
   //return key;
   return key_press;
  }
 }while(key_press == (PINB & 0b00011000));
  //key is not pressed for a needed time 50ms
 if (key_delay < 16000){  //
  return key_nothing;
 }
 //select  pressed
 if (key_press == 0b00000000){
  key_press = key_select;
 }
 //up  key pressed
 else if (key_press == 0b00001000){
  key_press = key_up;
 }
 //down  key pressed
 else{
  key_press = key_down;
 }
 Sound_Play(1000, 10);
 //wait key release
 do{
 }while((PINB & 0b00011000) != 0b00011000);
//return pressed key
 return key_press;
}
//------------------------------------------------------------------------------
char GetPresedKeyFastSelect(){
 switch(PINB & 0b00011000){
  case 0b00001000:
   return key_up;
  break;
  case 0b00010000:
   return key_down;
  break;
  case 0b00000000:
   return key_select;
  break;
  default:
   return key_nothing;
  break;
 }
}
//------------------------------------------------------------------------------