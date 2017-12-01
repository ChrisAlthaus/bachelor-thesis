
#define VALUES_SIZE 4

boolean rgb_setup = false;
boolean led_setup = true;
int led_pin = 10;
int r_led = 9;
int g_led = 10;
int b_led = 11;

boolean initialize = false;

String message = "";

String command = "";
String values[VALUES_SIZE];
char seperator='/';  


void setup() {
  Serial.begin(9600);
  if(led_setup)pinMode(led_pin, OUTPUT);

  if(rgb_setup){
    pinMode(r_led, OUTPUT);
    pinMode(g_led, OUTPUT);
    pinMode(b_led, OUTPUT);
  }
  
 
}

void loop() {
  if(!initialize && Serial){
    Serial.println("initialized system");
    delay(1000);
  }
  
  
  while(Serial.available()){
    if(!initialize)initialize=true;
   
    message+= Serial.readString();
  }

  if(message.length()>0){
    parseMessage(message);
    //executeCommand();
   
    memset(values,0,sizeof(values));
  }

  message="";
 
 }
  
//SEQUENCE:FLASH/100/100//DIMMING/500/500//FLASH/200/700//FLASH/100/100//DIMMING/500/500//FLASH/200/700
//SEQUENCE:FLASH/1000/1000//FLASH/2000/2000//FLASH/700/700//FLASH/10000/10000
//NORMAL:Flash/100/100
//NORMAL:STAIRCASE/2000/LOOP

void parseMessage(String message){
   int message_length = message.length();
   if(!message_correct(message))return;
   
   int index_first_entry = message.indexOf(':',0);
   String mode_of_operation = message.substring(0,index_first_entry);


   if(mode_of_operation=="NORMAL"){
    Serial.println("NORMAL");
    parseSegment(message.substring(index_first_entry +1, message_length));
    //print_values();
    executeCommand();
    
   }else if(mode_of_operation=="SEQUENCE"){
    Serial.println("SEQUENCE");
     int index_next_command;
     Serial.println("message_length=" + message_length);
      for(int i = index_first_entry +1; i< message_length;i++){
       
        //Serial.println("i=" + i);
        index_next_command = message.indexOf("//",i);
      
        Serial.println(index_next_command);
        if(index_next_command==-1)index_next_command = message_length; 
        
        
        String segment = message.substring(i,index_next_command);
        //Serial.println("segment="+ segment);
        parseSegment(segment);
        executeCommand();
        //print_values();

        //resetBuffer();
        if(index_next_command<0)break;
        i=index_next_command +1;
      }
   }

   //resetBuffer();
    //print_values();
}

void parseSegment(String segment){
  int segment_length = segment.length();
  int counter_fields = 0;
  
    for(int i = 0; i<segment_length ;i++){
      int index = segment.indexOf(seperator, i);
      if(index==-1){
        values[counter_fields++] = segment.substring(i,segment_length);
        break;
        }
      
      values[counter_fields++] = segment.substring(i,index);
      i=index;
    }
    command = values[0];
}

boolean message_correct(String message){
   message.trim();
   if(message.length()==0){
    Serial.println("Unvalid Command");
    return false;
   }
   if(number_fields_of_message(message)<2){
    Serial.println("Unvalid Command");
    return false;
   }

   return true;
}

void executeCommand(){
    print_values();
    if(command=="FLASH"){
      if(values[1]=="COLOR"){
        if(values[2]=="LOOP"){
          flash_color_led(values[3].toInt(),values[4].toInt(),values[5].toInt(),values[6].toInt(),values[7].toInt(), true);
        }else{
          flash_color_led(values[3].toInt(),values[4].toInt(),values[5].toInt(),values[6].toInt(),values[7].toInt(), false);
        }
      }else{
        if(values[1]=="LOOP"){
          flash_led(values[2].toInt(),values[3].toInt(),true);
        }else{
          flash_led(values[1].toInt(),values[2].toInt(),false);
        }
      }
    }else if(command=="DIMMING"){
       if(values[1]=="LOOP"){
          varying_brightness(values[2].toInt(),values[3].toInt(),true);
        }else{
          varying_brightness(values[1].toInt(),values[2].toInt(),false);
        }
    }else if(command=="STAIRCASE"){
       if(values[1]=="LOOP"){
          staircase_continuous(values[2].toInt(),true);
        }else{
          staircase_continuous(values[1].toInt(),false);
        }
    }else if(command=="BEACON"){
       if(values[1]=="LOOP"){
          beacon(values[2].toInt(),true);
        }else{
          beacon(values[1].toInt(),false);
        }
    }else if(command=="PULSESLOW"){
      if(values[1]=="LOOP"){
          pulse_slow(values[2].toInt(),true);
        }else{
          pulse_slow(values[1].toInt(),false);
        }
    }
}


void resetBuffer(){
  int values_length = VALUES_SIZE;
  for(int i =0; i<values_length;i++){
    values[i]="";
  }

  command="";
}

void print_values(){
  Serial.println("command="+ command);
  for(int i=0; i< VALUES_SIZE; i++){
    Serial.println("values=" + values[i]);
  }
}


int number_fields_of_message(String message){
  int message_length = message.length();
  int number = 1;
  for(int i=0 ; i<message_length; i++){
    if(message[i]==seperator)number++; 
  }

  return number;
}



void flash_led(long duration_on, long duration_off, boolean loop){
  if(loop){
    while(!Serial.available()){
      digitalWrite(led_pin, HIGH);
      delay(duration_on);
      digitalWrite(led_pin, LOW);
      delay(duration_off);
    }
  }else{
     digitalWrite(led_pin, HIGH);
      delay(duration_on);
      digitalWrite(led_pin, LOW);
      delay(duration_off);
  }
}

void flash_color_led(long duration_on, long duration_off, int red, int green , int blue, boolean loop){
  if(loop){
    while(!Serial.available()){
      digitalWrite(led_pin, HIGH);
      delay(duration_on);
      digitalWrite(led_pin, LOW);
      delay(duration_off);
    }
  }else{
     digitalWrite(led_pin, HIGH);
      delay(duration_on);
      digitalWrite(led_pin, LOW);
      delay(duration_off);
  }
}

void varying_brightness(long duration_on, long duration_off, boolean loop){
   int delay_on=duration_on/255;
   if(loop){
    while(!Serial.available()){
      for(int i=0; i<=255; i++){
        analogWrite(led_pin,i);
        delay(delay_on);
      }
      for(int j=255; j>=0; j--){
        analogWrite(led_pin,j);
        delay(delay_on);
      }
      delay(duration_off);
    }
  }else{
     for(int i=0; i<=255; i++){
        analogWrite(led_pin,i);
        delay(delay_on);
      }
      for(int j=255; j>=0; j--){
        analogWrite(led_pin,j);
        delay(delay_on);
      }
      delay(duration_off);
  }
}

//NORMAL:STAIRCASE/LOOP/2000
void staircase_continuous(int duration_one_cycle, boolean loop){
   int number_steps= 4;
   int step_size= 255/4;
   int duration_delay= duration_one_cycle/(number_steps + 2);

   Serial.println(duration_one_cycle);

   if(loop){
    while(!Serial.available()){
      for(int i=1 ; i<=number_steps; i++){
        analogWrite(led_pin,i*step_size);
        delay(duration_delay);
      }
   
      analogWrite(led_pin,0);
      delay(2*duration_delay);
    }
   }else{
     for(int i=1 ; i<=number_steps; i++){
      analogWrite(led_pin,i*step_size);
      delay(duration_delay);
     }
     
     analogWrite(led_pin,0);
     delay(2*duration_delay);
   }
}

void beacon(int duration_one_cycle, boolean loop){
  int duration_delay= duration_one_cycle/16;
  int pause_delay = 2*duration_delay;
  int end_delay= 6*duration_delay;

  while(!Serial.available()){
     flash(duration_delay,duration_delay);
    flash(duration_delay,duration_delay);
  
    delay(pause_delay);
    
    flash(duration_delay,duration_delay);
    flash(duration_delay,duration_delay);
  
    delay(end_delay);
    if(!loop)break;
  }
}

void pulse_slow(int duration_one_cycle,boolean loop){
  int sample_numbers=360;
  double step_size=360/sample_numbers;
  int duration_delay=duration_one_cycle/sample_numbers;

  while(!Serial.available()){
    for(int i=0; i<sample_numbers;i++){
      int value= ((sin(radians(i))+1)/2)*255;
      analogWrite(led_pin,value);
      //Serial.println(i);
      //Serial.println((sin(radians(i))));
      delay(duration_delay);
    }
    if(!loop)break;
  }
  
}

void flash(int duration_on, int duration_off){
  digitalWrite(led_pin,HIGH);
  delay(duration_on);
  digitalWrite(led_pin,LOW);
  delay(duration_off);
}

