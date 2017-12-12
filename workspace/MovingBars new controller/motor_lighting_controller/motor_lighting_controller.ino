#include <Stepper-MotorControl.h>
#include <AlaLedLite.h>         // modified Arduino Animation Library http://yaab-arduino.blogspot.de/p/ala.html
#include <Debugger.h> 
#include <String.h>

#include "Modes.h"
#include <SPI.h>

#define BAUD_RATE       9600 

#define LED_PIN         8       // D8
#define MESSAGE_LENGTH 100


MotorControl ctl;
AlaLedLite led;                   // LED Stripe
Debugger debug;                   // Debugger


char message[MESSAGE_LENGTH];
volatile byte pos;
volatile boolean process_it;
int errors =0;

boolean _hasNewTargetPosition = false;


void setup() {
  Serial.begin(BAUD_RATE);
  
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  
  // get ready for an interrupt 
  pos = 0;   // buffer empty
  process_it = false;

  // turn on interrupts
  SPI.attachInterrupt();
  
  //Pin configuration for stepper motor
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

  led.initBarStripes(LED_PIN); 
  //led.setRefreshRate(10); //adjust when flickering
}

void loop() {
  //test();
 
  
 int availableBytes=Serial.available();

  if(availableBytes>0){
    Serial.println(availableBytes);
    for(int i=0; i< availableBytes;i++){
      message[i]=Serial.read()-'0';
    }

    int messageLength= strlen(message);
    message[messageLength]=0;
    
    parseMessage();
  }
	
	if (process_it){
    message [pos] = 0;
    //Serial.println("message=");  
    //Serial.println(message[0]);
	  //Serial.println(message[1]);
    //Serial.println(message[2]);-
    //Serial.println(message[3]);
    
    if(sizeof(message)!=100){
      Serial.println("error");
    }
    pos = 0;
    process_it = false;
	
	  parseMessage();
  } 


    
    // Perfom animations:
    // 1. setAnimation
    // 2. runAnimation
  //led.runAnimation();
  //testButtons();
  //delay(1000);
}

// SPI interrupt routine
ISR (SPI_STC_vect)
{
 byte c = SPDR;  // grab byte from SPI Data Register
 
  // add to buffer if room
  if (pos < sizeof message)
    {
    message [pos++] = char(c);
    //Serial.println("get a byte");
    //Serial.println(c);
    if (c == '\n')
      process_it = true;
      
    }  
}  



void test(){
  uint8_t b1= B11111111;
  uint8_t b2= B0;
  uint8_t b3= B0;
  
  String hexValue = getColorHexValue(b1,b2,b3);
  //Serial.println(hexValue);

  uint8_t value= B10011101;
  uint8_t bitMaskedValue= getValueBitMask(value,4,1);
  //Serial.println(bitMaskedValue,BIN);
  
}

void testButtons(){
  debug.println("Button1=",ctl.getButton1Value());
  debug.println("Button2=",ctl.getButton2Value());
  debug.println("IR=",ctl.getIRValue());

  delay(1000);
}


void parseMessage() {
  int mode=message[0];
 
  Serial.println("Parsing message....");
  
  if(mode == MODE.MOVE){
    int targetPosition=50; //message[1];
    String stepMode="HALF";getStepMode(message[2]);

    if(isValidPosition(targetPosition)& stepMode != "INVALID"){
      _hasNewTargetPosition = true; // activate movement
      
	    doMovement(targetPosition,stepMode);
    }

    debug.println("MOVE:", targetPosition); 
    debug.println(stepMode); 
    
  }else if(mode == MODE.UP){
    int steps=getStepNumberFromBytes(message[1],message[2]);
    
   
    Serial.println(message[1]);
    Serial.println(message[2]);

    
    handleMove("UP",steps);
    
    debug.println("UP", steps); 
    //Serial.println("UP");
    //Serial.println(steps);
  
  }else if(mode == MODE.DOWN){
    int steps=400;//message[1];
    handleMove("DOWN",steps);
     
    debug.println("DOWN", steps); 
  
  }else if(mode == MODE.INIT){
    String initMode= getInitMode(message[1]);
     debug.println("Initialization:");

   
    if(initMode=="CALIBRATE"){      
      ctl.calibrate();
      debug.print("... OK");
      debug.println("calibrate");
      
    }else{ /* TOP or BOTTOM initialization*/
      int positionSteps=message[2];

      if(initMode=="TOP"){
        //init top position
        if(isValidStepNumber(positionSteps)){
         //ctl.setMaxPosition(positionSteps);
         debug.println("INIT:TOP/", positionSteps);  
        }else{
          debug.println("No valid step number"); 
        }
         
      }else if(initMode=="BOTTOM"){
        //init bottom position
         if(isValidStepNumber(positionSteps)){
          //ctl.setMinPosition(positionSteps);
          debug.println("INIT:BOTTOM/", positionSteps);  
         }else{
          debug.println("No valid step number"); 
         }
      }
    }
    
  }else if(mode == MODE.ANI){
    String animationMode= getAnimationMode(message[1]);
    String color=getColorHexValue(message[3],message[4],message[5]);
    int brightness=message[6];
    int speedAnimation=message[7];

	
	  int rgba[4] = { 0, 0, 0, -1};
	
	// Convert HEX to RGB
    rgba[0] = hexToDec(color.substring(0,2));
    rgba[1] = hexToDec(color.substring(2,4));
    rgba[2] = hexToDec(color.substring(4,6));
    rgba[3] = brightness;
    debug.println("R:", rgba[0]);
    debug.print("G:", rgba[1]);
    debug.print("B:", rgba[2]);
    debug.print("Brightness:", rgba[3]); 
	 
	//Update Animation
    //setAnimation(animationMode, rgba, speedAnimation);
    
    Serial.println("ANI");
    Serial.println(animationMode);
    Serial.println(color);
    Serial.println(brightness);
    Serial.println(speedAnimation);
    
  }else if(mode == MODE.LIGHT){  
    int field= 90;
    String side=getSide(getValueBitMask(field,7,6));
    String operation=getOperation(getValueBitMask(field,5,4));
    int led=getValueBitMask(message[1],3,0);
    String color=getColorHexValue(message[2],message[3],message[4]);
    int brightness=message[5];

     int rgba[4] = { 0, 0, 0, -1};
	
     // Convert HEX to RGB
    if ( operation != "-" ) {
      // Convert HEX to RGB
		rgba[0] = hexToDec(color.substring(0,2));
		rgba[1] = hexToDec(color.substring(2,4));
		rgba[2] = hexToDec(color.substring(4,6));
		rgba[3] = brightness;
		debug.println("R:", rgba[0]);
		debug.print("G:", rgba[1]);
		debug.print("B:", rgba[2]);
		debug.print("Brightness:", rgba[3]); 
    }

    /*rgba[0] = 10;
    rgba[1] = 255;
    rgba[2] = 50;
    rgba[3] = 100;*/
	
    
	// Update Lighting Pattern
    setLighting(side, operation, led, rgba);
    
    Serial.println("LIGHT");
    Serial.println(field);
    Serial.println(side);
    Serial.println(operation);
    Serial.println(led);
    Serial.println(color);
    Serial.println(brightness);
    
  }else if(mode == MODE.LEVEL){

    int ledLevel=message[1];
    String color=getColorHexValue(message[2],message[3],message[4]);
    int brightness=message[5];
	
	int rgba[4] = { 0, 0, 0, -1};
  
	// Convert HEX to RGB
    rgba[0] = hexToDec(color.substring(0,2));
    rgba[1] = hexToDec(color.substring(2,4));
    rgba[2] = hexToDec(color.substring(4,6));
    rgba[3] = brightness;
    debug.println("R:", rgba[0]);
    debug.print("G:", rgba[1]);
    debug.print("B:", rgba[2]);
    debug.print("Brightness:", rgba[3]); 
      
  
    // Update Lighting Pattern
    setLighting("A","+",ledLevel,rgba);
    setLighting("B","+",ledLevel,rgba);
    setLighting("C","+",ledLevel,rgba);
    setLighting("D","+",ledLevel,rgba);
    
    Serial.println("LEVEL");
    Serial.println(ledLevel);
    Serial.println(color);
    Serial.println(brightness);
  }

  memset(message,0,MESSAGE_LENGTH);
  
}

void handleMove(String mode,int steps){
  ctl.setStepMode(HALF_STEP);
	
    if(isValidStepNumber(steps)){  
      if(mode=="UP"){
		    //ctl.up();
        ctl.moveUp(steps);
      }else if(mode=="DOWN"){
        ctl.moveDown(steps);
      }
    }else{
      debug.print("[Default Value: 400 Steps]");
      if(mode=="UP"){
		    //ctl.up();
        ctl.moveUp(400);
      }else if(mode=="DOWN"){
        ctl.moveDown(400);
      }
    }
	
    
    debug.println("Current Position: ", ctl.getPosition() );
}

void setLighting(String side, String operation, int num, int rgba[4]) {
  // Scale and set brightness
  if (rgba[3] >= 0){
    int brightness = getScaledBrightness(rgba[3]);
    led.setBrightness(AlaColor(brightness, brightness, brightness)); // TODO
  }
  
  if (operation == "+"){
    led.setLED(side, num, ALA_ON, AlaColor(rgba[0], rgba[1], rgba[2]));
  }else if(operation == "-"){
    led.setLED(side, num, ALA_OFF, AlaColor(0, 0, 0));
  }else if (operation == "*"){
    led.setSide(side, ALA_OFF, AlaColor(0, 0, 0));
    //led.setLED(side, num, ALA_ON, AlaColor(rgba[0], rgba[1], rgba[2]));
  }

  led.setCustomLighting(true);
}

void setAnimation(String animation, int rgba[4], int spd) {

  // Scale and set brightness
  int brightness = getScaledBrightness(rgba[3]);
  led.setBrightness(AlaColor(brightness, brightness, brightness));

  // Special duration-speed mapping for each animation
  int duration;
  
  if (animation == "OFF") {
    
    led.setAnimation(ALA_OFF, 0, AlaColor(0, 0, 0));
     
  } else if (animation == "on") {
    
    led.setAnimation(ALA_ON, 0, AlaColor(rgba[0], rgba[1], rgba[2]));
     
  } else if (animation == "GLOW") {
  
    duration = map(spd, 0, 100, 20000, 1000);
    led.setAnimation(ALA_GLOW, duration, AlaColor(rgba[0], rgba[1], rgba[2]));
     
  } else if (animation == "BLINK") {
  
    duration = map(spd, 0, 100, 4000, 200);
    led.setAnimation(ALA_BLINK, duration, AlaColor(rgba[0], rgba[1], rgba[2]));
     
  } else if (animation == "UP") {
  
    duration = map(spd, 0, 100, 10000, 800);
    led.setAnimation(ALA_PIXELSMOOTHSHIFTRIGHT, duration, AlaColor(rgba[0], rgba[1], rgba[2]));
     
  } else if (animation == "DOWN") {
    
    duration = map(spd, 0, 100, 10000, 800);
    led.setAnimation(ALA_PIXELSMOOTHSHIFTLEFT, duration, AlaColor(rgba[0], rgba[1], rgba[2]));
    
  } else if (animation == "COM") {
    
    duration = map(spd, 0, 100, 10000, 800);
    led.setAnimation(ALA_COMET, duration, AlaColor(rgba[0], rgba[1], rgba[2]));
    
  } else if (animation == "BOUNCE") {
    
    duration = map(spd, 0, 100, 10000, 800);
    led.setAnimation(ALA_PIXELSMOOTHBOUNCE, duration, AlaColor(rgba[0], rgba[1], rgba[2]));
     
  } else if (animation == "MOVE") {
  
    duration = map(spd, 0, 100, 10000, 800);
    led.setAnimation(ALA_LARSONSCANNER, duration, AlaColor(rgba[0], rgba[1], rgba[2]));
  }

  debug.println("//// -------------------- ////");
  debug.println("Animation: ", animation);
  debug.println("Color: ", rgba[0]);
    debug.print(",", rgba[1]);
    debug.print(",", rgba[2]);
  debug.println("Brightness: ", rgba[3]);
  debug.println("Speed: ", spd);
  debug.println("Duration: ", duration);
  debug.println("//// -------------------- ////");

  led.setCustomLighting(false);
}

//b1=MSB and b3=LSB
String getColorHexValue(byte b1, byte b2, byte b3){
  uint32_t sum= 0;
  int lengthColorHex=6;

  sum |= (uint32_t)b3<<0;
  sum |= (uint32_t)b2<<8;
  sum |= (uint32_t)b1<<16;
  
  String colorValue=String(sum,HEX);
  int length = colorValue.length();

  for(int i=0; i<(lengthColorHex-length);i++){
    colorValue= "0"+colorValue;
  }
  
return colorValue;
}


//Bit Positions: 7,6,5,4,3,2,1,0
uint8_t getValueBitMask(uint8_t value, int toMSB, int fromLSB){ 
  uint8_t bitMask = 0;
  
  for(int i=fromLSB; i<toMSB+1;i++){
    bitSet(bitMask,i);
  }
 
  //Serial.println(bitMask,BIN);
  value&= bitMask;
  value>>= fromLSB;
  //Serial.println(value);
  return value;
  
}


int getStepNumberFromBytes(uint8_t MSB, uint8_t LSB){
	int result=LSB + (MSB<<8);
	
	return result;
}

// Convert color HEX #FF to RGB 255
unsigned int hexToDec(String hexString) {
  
  unsigned int decValue = 0;
  int nextInt;
  
  for (int i = 0; i < hexString.length(); i++) {
    
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    
    decValue = (decValue * 16) + nextInt;
  }
  
  return decValue;
}

void setRGBA(int rgba[4],String color,int brightness){
	// Convert HEX to RGB
    rgba[0] = hexToDec(color.substring(0,2));
    rgba[1] = hexToDec(color.substring(2,4));
    rgba[2] = hexToDec(color.substring(4,6));
    rgba[3] = brightness;
    debug.println("R:", rgba[0]);
    debug.print("G:", rgba[1]);
    debug.print("B:", rgba[2]);
    debug.print("Brightness:", rgba[3]); 

}

void doMovement(unsigned int targetPosition, String stepMode) {
  
    ctl.enable();
  
    if (stepMode == "FULL") {    
      ctl.moveTo( targetPosition, SPEED.FULL);    
    } else if (stepMode == "HALF") {
      ctl.moveTo( targetPosition, SPEED.HALF);
    } else if (stepMode == "QUARTER") {
      ctl.moveTo( targetPosition, SPEED.QUARTER);
    } else if (stepMode == "EIGTHTH") {
      ctl.moveTo( targetPosition, SPEED.EIGTHTH);
    }

    ctl.disable();

    _hasNewTargetPosition = false;
  
}


String getStepMode(int speedNumber){
  if(speedNumber == SPEED.FULL){
    return "FULL";
  }else if(speedNumber == SPEED.HALF){
    return "HALF";
  }else if(speedNumber == SPEED.QUARTER){
    return "QUARTER";
  }else if(speedNumber == SPEED.EIGTHTH){
    return "EIGTHTH";
  }
  return "INVALID";
}


String getInitMode(int initModeNumber){
  if(initModeNumber == INIT.TOP){
    return "TOP";
  }else if(initModeNumber == INIT.BOTTOM){
    return "BOTTOM";
  }else if(initModeNumber == INIT.CALIBRATE){
    return "CALIBRATE";
  }
  return "INVALID";
}


String getAnimationMode(int animationNumber){
  if(animationNumber == ANIMATION.BLINK){
    return "BLINK";
  }else if(animationNumber == ANIMATION.GLOW){
    return "GLOW";
  }
  return "INVALID";
}

String getSide(int sideNumber){
  if(sideNumber == SIDE.A){
    return "A";
  }else if(sideNumber == SIDE.B){
    return "B";
  }else if(sideNumber == SIDE.C){
    return "C";
  }else if(sideNumber == SIDE.D){
    return "D";
  }
  
  return "INVALID";
}

String getOperation(int operationNumber){
  if(operationNumber == LIGHTOPERATIONS.ADD){
    return "+";
  }else if(operationNumber == LIGHTOPERATIONS.REMOVE){
    return "-";
  }else if(operationNumber == LIGHTOPERATIONS.CLEARSIDE){
    return "*";
  }
  
  return "INVALID";
}

int getScaledBrightness(int percent){
  return map(percent,0,100,0,255);
}

boolean isValidPosition(int positionValue){
  return (positionValue>=0 & positionValue <=100);
}

boolean isValidStepNumber(int stepNumber){
   return(stepNumber>=0 & stepNumber <= 16000);
} 





