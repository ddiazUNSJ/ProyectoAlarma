#include <OMEEPROM.h>

#include <EEPROM.h>


byte estadoAlarma = 0;
const int EEPROM_estadoAlarma       = 1;
int clave;
const int EEPROM_clave       = 2;
byte estadoZonas=0;
const int EEPROM_estadoZonas       = 4;
 byte zonaAsigS1;

const int EEPROM_zonaAsigS1       = 5;
 byte zonaAsigS2;
const int EEPROM_zonaAsigS2       = 6;
//------Tipos de sensores
 byte tipoS1;
 const int EEPROM_tipoS1       = 7;
 byte tipoS2;
 const int EEPROM_tipoS2       = 8;
 byte estadoSensores=0;
       const int EEPROM_estadoSensores     = 9;
  
void setup() {
  
  estadoAlarma=255;
  clave=1234;
  estadoZonas=7;
  zonaAsigS1=1;
  zonaAsigS2=2;
  tipoS1=2;
  tipoS2=1;
  estadoSensores=7;



   OMEEPROM::write(EEPROM_estadoAlarma, estadoAlarma); // inicializa estado de la alarma
   OMEEPROM::write(EEPROM_clave, clave); // inicializa contraseña actual alarma
   OMEEPROM::write(EEPROM_estadoZonas, estadoZonas); // inicializa estado de las zonas
   OMEEPROM::write(EEPROM_zonaAsigS1, zonaAsigS1); // inicializa zona asignada a sensor 1
   OMEEPROM::write(EEPROM_zonaAsigS2, zonaAsigS2); // inicializa zona asignada a sensor 2
   OMEEPROM::write(EEPROM_tipoS1, tipoS1); // inicializa tipo asignado a sensor 1
   OMEEPROM::write(EEPROM_tipoS2, tipoS2); // inicializa tipo asignado a sensor 2
   OMEEPROM::write(EEPROM_estadoSensores, estadoSensores); // Inicializa el estado actual de sensores, sensor 1 en bit 1, sensor 2 en bit2,...etc

}

void loop() {
  // put your main code here, to run repeatedly: 
  
}
