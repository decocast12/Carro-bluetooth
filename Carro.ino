/*Este es el código para el control del carro vía bluetooth.*/

/*En esta parte llamamos a las librerías necesarias para comunicar el cont
rol de ps4 con el ESP32, la de wifi sirve para obtener la dirección MAC y la otra para facilitar los comandos desde el control.*/

#include <wifi.h>
#include <PS4Controller.h>

//LA DOCUMENTACIÓN DE LAS LIBRERÍAS:
//PS4Controller https://github.com/pablomarquez76/PS4_Controller_Host
//https://github.com/arduino-libraries/WiFi

/*No sé si te acuerdas pero toda función en Arduino o ESP32 debe tener dos funciones principales para funcionar,
Una función setup, en la que se definen los pines, se inicia la comunicación serial, se inician las librerías, etc.
*/


void setup() {
  /*Aquí inciamos la comunicación serial a 115200 baudios*/
  Serial.begin(115200);
 
  //luego se obtiene la dirección mac y la almacenamos en la variable macESP32
  macESP32 = WiFi.macAddress();
  
  //Aquí decimos que si el control de ps4 se ha logrado conectar al ESP32 confirme, y si no pues no :v
  if (PS4.begin(macESP32)) {
    Serial.println("Control conectado");
  } else {
    Serial.println("No se ha detectado el control.");
  }
    
}

/*También la función loop es donde se ejecuta el código que debe repetirse indefinidamente,
como la lectura de los datos recibidos del control, que serán los parámetros de los joysticks, los gatillos o botones,
según se necesite.*/

void loop() {
  //En esta parte empezamos a recibir la información de ambos joysticks
    if (PS4.isConnected()) {
      int lx = PS4.LStickX();
      int ly = PS4.LStickY();
      int rx = PS4.RStickX();
      int ry = PS4.RStickY();

      
      //SI LE HACES CAMBIOS AL CÓDIGO NO TE OLVIDES DE COMENTARLO, GONORREA.
    
    }
  //Un pequeño delay para que la lectura de datos no se sature.
  delay(20);
}
