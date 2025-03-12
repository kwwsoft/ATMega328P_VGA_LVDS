void setup_1024_768_60(){
//   Timer 1 - vertical sync pulses
//  Set OC1A/OC1B on Compare Match, clearO C1A/OC1B at BOTTOM (inverting mode)

  TCNT1H = 0;
  TCNT1L = 0;
  //
  TCCR2A |= (1<<COM2B1)|(0<<COM2B0)|(1<<WGM20)|(1<<WGM21);
  TCCR2B |= (1<<WGM22);
  OCR2A = 51;   //40*0.5 =  20 mks  (20.4 mks)
  OCR2B = 5;    //4*0.5 = 1.5 mks  (2.0 mks)
  TIFR2 = 0;
  TIMSK2 |= (1<<TOIE2)|(1<<OCIE2B);
  //clk/8      t = 0.5 mks
  TCCR2B |= (0<<CS22)|(1<<CS21)|(0<<CS20);

  //set non inverted PWM on OC1A pin
  //and inverted on OC1B
  TCCR1A |= (0<<COM1A1)|(0<<COM1A0)|(1<<COM1B1)|(0<<COM1B0);
  //set top value to ICR1
  ICR1H = 16;     // (4*256 + 13)*16 = 16592 mks   (16610 mks)
  ICR1L = 50;    //
  TCCR1A |= (0<<WGM10)|(1<<WGM11);
  TCCR1B |= (1<<WGM12)|(1<<WGM13);
  OCR1BH=0;
  OCR1BL=25;   //6*16 = 96 mks  (110 mks)
  //clk/8      t= 4 mks
  TCCR1B |= (0<<CS12)|(1<<CS11)|(1<<CS10);
}  // end of setup