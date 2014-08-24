// Arduino Leonardo Interrupts
#define int3 3//Interrupt 3 attached to digital pin 1
#define int2 2//Interrupt 2 attached to digital pin 0
#define int1 1//Interrupt 1 attached to digital pin 2
#define int0 0//Interrupt 0 attached to digital pin 3
int led=9;
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
volatile int state=LOW;
volatile boolean prenderFoco;
volatile long lastDebounceTime = 0;   // the last time the interrupt was triggered
#define debounceDelay 300    // the debounce time in ms; decrease if quick button presses are ignored, increase

void setup() {
  Serial.begin(9600);  // serial comms for troubleshooting (always)
  // put your setup code here, to run once:
 // put your setup code here, to run once:
// Leonardo	3	2	0
// Leonardo	int0	int1	int2
  //attachInterrupt(2,stateChange, LOW);  // funca con pin 0
 //  attachInterrupt(1,stateChange, LOW); //funca en pin 2
//   attachInterrupt(0,stateChange, LOW); // funca en pin 3
//    attachInterrupt(3,stateChange, LOW); // funca en pin 1
//    attachInterrupt(int3,stateChange, LOW);// funca en pin 1
  attachInterrupt(int2,stateChange, LOW);  // funca con pin 0


  pinMode(led, OUTPUT);
  prenderFoco=false;
}
void stateChange()
 {
  state = !state;
  digitalWrite(led, state); 
  }

void alarmaActivada() //triggers when the switch is pressed. each time you press the switch,
{                //there are hundreds of little tiny conenctions as you press it down, so the delay filters those out.

  if ((millis() - lastDebounceTime) > debounceDelay) //if current time minus the last trigger time is greater than
  {                                                  //the delay (debounce) time, button is completley closed.
    lastDebounceTime = millis();
    //scanear sensores 1,2 y 3 
    prenderFoco=true;
    cli();
    Serial.print("prenderFoco: "); Serial.println(prenderFoco);
    sei();
   }
}
void loop() {
  noInterrupts();
  // critical, time-sensitive code here
  interrupts();
  // put your main code here, to run repeatedly: 
   // set the brightness of pin 9:
  if (prenderFoco)
  {
  analogWrite(led, brightness);    

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade: 
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ; 
  }     
  // wait for 30 milliseconds to see the dimming effect    
  delay(30);
  }
  
}
