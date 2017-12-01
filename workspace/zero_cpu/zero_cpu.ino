#define STEP_PIN        3 //OC2B
#define DIRECTION_PIN   6 //arbitrary
#define ENABLE_PIN      9 // OC1A
#define MS1_PIN         11 // arbitrary
#define MS2_PIN         12 // arbitrary
#define MS3_PIN 0 // not used
#define PULSE_COUNTER_PIN 5 // T1 (clocksource for timer1)
#define STEP_DELAY 350

volatile int pulsesDone = 1;
boolean enableMotor = true;

int i=7;

void startPulses(unsigned int period, unsigned int count);
void setup() {
  // put your setup code here, to run once:
  // put your setup code here, to run once:
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(MS1_PIN, OUTPUT);
  pinMode(MS2_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(DIRECTION_PIN, HIGH);
  digitalWrite(ENABLE_PIN, HIGH);
  digitalWrite(MS1_PIN, HIGH);
  digitalWrite(MS2_PIN, LOW);
  pinMode(13, OUTPUT);

}

void loop() {
  delay(2000);
  if(i>=0){
    startPulses(500,400);
    enableMotor=false;
    i--;
  }
}


void startPulses( unsigned int period, unsigned int count) {
  digitalWrite(13, !digitalRead(13));

  TCCR1B = 0; //disable timer1
  TIFR1 = (1<<OCF1A);
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
  OCR2A = period/16-1;    //62.5 kHz = 16 micro seconds
  OCR2B = 0;
}
