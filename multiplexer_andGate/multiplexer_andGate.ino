int selPin[] = { 2, 3, 4 }; // select pins on 4051 (analog A0, A1, A2)
int muxSignal = 9;           // the pin that the LED is attached to
int brightness = 255;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
byte counter=0;



void setup(){
  Serial.begin(9600);  // serial comms for troubleshooting (always)
  // declare signal multiplexer as output:
  pinMode(muxSignal, OUTPUT);
  // setting multiplexer control pins
  for(int pin = 0; pin < 3; pin++){ // setup select pins
    pinMode(selPin[pin], OUTPUT);
  } 
}


void loop(){
 
  //scanear los pines de 1 a 5
  for(byte pin = 4; pin < 5; pin++){ // setup select pins
   
    //digitalWrite(  selPin[0],bitRead(pin,0));
    //digitalWrite(  selPin[1],bitRead(pin,1));
    //digitalWrite(  selPin[2],bitRead(pin,2));
    
     digitalWrite(selPin[0], pin & 1); 
     digitalWrite(selPin[1], pin & 2); 
     digitalWrite(selPin[2], pin & 4); 

// salida selPin[2], selPin[1], selPin[0], que son A2,A1,A0
    Serial.print("salida=");Serial.print((pin&4)? "1":"0");Serial.print((pin&2)? "1":"0");Serial.print((pin&1)? "1":"0");Serial.println(); 
  analogWrite(muxSignal, brightness);
   // change the brightness for next time through the loop:
//  brightness = brightness + fadeAmount;
//
//  // reverse the direction of the fading at the ends of the fade: 
//  if (brightness == 0 || brightness == 255) {
//    fadeAmount = -fadeAmount ; 

  // wait for 30 milliseconds to see the dimming effect    
//  delay(30);  
 delay(100);
}
}


