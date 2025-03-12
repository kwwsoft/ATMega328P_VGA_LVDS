char ch6x8[7];
//-----------------------------------------------------------------------------
#include "built_in.h"
#include "chars.h"
//-----------------------------------------------------------------------------
//        x = 0...13
//        y = 0...5
//-----------------------------------------------------------------------------
//управление дисплеем NOKIA 5510
//          Res(1)    PB5      0b00100000
//          D/C(3)    PD4      0b00010000
//          Sclk(5)   PD7      0b10000000
//          Sdin(4)   PD6      0b01000000
//----------------------------------------------------------------------------
#define     reset     PORTB5_bit
#define     d_c       PORTD4_bit
#define     sdin      PORTD6_bit
#define     sclk      PORTD7_bit
//-----------------------------------------------------------------------------
void writecom(char);                                // Writes a command.
void writedata(char);                                // Writes data to DDRAM to illuminate the pixels.
void clockdata(char);                                // Clocks in data into the PCD8544 controller.
void clearram(void);                                // Erase the DDRAM contents.
void GetOneChar_6x8(char ch);
//-----------------------------------------------------------------------------
void L3310_cursorxy(char x, char y){
//x = 0...83
//y = 0...5
        writecom( 0x40 | y );        // Y axis
        writecom( 0x80 | x );        // X axis
}
//-----------------------------------------------------------------------------
void L3310_ClearScreen(){
 int ddram;
 L3310_cursorxy(0,0);                                                                                        // Cursor Home.
 for ( ddram = 504; ddram > 0; ddram--){
  writedata(0x00);
 }                // 6*84 = 504 DDRAM addresses.
}
//-----------------------------------------------------------------------------
void clockdata(char bits_in){
//вывод 1-го байта в шину со старшего бита до младшего
 char bitcnt;
 for (bitcnt = 8; bitcnt > 0; bitcnt --){
    sclk = 0;
    if ( (bits_in & 0x80) == 0x80 ){ //вывод идет со старшего бита
     sdin = 1;
    }
    else {
     sdin = 0;
    }
    sclk = 1;                                                                // Data is clocked on the rising edge of SCK.
    bits_in <<= 1;                            // Logical shift data by 1 bit left.
 }
 sclk = 0;                                                                // Set Clock Idle level LOW.
}
//-----------------------------------------------------------------------------
void writecom(char command_in){
  d_c = 0;                                // Select Command register.
  clockdata(command_in);        // Clock in command bits.
}
//-----------------------------------------------------------------------------
void writedata(char data_in){
 d_c = 1;                                // Select Data register.
 clockdata(data_in);                // Clock in data bits.
}
//-----------------------------------------------------------------------------
void L3310_WriteChar(char ch){
 GetOneChar_6x8(ch);
 writedata(ch6x8[0]);
 writedata(ch6x8[1]);
 writedata(ch6x8[2]);
 writedata(ch6x8[3]);
 writedata(ch6x8[4]);
 writedata(ch6x8[5]);
}
//-----------------------------------------------------------------------------
void L3310_WriteCharInv(char ch){
 GetOneChar_6x8(ch);
 writedata(~ch6x8[0]);
 writedata(~ch6x8[1]);
 writedata(~ch6x8[2]);
 writedata(~ch6x8[3]);
 writedata(~ch6x8[4]);
 writedata(~ch6x8[5]);
}
//-----------------------------------------------------------------------------
void L3310_WriteCharAtPos(char x, char y, char ch, char inv){
 L3310_cursorxy(x, y);
 if (inv == 0){
  L3310_WriteChar(ch);
 }
 else{
  L3310_WriteCharInv(ch);
 }
}
//-----------------------------------------------------------------------------
void L3310_WriteStringAtPos(char x, char y, char *str, char inv){
 int len, i;
 len = strlen(str);
 L3310_cursorxy(x, y);
 if (inv == 0){
  for (i = 0; i < len; i ++){
   L3310_WriteChar(str[i]);
  }
 }
 else{
  for (i = 0; i < len; i ++){
   L3310_WriteCharInv(str[i]);
  }
 }
}
//-----------------------------------------------------------------------------
void L3310_init_lcd(){
 PORTD = 0;
 reset = 1 ;
 Delay_100ms();
 reset = 0 ;
 Delay_100ms();
 reset = 1 ;
 Delay_100ms();
 writecom(0x21);                // Activate Chip and H=1.
 writecom(0xC0);                // Set LCD Voltage to about 7V.
 writecom(0x12);                // Adjust voltage bias.
 writecom(0x20);                // Horizontal addressing and H=0.
 writecom(0x09);                // Activate all segments.
 L3310_ClearScreen();                        // Erase all pixel on the DDRAM.
 writecom(0x08);                // Blank the Display.
 writecom(0x0C);                // Display Normal.
 L3310_cursorxy(0,0);                // Cursor Home.
}
//-----------------------------------------------------------------------------
void L3310_WriteStringAtCenter(char y, char *str, char inv){
//вывод строки с центровкой на экране с инверсной полосой на всю ширину
//или без инверсии
 char bufer[15];
 short i, j;
 char ch_bef;
 short len;
 L3310_cursorxy(0, y);
// clear array
 for (i = 0; i < 14; i ++){
  bufer[i] = ' ';
 }
 bufer[14] = 0;
 //смотрим скока пробелов нужно
 len = strlen(str);
 ch_bef = (14 - len) / 2;
 //make string
 i = 0;
 j = 0;
 //ставим пробелы в начале
 while(ch_bef != 0){
  ch_bef --;
  i ++;
 }
 //вставляем строку
 while(j < len){
  bufer[i++] = str[j++];
 }
 //show string to screen
 i = 0;
 while(i < 14){
  if (inv == 0){
   L3310_WriteChar(bufer[i]);
  }
  else{
   L3310_WriteCharInv(bufer[i]);
  }
  i ++;
 }
}
//-----------------------------------------------------------------------------
void GetOneChar_6x8(char ch){
 unsigned int tmp;
 tmp = ch * 8;
 ch6x8[0] = chargen[tmp++];
 ch6x8[1] = chargen[tmp++];
 ch6x8[2] = chargen[tmp++];
 ch6x8[3] = chargen[tmp++];
 ch6x8[4] = chargen[tmp++];
 ch6x8[5] = chargen[tmp];
 ch6x8[6] = 0;
}
//-----------------------------------------------------------------------------
