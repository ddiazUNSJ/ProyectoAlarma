#include <EEPROM.h>


/**  Example OMMenuMgr Sketch

 @author
 C. A. Church
 
 */
#include "OMEEPROM.h"
#include "OMMenuMgr.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>  // Comes with Arduino IDE
#include <Keypad.h>
#include <Password.h>
#include <avr/io.h>
#include <avr/interrupt.h>
// #include <MenuBackend.h>
//#define DEBUG;
#define ARMAR;

// Debug password  keypad  
#ifdef ADEBUG
  #define ADEBUG_PRINT(x)     Serial.print (x)
  #define ADEBUG_PRINTDEC(x)     Serial.print (x, DEC)
  #define ADEBUG_PRINTLN(x)  Serial.println (x)
#else
  #define ADEBUG_PRINT(x)
  #define ADEBUG_PRINTDEC(x)
  #define ADEBUG_PRINTLN(x) 
#endif

//debug multipler y compuerta and
#ifdef MDEBUG
  #define MDEBUG_PRINT(x)        Serial.print (x)
  #define MDEBUG_PRINTDEC(x)     Serial.print (x, DEC)
  #define MDEBUG_PRINTLN(x)      Serial.println (x)
#else
  #define MDEBUG_PRINT(x)
  #define MDEBUG_PRINTDEC(x)
  #define MDEBUG_PRINTLN(x) 
#endif


/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

//------------------Menu variables-----------------------------------//
//-------------------------------------------------------------------//
 // button values
 // which input is our button
    const byte BUT_PIN = 14;
    
    //basado en  http://forum.arduino.cc/index.php/topic,8558.0.html
      // analog button read values
    const int BUTSEL_VAL  = 173;
    const int BUTFWD_VAL  = 320;
    const int BUTREV_VAL  = 733;
    const int BUTDEC_VAL  = 608;
    const int BUTINC_VAL  = 838;
    
    const byte BUT_THRESH  = 60;
    
      // mapping of analog button values for menu
    int BUT_MAP[5][2] = {
                             {BUTFWD_VAL, BUTTON_FORWARD}, 
                             {BUTINC_VAL, BUTTON_INCREASE}, 
                             {BUTDEC_VAL, BUTTON_DECREASE}, 
                             {BUTREV_VAL, BUTTON_BACK}, 
                             {BUTSEL_VAL, BUTTON_SELECT} 
                        };

/*
	//Este es el menu que implemente para hacer la prueba//
	
        Armar Sistema
                     On 
                     Off   
	Cambiar Clave
                  Clave
	Estado de Zonas
		Zona1
                     On (es Vigilar)
                     Off (es No Vigilar)                     
		Zona2
                     On
                     Off
	Sensores
		Sensor1
                      zona
                           zona1
                           zona2
                      tipo  
                           apertura
                           movimiento
                           humo
                      estado
                           On
                           Off
		Sensor2
                      zona Asignada
                           zona1
                           zona2
                      tipo  
                           apertura
                           movimiento
                           humo
                      estado
                           On
                           Off
*/


byte estadoAlarma = 0;
#ifdef ARMAR
const int pos_EEPROM_estadoAlarma       = 1;
//--SubMenu Armar Sistema
 MENU_FLAG    armar_flag    = { 3, &estadoAlarma };
 MENU_VALUE   armar_value   = { TYPE_BFLAG, 0, 0, MENU_TARGET(&armar_flag), pos_EEPROM_estadoAlarma };
 MENU_ITEM    item_armar    = { {"Armar Sistema"}, ITEM_VALUE, 0, MENU_TARGET(&armar_value) };
#endif

//--SubMenu Cambiar Password
int clave;
const int pos_EEPROM_clave       = 2;
MENU_VALUE clave_value = { TYPE_UINT,       9999, 1000,   MENU_TARGET(&clave),pos_EEPROM_clave  };
MENU_ITEM item_clave    = { {"Cambiar Clave"}, ITEM_VALUE,  0, MENU_TARGET(&clave_value) };

//--SubMenu Estado de zonas
byte estadoZonas=0;
const int pos_EEPROM_estadoZonas       = 4;
 //zona1 
 MENU_FLAG    zona1_flag    = { 1, &estadoZonas };
 MENU_VALUE   zona1_value   = { TYPE_BFLAG, 0, 0, MENU_TARGET(&zona1_flag), pos_EEPROM_estadoZonas  };
 MENU_ITEM    item_zona1    = { {"Zona1"}, ITEM_VALUE, 0, MENU_TARGET(&zona1_value) };
 //zona2 
 MENU_FLAG    zona2_flag    = { 2, &estadoZonas };
 MENU_VALUE   zona2_value   = { TYPE_BFLAG, 0, 0, MENU_TARGET(&zona2_flag), pos_EEPROM_estadoZonas  };
 MENU_ITEM    item_zona2    = { {"Zona2"}, ITEM_VALUE, 0, MENU_TARGET(&zona2_value)  };
 
 //Armando subMenus Estado de Zonas
 MENU_LIST zonas_list[]   = {  &item_zona1, &item_zona2 };
 MENU_ITEM menu_estado_zonas     = { {"Estado Zonas"}, ITEM_MENU,   MENU_SIZE(zonas_list),    MENU_TARGET(&zonas_list) };

//--subMenu Sensores
        //------Zona Asignada
        byte zonaAsigS1;
        const int pos_EEPROM_zonaAsigS1       = 5;
        byte zonaAsigS2;
        const int pos_EEPROM_zonaAsigS2       = 6;
        // Lista de zonas posibles
        MENU_SELECT_ITEM  opc_zona1 = { 1, {"Zona1"} };
        MENU_SELECT_ITEM  opc_zona2  = { 2, {"Zona2"} };
        
        MENU_SELECT_LIST  tipos_de_zonas[] = { &opc_zona1, &opc_zona2 };
                                          
        MENU_SELECT zona_S1_sel = { &zonaAsigS1,  MENU_SELECT_SIZE(tipos_de_zonas),   MENU_TARGET(&tipos_de_zonas) };
        MENU_VALUE zona_S1_value = { TYPE_SELECT,     0,     0,     MENU_TARGET(&zona_S1_sel),pos_EEPROM_zonaAsigS1  };
        MENU_ITEM item_zona_S1    = { {"Zona Sensor 1"}, ITEM_VALUE,  0,        MENU_TARGET(&zona_S1_value) };

        MENU_SELECT zona_S2_sel = { &zonaAsigS2,  MENU_SELECT_SIZE(tipos_de_zonas),   MENU_TARGET(&tipos_de_zonas) };
        MENU_VALUE zona_S2_value = { TYPE_SELECT,     0,     0,     MENU_TARGET(&zona_S2_sel), pos_EEPROM_zonaAsigS2  };
        MENU_ITEM item_zona_S2    = { {"Zona Sensor 2"}, ITEM_VALUE,  0,        MENU_TARGET(&zona_S2_value) };
       
        
        
        //------Tipos de sensores
        byte tipoS1;
        const int pos_EEPROM_tipoS1       = 7;
  
        byte tipoS2;
        const int pos_EEPROM_tipoS2       = 8;
  
        // Lista de tipos de sensores posibles
        MENU_SELECT_ITEM  opc_aper = { 2, {"Apertura"} };
        MENU_SELECT_ITEM  opc_mov  = { 1, {"Movimiento"} };
        MENU_SELECT_ITEM  opc_humo= { 0, {"Humo"} };
        
        MENU_SELECT_LIST  tipos_de_sensores[] = { &opc_aper, &opc_mov, &opc_humo };
                                          
        MENU_SELECT tipo_S1_sel = { &tipoS1,  MENU_SELECT_SIZE(tipos_de_sensores),   MENU_TARGET(&tipos_de_sensores) };
        MENU_VALUE tipo_S1_value = { TYPE_SELECT,     0,     0,     MENU_TARGET(&tipo_S1_sel), pos_EEPROM_tipoS1 };
        MENU_ITEM item_tipo_S1    = { {"Tipo Sensor 1"}, ITEM_VALUE,  0,        MENU_TARGET(&tipo_S1_value) };

        
        MENU_SELECT tipo_S2_sel = { &tipoS2,  MENU_SELECT_SIZE(tipos_de_sensores),   MENU_TARGET(&tipos_de_sensores) };
        MENU_VALUE tipo_S2_value = { TYPE_SELECT,     0,     0,     MENU_TARGET(&tipo_S2_sel), pos_EEPROM_tipoS2 };
        MENU_ITEM item_tipo_S2    = { {"Tipo Sensor 2"}, ITEM_VALUE,  0,        MENU_TARGET(&tipo_S2_value) };

       //-----Estados
       byte estadoSensores=0;
       const int pos_EEPROM_estadoSensores     = 9;
  
       //Estado Sensor 1 
       MENU_FLAG    estado_S1_flag    = { 1, &estadoSensores };
       MENU_VALUE   estado_S1_value   = { TYPE_BFLAG, 0, 0, MENU_TARGET(&estado_S1_flag), pos_EEPROM_estadoSensores };
       MENU_ITEM    item_estado_S1    = { {"estado Sensor 1"}, ITEM_VALUE, 0, MENU_TARGET(&estado_S1_value) };
      
      //Estado Sensor 2 
       MENU_FLAG    estado_S2_flag    = { 2, &estadoSensores };
       MENU_VALUE   estado_S2_value   = { TYPE_BFLAG, 0, 0, MENU_TARGET(&estado_S2_flag), pos_EEPROM_estadoSensores };
       MENU_ITEM    item_estado_S2    = { {"estado Sensor 2"}, ITEM_VALUE, 0, MENU_TARGET(&estado_S2_value) };


      //-----Arma submenus sensores
      //                Sensores
      //                   Sensor1
      //                   Sensor2
       MENU_LIST sensor1_list[]   = {  &item_zona_S1, &item_tipo_S1, &item_estado_S1 };
       MENU_ITEM item_sensor1     = { {"Sensor 1"}, ITEM_MENU,   MENU_SIZE(sensor1_list),    MENU_TARGET(&sensor1_list) };
      
       MENU_LIST sensor2_list[]   = {  &item_zona_S2, &item_tipo_S2, &item_estado_S2 };
       MENU_ITEM item_sensor2     = { {"Sensor 2"}, ITEM_MENU,   MENU_SIZE(sensor2_list),    MENU_TARGET(&sensor2_list) };
      
       MENU_LIST sensores_list[]   = {  &item_sensor1, &item_sensor2 };
       MENU_ITEM menu_sensores     = { {"Sensores"}, ITEM_MENU,   MENU_SIZE(sensores_list),    MENU_TARGET(&sensores_list) };

//--Menu Principal

 #ifdef ARMAR
    MENU_LIST root_list[]   = {  &item_armar, &item_clave, &menu_estado_zonas, &menu_sensores};
 #else
    MENU_LIST root_list[]   = {   &item_clave, &menu_estado_zonas, &menu_sensores};
 #endif

MENU_ITEM menu_root     = { {"Root"},        ITEM_MENU,   MENU_SIZE(root_list),    MENU_TARGET(&root_list) };



OMMenuMgr Menu(&menu_root);

//------------------Menu  Functions-----------------------------------//  
//      void testAction() {
//      
//       digitalWrite(5, HIGH);  
//      }
//      
      
      void uiDraw(char* p_text, int p_row, int p_col, int len) {
        lcd.setCursor(p_col, p_row);
        
        for( int i = 0; i < len; i++ ) {
          if( p_text[i] < '!' || p_text[i] > '~' )
            lcd.write(' ');
          else  
            lcd.write(p_text[i]);
        }
      }
      
      
      void uiClear() {
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter for Menu");
      }
      
      
      //void uiQwkScreen() {
      //  lcd.clear();
      //  Menu.enable(false);
      //  
      //  lcd.print("Action!");
      //  lcd.setCursor(0, 1);
      //  lcd.print("Enter 2 return");
      //  
      //  while( Menu.checkInput() != BUTTON_SELECT ) {
      //    ; // wait!
      //  }
      //  
      //  Menu.enable(true);
      //  lcd.clear();
      //}  

 
//------------------Activeted Alarm Variables-(Multiplexer  and gate) ---------------------------//
//----------------------------------------------------------------------------------------------//      


int selPin[] = { 3, 4, 5 }; // select pins on 4051 (analog A0, A1, A2)
int muxSignal = 9;           // arduino pin attached to multiplexer output/input pin
volatile byte estadoSensores=0;
volatile long lastDebounceTime = 0;   // the last time the interrupt was triggered
#define debounceDelay 300    // the debounce time in ms; decrease if quick button presses are ignored, increase
#define int1 1//digital pin 2, interrupt 1 , on leonardo config.
int pin13=13;
volatile int tSensorDisparado;
volatile int tLanzarSirena;
volatile boolean temporizarMenu;
//------------------Activeted Alarm Functions----------------------------//
//----------------------------------------------------------------------//      

//------ Interrupt Service Routine of activated alarm

void alarmaActivada() //triggers the alarm when an intruder is detected by any  sensor

  if ((millis() - lastDebounceTime) > debounceDelay) //delay debounce 
  {                                                  
    lastDebounceTime = millis();
    //scanear sensores 1,2 y 3 
    scanSensor(1,3);
    tLanzarSirena=0; // 2 minutos para desactivar alarma
    tSensorDisparado=0; // comenzando a contar a partir de ahora
    temporizarMenu=false; // dejar de temporizar menu 

   }
}
        
//------ sensors scan from 0 to 7
void scanSensor(int nroSensorIni, int nroSensorFin){
  // sensor 0 => 0000 0001
  // sensor 7 => 1000 0000
  estadoSensores=0;
  
  byte sensorActual=1; // setting scan  to sensor 0
    
  for(byte pin = nroSensorIni; pin < nroSensorFin +1; pin++)
    { // setup select pins
    //  delayMicroseconds(10050); 
     sensorActual=1; // setting scan to sensor 0 --> sensorActual=0000 0001
     sensorActual=sensorActual<<pin;// setting the first sensor to scan, example if nroSensorIni=4 then  sensorActual=0001 0000
     
     #ifdef MDEBUG
        sei();
     #endif  
      
    // control de multiplexer 
    //enviar 1 o 0 depende del valor del bit en c/u de las posiciones
     digitalWrite(  selPin[0],bitRead(pin,1));
     digitalWrite(  selPin[1],bitRead(pin,2));
     digitalWrite(  selPin[2],bitRead(pin,3));
    
     // digitalWrite(selPin[0], pin & 1); 
     // digitalWrite(selPin[1], pin & 2); 
     // digitalWrite(selPin[2], pin & 4); 
 
    // the selPin[2], selPin[1], selPin[0], are connecting to pins A2,A1,A0 of multiplexer respectivily  
      MDEBUG_PRINT("sensorActual=");MDEBUG_PRINTLN(sensorActual);
      MDEBUG_PRINT("sel Multiplexor=");MDEBUG_PRINT((pin&4)? "1":"0");MDEBUG_PRINT((pin&2)? "1":"0");MDEBUG_PRINT((pin&1)? "1":"0");MDEBUG_PRINTLN(); 
   
    if (digitalRead(muxSignal)==LOW)  //if actual sensor muxSignal is low 
     {
        MDEBUG_PRINT("Señal Activada ");
       estadoSensores= sensorActual | estadoSensores; //almacena el estado del sensor actual byte sensores
         digitalWrite(pin13,HIGH);
      }
       
    //Imprimir estasensores
    MDEBUG_PRINT("estadosensores=");MDEBUG_PRINT((estadoSensores&128)? "1":"0");MDEBUG_PRINT((estadoSensores&64)? "1":"0");MDEBUG_PRINT((estadoSensores&32)? "1":"0");
    MDEBUG_PRINT((estadoSensores&16)? "1":"0");MDEBUG_PRINT((estadoSensores&8)? "1":"0");MDEBUG_PRINT((estadoSensores&4)? "1":"0");MDEBUG_PRINT((estadoSensores&2)? "1":"0");
    MDEBUG_PRINT((estadoSensores&1)? "1":"0");MDEBUG_PRINTLN(); 
    #ifdef MDEBUG
       cli();
    #endif 
  
    }


//------------------Password  variables-----------------------------------//
//----------------------------------------------------------------------//

     volatile int seconds;
     volatile int count=0;
     volatile boolean enabledMenu;	
     #define LEDPIN 13 
     Password password = Password( "1234" );
//------------------Password  Functions-----------------------------------//     
        //---------------------------------------
        boolean passwordIsValid(){
         return enabledMenu;
        }
        //---------------------------------------
        void checkPassword(){
          if (password.evaluate()){
            ADEBUG_PRINTLN("Entrando al checkPassword");
            lcd.clear();
            lcd.print("Password Correcto ");
            lcd.setCursor(0,1);
            lcd.print("Menu operable");
            enabledMenu = true;
            if (!temporizarMenu)  temporizarMenu=true; // si esta pidiendo password  por alarma activada
            lcd.clear();
            Menu.enable(true);
            digitalWrite(LEDPIN,HIGH);
            seconds=120;
            password.reset();
            //Add code to run if it works
          }else{
            ADEBUG_PRINT("Wrong-> ");
             ADEBUG_PRINTLN(password.getGuess());
             password.reset();
             lcd.clear();
             lcd.print("password invalido");
        //
            //add code to run if it did not work
          }
        }

//------------------Keypad  variables-----------------------------------//
//----------------------------------------------------------------------//
    // keypad type definition
    const byte ROWS = 4; //four rows
    const byte COLS = 4; //three columns
    char keys[ROWS][COLS] =
     {{'A','3','2','1'},
      {'B','6','5','4'},
      {'C','9','8','7'},
      {'D','#','0','*'}};
    byte rowPins[ROWS] = {
      5, 4, 3, 2}; //connect used to the row pinouts of the keypad
    byte colPins[COLS] = {
      9, 8, 7, 6}; // connect to the column pinouts of the keypad
    
    Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
    
//------------------Keypad  Functions for validation password-----------------------------------//  
    void keypadEvent(KeypadEvent eKey){
          switch (keypad.getState()){
            case PRESSED:
        	ADEBUG_PRINT("Pressed: ");
        	ADEBUG_PRINTLN(eKey);
                ADEBUG_PRINT("Contador: ");ADEBUG_PRINTLN(count);
               
               if (!enabledMenu)
                {
                 
                  ADEBUG_PRINT("Entrando a enabledMenu : ");
                  ADEBUG_PRINT(eKey);
        	        switch (eKey){
        	         case '*': checkPassword(); break;
        	         case '#': password.reset(); break;
        	         default: password.append(eKey);
                   }
                   if ((count==0)&&(temporizarMenu)){ 
                      lcd.clear();
                      lcd.print("Menu No Activo");
                      lcd.setCursor(0,1);
//                      lcd.setCursor(0,1);
//                      lcd.print("                ");
//                      lcd.setCursor(0,1);
                      }
                   lcd.print(eKey);
                   count++;
                   if ((count >4)&&(!enabledMenu) ){
                      password.reset();
                      lcd.clear();
                      count=0;
                      lcd.print("Password Invalido");
                      lcd.setCursor(0,1);
                      lcd.print("Intente nuevamente");
                     }
                 }
                else
                 {
                  lcd.clear();
                  lcd.print("el tiempo corre");
                  }
            }//Fin de switch
        }


        
        
//------------------Active Menu Time Function-----------------------------------//
//----------------------------------------------------------------------//        
        
        //------ Interrupt Service Routine used to count the active menu time 
        ISR(TIMER1_COMPA_vect)
        {
          
         
                if (temporizarMenu)
                   {
                    //http://forums.adafruit.com/viewtopic.php?f=19&t=27089
                    seconds--;
                    if (seconds < -10) seconds=-1;
                    ADEBUG_PRINT("second:");     ADEBUG_PRINTLN(seconds);

                     if ( seconds == 0 )
                     {
                      Menu.enable(false);
                      enabledMenu = false;
                      sei();
                      lcd.clear();
                      lcd.print("Menu No Activo");
                      lcd.setCursor(0,1);
                      lcd.print("Ingrese password");
                      cli();
                      count=0;
                       }
                    }
                 else  //Temporizar activar sirena
                     tSensorDisparado++;
                     if ( tSensorDisparado == tLanzarSirena )
                      { 
                        digitalWrite(pin13, HIGH); 
                        sei();
                        lcd.clear();
                        lcd.print("Alarma activada");
                        lcd.setCursor(0,1);
                        lcd.print("Ingrese password");
                        cli();  
                       }
              }  
        
 

void setup() {

  // recupera valores ante una perdida de energia del dispositivo
   #ifdef ARMAR
   OMEEPROM::read(pos_EEPROM_estadoAlarma, estadoAlarma); // inicializa estado de la alarma
   #endif

   OMEEPROM::read(pos_EEPROM_clave, clave); // inicializa contraseña actual alarma
   OMEEPROM::read(pos_EEPROM_estadoZonas, estadoZonas); // inicializa estado de las zonas
   OMEEPROM::read(pos_EEPROM_zonaAsigS1, zonaAsigS1); // inicializa zona asignada a sensor 1
   OMEEPROM::read(pos_EEPROM_zonaAsigS2, zonaAsigS2); // inicializa zona asignada a sensor 2
   OMEEPROM::read(pos_EEPROM_tipoS1, tipoS1); // inicializa tipo asignado a sensor 1
   OMEEPROM::read(pos_EEPROM_tipoS2, tipoS2); // inicializa tipo asignado a sensor 2
   OMEEPROM::read(pos_EEPROM_estadoSensores, estadoSensores); // Inicializa el estado actual de sensores, sensor 1 en bit 1, sensor 2 en bit2,...etc
 
   seconds=120;
   Serial.begin(9600);  // Used to type in characters
   keypad.addEventListener(keypadEvent); //add an event listener for this keypad

  
 
  //------- LCD Settings
   lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight

   // ------- Quick 3 blinks of backlight  -------------
        for(int i = 0; i< 3; i++)
        {
          lcd.backlight();
          delay(250);
          lcd.noBacklight();
          delay(250);
        }
        lcd.backlight(); // finish with backlight on  

   //-------- Write characters on the display ------------------
      // NOTE: Cursor Position: (CHAR, LINE) start at 0  
    lcd.setCursor(0,0); //Start at character 4 on line 0
    lcd.print("Iniciando");
    delay(1000);

   //------ Interruptions Settings
        pinMode(LEDPIN, OUTPUT);
     
        // initialize Timer1
        cli();          // disable global interrupts
        TCCR1A = 0;     // set entire TCCR1A register to 0
        TCCR1B = 0;     // same for TCCR1B
     
        // set compare match register to desired timer count:
        OCR1A = 15624;
        // turn on CTC mode:
        TCCR1B |= (1 << WGM12);
        // Set CS10 and CS12 bits for 1024 prescaler:
        TCCR1B |= (1 << CS10);
        TCCR1B |= (1 << CS12);
        // enable timer compare interrupt:
        TIMSK1 |= (1 << OCIE1A);
      //    attachInterrupt(0, activarMenu, CHANGE); // attach ISR 
        // enable global interrupts:
        sei();      // enable global interrupts

  
  
  uiClear();
  
  Menu.setDrawHandler(uiDraw);
  Menu.setExitHandler(uiClear);
  Menu.setAnalogButtonPin(BUT_PIN, BUT_MAP, BUT_THRESH);
  Menu.enable(true); 
  
  
  //Activation of alarm (Multiplexer  and gate)
  // Settings
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

void loop() {
 Menu.checkInput();
 keypad.getKey();
 
}




//------ Password Validation Functions
   