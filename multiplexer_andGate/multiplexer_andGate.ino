#define DEBUG 1

#ifdef DEBUG
  #define DEBUG_PRINT(x)        Serial.print (x)
  #define DEBUG_PRINTDEC(x)     Serial.print (x, DEC)
  #define DEBUG_PRINTLN(x)      Serial.println (x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTDEC(x)
  #define DEBUG_PRINTLN(x) 
#endif


int selPin[] = { 3, 4, 5 }; // select pins on 4051 (analog A0, A1, A2)
byte estadoSensor=0;
int muxSignal = 9;           // arduino pin attached to multiplexer output/input pin
int pin13=13;


volatile byte estadoSensores=0;
volatile long lastDebounceTime = 0;   // the last time the interrupt was triggered
#define debounceDelay 300    // the debounce time in ms; decrease if quick button presses are ignored, increase

                             //if you get noise (multipule button clicks detected by the code when you only pressed it once)
// Leonardo pin                  	3	2	0	1	7
// nro de interrupcion                int0    int1    int2    int3    int4
//enum nroInterrupcion {int2,int3,int1,int0} 
#define int1 1//digital pin 2, interrupt 1



void setup(){
  Serial.begin(9600);  // serial comms for troubleshooting (always)
  // declare signal multiplexer as output:
  pinMode(muxSignal, INPUT);
  pinMode(pin13, OUTPUT);
  
  // setting multiplexer control pins
  for(int pin = 0; pin < 3; pin++){ // setup select pins
    pinMode(selPin[pin], OUTPUT);
  } 
  
  // put your setup code here, to run once:
  attachInterrupt(int1,alarmaActivada, LOW);
  interrupts(); 
}

void alarmaActivada() //triggers when the switch is pressed. each time you press the switch,
{                //there are hundreds of little tiny conenctions as you press it down, so the delay filters those out.

  if ((millis() - lastDebounceTime) > debounceDelay) //if current time minus the last trigger time is greater than
  {                                                  //the delay (debounce) time, button is completley closed.
    lastDebounceTime = millis();
    //scanear sensores 1,2 y 3 
    scanSensor(1,3);
   }
}

//scanea sensores, partiendo desde 0 hasta 7

void scanSensor(int nroSensorIni, int nroSensorFin){
  // sensor 0 => 0000 0001
  // sensor 7 => 1000 0000
  estadoSensores=0;
  
  byte sensorActual=1; // setting scan  to sensor 0
  //if (nroSensorIni>0)    sensorActual=sensorActual <<nroSensorIni;
  // for (int i=0 ; i<nroSensorIni; i++) sensorActual=sensorActual <<i; // si el comienzo es mayor a 1
   
  for(byte pin = nroSensorIni; pin < nroSensorFin +1; pin++)
    { // setup select pins
    //  delayMicroseconds(10050); 
     sensorActual=1; // setting scan to sensor 0
     sensorActual=sensorActual<<pin;
     
    #ifdef DEBUG
      sei();
     #endif  
    
    // control de multiplexer 
    //enviar 1 o 0 depende del valor del bit en c/u de las posiciones
    //digitalWrite(  selPin[0],bitRead(pin,1));
    //digitalWrite(  selPin[1],bitRead(pin,2));
    //digitalWrite(  selPin[2],bitRead(pin,3));
    
     digitalWrite(selPin[0], pin & 1); 
     digitalWrite(selPin[1], pin & 2); 
     digitalWrite(selPin[2], pin & 4); 

 
    // salida selPin[2], selPin[1], selPin[0], que son A2,A1,A0
      DEBUG_PRINT("sensorActual=");DEBUG_PRINTLN(sensorActual);
      DEBUG_PRINT("sel Multiplexor=");DEBUG_PRINT((pin&4)? "1":"0");DEBUG_PRINT((pin&2)? "1":"0");DEBUG_PRINT((pin&1)? "1":"0");DEBUG_PRINTLN(); 
   
    if (digitalRead(muxSignal)==LOW)  
     {
        DEBUG_PRINT("Señal Activada ");
       estadoSensores= sensorActual | estadoSensores; //almacena el estado del sensor actual byte sensores
         digitalWrite(pin13,HIGH);
      }
   
    
    //Imprimir estasensores
    DEBUG_PRINT("estadosensores=");DEBUG_PRINT((estadoSensores&128)? "1":"0");DEBUG_PRINT((estadoSensores&64)? "1":"0");DEBUG_PRINT((estadoSensores&32)? "1":"0");
    DEBUG_PRINT((estadoSensores&16)? "1":"0");DEBUG_PRINT((estadoSensores&8)? "1":"0");DEBUG_PRINT((estadoSensores&4)? "1":"0");DEBUG_PRINT((estadoSensores&2)? "1":"0");
    DEBUG_PRINT((estadoSensores&1)? "1":"0");DEBUG_PRINTLN(); 
    #ifdef DEBUG
       cli();
    #endif 
  
    }

}  
void loop(){
 
 delay(3000);
 digitalWrite(pin13,LOW);
}

//Basurero

//digitalWrite(  selPin[0],bitRead(pin,0));
    //digitalWrite(  selPin[1],bitRead(pin,1));
    //digitalWrite(  selPin[2],bitRead(pin,2));
