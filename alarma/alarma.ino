#include <EEPROM.h>


/**  Example OMMenuMgr Sketch

 @author
 C. A. Church
 
 */
#include "OMEEPROM.h"
#include "OMMenuMgr.h"
#include <LiquidCrystal_I2C.h>

//#include <LiquidCrystal.h>
#include <Wire.h>  // Comes with Arduino IDE
//#include <Keypad.h>
//#include <Password.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
// #include <MenuBackend.h>



/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address



 // ui setup
 
 // ui setup
 
    // lcd pins
//const byte LCD_RS  = 17;
//const byte LCD_EN  = 18;
//const byte LCD_D4  = 11;
//const byte LCD_D5  = 8;
//const byte LCD_D6  = 7;
//const byte LCD_D7  = 4;
//
//
//const byte LCD_ROWS = 2;
//const byte LCD_COLS = 16;

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

// ====== Test Menu =========== 

byte foo = 0;
byte sel = 0;
unsigned int bar = 1;
long baz  = 0;
float bak = 0.0;

byte estadoAlarma = 0;
//--SubMenu Armar Sistema
 MENU_FLAG    armar_flag    = { 3, &estadoAlarma };
 MENU_VALUE   armar_value   = { TYPE_BFLAG, 0, 0, MENU_TARGET(&armar_flag) };
 MENU_ITEM    item_armar    = { {"Armar Sistema"}, ITEM_VALUE, 0, MENU_TARGET(&armar_value) };

//--SubMenu Cambiar Password
int clave;
MENU_VALUE clave_value = { TYPE_UINT,       9999, 1000,   MENU_TARGET(&clave) };
MENU_ITEM item_clave    = { {"Cambiar Clave"}, ITEM_VALUE,  0, MENU_TARGET(&clave_value) };

//--SubMenu Estado de zonas
byte estadoZonas=0;
 //zona1 
 MENU_FLAG    zona1_flag    = { 1, &estadoZonas };
 MENU_VALUE   zona1_value   = { TYPE_BFLAG, 0, 0, MENU_TARGET(&zona1_flag) };
 MENU_ITEM    item_zona1    = { {"Zona1"}, ITEM_VALUE, 0, MENU_TARGET(&zona1_value) };
 //zona2 
 MENU_FLAG    zona2_flag    = { 2, &estadoZonas };
 MENU_VALUE   zona2_value   = { TYPE_BFLAG, 0, 0, MENU_TARGET(&zona2_flag) };
 MENU_ITEM    item_zona2    = { {"Zona2"}, ITEM_VALUE, 0, MENU_TARGET(&zona2_value) };
 
 //Armando subMenus Estado de Zonas
 MENU_LIST zonas_list[]   = {  &item_zona1, &item_zona2 };
 MENU_ITEM menu_estado_zonas     = { {"Estado Zonas"}, ITEM_MENU,   MENU_SIZE(zonas_list),    MENU_TARGET(&zonas_list) };

//subMenu Sensores
//------Zona Asignada
        byte zonaAsigS1;
        byte zonaAsigS2;
        // Lista de zonas posibles
        MENU_SELECT_ITEM  opc_zona1 = { 1, {"Zona1"} };
        MENU_SELECT_ITEM  opc_zona2  = { 2, {"Zona2"} };
        
        MENU_SELECT_LIST  tipos_de_zonas[] = { &opc_zona1, &opc_zona2 };
                                          
        MENU_SELECT zona_S1_sel = { &zonaAsigS1,  MENU_SELECT_SIZE(tipos_de_zonas),   MENU_TARGET(&tipos_de_zonas) };
        MENU_VALUE zona_S1_value = { TYPE_SELECT,     0,     0,     MENU_TARGET(&zona_S1_sel) };
        MENU_ITEM item_zona_S1    = { {"Zona para S1"}, ITEM_VALUE,  0,        MENU_TARGET(&zona_S1_value) };

        MENU_SELECT zona_S2_sel = { &zonaAsigS2,  MENU_SELECT_SIZE(tipos_de_zonas),   MENU_TARGET(&tipos_de_zonas) };
        MENU_VALUE zona_S2_value = { TYPE_SELECT,     0,     0,     MENU_TARGET(&zona_S2_sel) };
        MENU_ITEM item_zona_S2    = { {"Zona para S2"}, ITEM_VALUE,  0,        MENU_TARGET(&zona_S2_value) };
       
        
        
//------Tipos de sensores
        byte tipoS1;
        byte tipoS2;
        // Lista de tipos de sensores posibles
        MENU_SELECT_ITEM  opc_aper = { 2, {"Apertura"} };
        MENU_SELECT_ITEM  opc_mov  = { 1, {"Movimiento"} };
        MENU_SELECT_ITEM  opc_humo= { 0, {"Humo"} };
        
        MENU_SELECT_LIST  tipos_de_sensores[] = { &opc_aper, &opc_mov, &opc_humo };
                                          
        MENU_SELECT tipo_S1_sel = { &tipoS1,  MENU_SELECT_SIZE(tipos_de_sensores),   MENU_TARGET(&tipos_de_sensores) };
        MENU_VALUE tipo_S1_value = { TYPE_SELECT,     0,     0,     MENU_TARGET(&tipo_S1_sel) };
        MENU_ITEM item_tipo_S1    = { {"Tipo Sensor 1"}, ITEM_VALUE,  0,        MENU_TARGET(&tipo_S1_value) };

        
        MENU_SELECT tipo_S2_sel = { &tipoS2,  MENU_SELECT_SIZE(tipos_de_sensores),   MENU_TARGET(&tipos_de_sensores) };
        MENU_VALUE tipo_S2_value = { TYPE_SELECT,     0,     0,     MENU_TARGET(&tipo_S2_sel) };
        MENU_ITEM item_tipo_S2    = { {"Tipo Sensor 2"}, ITEM_VALUE,  0,        MENU_TARGET(&tipo_S2_value) };

//-----Estados
       byte estadoSensores=0;
       //Estado Sensor 1 
       MENU_FLAG    estado_S1_flag    = { 1, &estadoSensores };
       MENU_VALUE   estado_S1_value   = { TYPE_BFLAG, 0, 0, MENU_TARGET(&estado_S1_flag) };
       MENU_ITEM    item_estado_S1    = { {"estado S1"}, ITEM_VALUE, 0, MENU_TARGET(&estado_S1_value) };
      
      //Estado Sensor 2 
       MENU_FLAG    estado_S2_flag    = { 2, &estadoSensores };
       MENU_VALUE   estado_S2_value   = { TYPE_BFLAG, 0, 0, MENU_TARGET(&estado_S2_flag) };
       MENU_ITEM    item_estado_S2    = { {"estado S2"}, ITEM_VALUE, 0, MENU_TARGET(&estado_S2_value) };


//-----Arma submenus
//                Sensores
//                   Sensor1
//                   Sensor2
 MENU_LIST sensor1_list[]   = {  &item_zona_S1, &item_tipo_S1, &item_estado_S1 };
 MENU_ITEM item_sensor1     = { {"Sensor 2"}, ITEM_MENU,   MENU_SIZE(sensor1_list),    MENU_TARGET(&sensor1_list) };

 MENU_LIST sensor2_list[]   = {  &item_zona_S2, &item_tipo_S2, &item_estado_S2 };
 MENU_ITEM item_sensor2     = { {"Sensor 2"}, ITEM_MENU,   MENU_SIZE(sensor2_list),    MENU_TARGET(&sensor2_list) };

 MENU_LIST sensores_list[]   = {  &item_sensor1, &item_sensor2 };
 MENU_ITEM menu_sensores     = { {"Sensores"}, ITEM_MENU,   MENU_SIZE(sensores_list),    MENU_TARGET(&sensores_list) };


MENU_LIST root_list[]   = {  &item_armar, &item_clave, &menu_estado_zonas, &menu_sensores};
MENU_ITEM menu_root     = { {"Root"},        ITEM_MENU,   MENU_SIZE(root_list),    MENU_TARGET(&root_list) };


OMMenuMgr Menu(&menu_root);



void setup() {

  Serial.begin(9600);  // Used to type in characters
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

  
  
  uiClear();
  
  Menu.setDrawHandler(uiDraw);
  Menu.setExitHandler(uiClear);
  Menu.setAnalogButtonPin(BUT_PIN, BUT_MAP, BUT_THRESH);
  Menu.enable(true); 
  
  
}

void loop() {
 Menu.checkInput();
 
}

void testAction() {

 digitalWrite(5, HIGH);  
}


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


void uiQwkScreen() {
  lcd.clear();
  Menu.enable(false);
  
  lcd.print("Action!");
  lcd.setCursor(0, 1);
  lcd.print("Enter 2 return");
  
  while( Menu.checkInput() != BUTTON_SELECT ) {
    ; // wait!
  }
  
  Menu.enable(true);
  lcd.clear();
}  

