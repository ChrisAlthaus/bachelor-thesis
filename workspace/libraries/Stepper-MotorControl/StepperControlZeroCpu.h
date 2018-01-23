#ifndef StepperControlZeroCpu_h
#define StepperControlZeroCpu_h

#define PERIODIC_TIME 500  //micro seconds


void startPulses(unsigned int count) {	//don't call with 0 as argument
  //digitalWrite(13, !digitalRead(13));

  TCCR1B = 0; //disable timer1
  TIFR1 = (1<<OCF1A); //clear register
  TCNT1 = 0;
  TCCR1A = (1<<COM1A1)|(0<<COM1A0); //clear enable pin on next compare match
  TCCR1C = (1<<FOC1A); //Force compare match to clean enable


  //Timer1 counting the pulses
  TCCR1A = (1<<COM1A1)|(1<<COM1A0); //set enable pin (low active)
  OCR1A = count-1;
  TCCR1B = (1<<CS12)|(1<<CS11)|(1<<CS10); //Clock T1 falling, normal mode


  //Timer2 generate pulses
  DDRD = (1<<PD3);      //set pin 3 
  TCCR2A = (1<<COM2B0)| //Toggle output PD3 on compare match
           (1<<WGM21);
  TCCR2B = (1<<CS22)|(1<<CS21)|(0<<CS20); // Prescaler 256 --> 62.5 kHz
  OCR2A = PERIODIC_TIME/16-1;    //62.5 kHz = 16 micro seconds
  OCR2B = 0;
}


#endif