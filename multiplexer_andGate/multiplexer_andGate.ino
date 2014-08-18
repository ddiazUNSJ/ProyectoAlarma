int selPin[] = { 3, 4, 5 }; // select pins on 4051 (analog A0, A1, A2)
byte estadoSensor=0;
int muxSignal = 9;           // the pin that the LED is attached to
int brightness = 255;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
byte counter=0;



volatile long lastDebounceTime = 0;   // the last time the interrupt was triggered
#define debounceDelay 50    // the debounce time in ms; decrease if quick button presses are ignored, increase
                             //if you get noise (multipule button clicks detected by the code when you only pressed it once)
// Leonardo pin                  	3	2	0	1	7
// nro de interrupcion                int0    int1    int2    int3    int4
//enum nroInterrupcion {int2,int3,int1,int0} 
#define int2 2//digital pin 0, interrupt 2



void setup(){
  Serial.begin(9600);  // serial comms for troubleshooting (always)
  // declare signal multiplexer as output:
  pinMode(muxSignal, OUTPUT);
  // setting multiplexer control pins
  for(int pin = 0; pin < 3; pin++){ // setup select pins
    pinMode(selPin[pin], OUTPUT);
  } 
  
  // put your setup code here, to run once:
  attachInterrupt(int2,FireEvent, RISING);
}

void FireEvent() //triggers when the switch is pressed. each time you press the switch,
{                //there are hundreds of little tiny conenctions as you press it down, so the delay filters those out.

  if ((millis() - lastDebounceTime) > debounceDelay) //if current time minus the last trigger time is greater than
  {                                                  //the delay (debounce) time, button is completley closed.
    lastDebounceTime = millis();
   //switch was pressed, do stuff like turn off your motor/led etc
  }
}

//scanea sensores, la partir de 0 hasta 7

byte scanSensor(byte nroSensorIni, byte nroSensorFin){
  // sensor 0 => 0000 0001
  // sensor 7 => 1000 0000
  
  byte sensorIni=1;
  byte sensorActual=0;
  byte estadoSensores=0;
  if (nroSensorIni>0) 
   for (int i=0 ; i<nroSensorIni; i++) sensorIni <<0; // si el comienzo es mayor a 1
  sensorActual =sensorIni;
  for(byte pin = nroSensorIni; pin < nroSensorFin +1; pin++)
    { // setup select pins
    
    //digitalWrite(  selPin[0],bitRead(pin,0));
    //digitalWrite(  selPin[1],bitRead(pin,1));
    //digitalWrite(  selPin[2],bitRead(pin,2));
    //enviar 1 o 0 depende del valor del bit en c/u de las posiciones
     digitalWrite(selPin[0], pin & 1); 
     digitalWrite(selPin[1], pin & 2); 
     digitalWrite(selPin[2], pin & 4); 

    // salida selPin[2], selPin[1], selPin[0], que son A2,A1,A0
    Serial.print("salida=");Serial.print((pin&4)? "1":"0");Serial.print((pin&2)? "1":"0");Serial.print((pin&1)? "1":"0");Serial.println(); 
   
    if (digitalRead(muxSignal)==HIGH)  sensorActual | estadoSensores; //almacena el estado del sensor actual byte sensores
    sensorActual <<0;
    //Imprimir estado sensores
    Serial.print("estadosensores=");Serial.print((estadoSensores&128)? "1":"0");Serial.print((estadoSensores&64)? "1":"0");Serial.print((estadoSensores&32)? "1":"0");
    Serial.print((estadoSensores&16)? "1":"0");Serial.print((estadoSensores&8)? "1":"0");Serial.print((estadoSensores&4)? "1":"0");Serial.print((estadoSensores&2)? "1":"0");
    Serial.print((estadoSensores&1)? "1":"0");Serial.println(); 
  
    }
  return estadoSensores;
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


