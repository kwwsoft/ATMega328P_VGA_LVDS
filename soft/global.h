#ifndef __global_h
#define __global_h
//-----------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define key_nothing  0
#define key_up       1
#define key_down     2
#define key_select   3
#define key_up_long      4
#define key_down_long    5
#define key_select_long  6

#define main_menu        4

#define black        0b00000000
#define red          0b00000001
#define green        0b00000010
#define blue         0b00000100
#define white        0b00000111

#define width_640      3
#define width_800      2
//------------------------------------------------------------------------------
void InitMega();
void ResetTimers();
void SetScalerResolution(short res);
void ChangeScalerResolution();
void IntToString(unsigned int num, char *str);
void ConvertFloatToString(float num, char *str);
//-----------------------------------------------------------------------------
void DebugShort(short y, short val);
//-----------------------------------------------------------------------------
#endif