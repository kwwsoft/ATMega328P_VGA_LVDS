#ifndef __lcd_driver_h
#define __lcd_driver_h
//-----------------------------------------------------------------------------
//управление дисплеем NOKIA 3310
//-----------------------------------------------------------------------------
void L3310_init_lcd();
void L3310_ClearScreen();
void L3310_WriteStringAtPos(char x, char y, char *str, char inv);
void L3310_WriteCharAtPos(char x, char y, char ch, char inv);
void L3310_WriteStringAtCenter(char y, char *str, char inv);
//-----------------------------------------------------------------------------
#endif