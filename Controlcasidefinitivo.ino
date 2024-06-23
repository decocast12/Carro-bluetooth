#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// Esta es una función que nos dice si el control ha sido conectado.
// Se pueden conectar hasta 4 controles.
void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Control conectado!, index=%d\n", i);
      // Adicional a eso se pide información del control tales como:
      // Modelo, VID, PID, Dirección de bluetooth, flags, etc.
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Control modelo: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
    }
  }
  
  if (!foundEmptySlot) {
    Serial.println("CALLBACK: Control conectado, pero sin espacio para conectar");
  }  
}

// Esta detecta si el control se desconectó
void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Control desconectado de index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }

  if (!foundController) {
    Serial.println("CALLBACK: Control desconectado, pero no encontrado en myControllers");
  }
}

void dumpGamepad(ControllerPtr ctl) {
  Serial.printf(
    "idx=%d, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d\n",
    ctl->index(),        // Controller Index
    //ctl->dpad(),         // touchpad
    //ctl->buttons(),      // bitmask de botones presionados
    ctl->axisX(),        // (-511 - 512) Izquierda eje x
    ctl->axisY(),        // (-511 - 512) Izquierda eje y
    ctl->axisRX(),       // (-511 - 512) Derecha eje x
    ctl->axisRY(),       // (-511 - 512) Derecha eje y
    ctl->brake(),        // (0 - 1023): freno
    ctl->throttle()      // (0 - 1023): acelerador
    //ctl->miscButtons(),  // bitmask of pressed "misc" buttons
    //ctl->gyroX(),        // Giroscopio X
    //ctl->gyroY(),        // Giroscopio Y
    //ctl->gyroZ(),        // Giroscopio Z
    //ctl->accelX(),       // Acelerómetro X
    //ctl->accelY(),       // Acelerómetro Y
    //ctl->accelZ()        // Acelerómetro Z
  );
}

void processGamepad(ControllerPtr ctl) {
  if (ctl->a()) {
    static int colorIdx = 0;
    // Aquí modificamos el color del LED del control cuando encienda:
    // Se pueden cambiar a gusto modificando los números de cada color:
    switch (colorIdx % 3) {
      case 0:
        // Rojo
        ctl->setColorLED(150, 0, 0);
        break;
      case 1:
        // Verde
        ctl->setColorLED(0, 100, 0);
        break;
      case 2:
        // Azul
        ctl->setColorLED(0, 0, 200);
        break;
    }
    colorIdx++;
  }
  // Aquí se obtiene la información de lo que se presiona en el control
  dumpGamepad(ctl);
}

void processControllers() {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    ControllerPtr myController = myControllers[i];
    if (myController && myController->isConnected() && myController->hasData()) {
      if (myController->isGamepad()) {
        processGamepad(myController);
      } else {
        Serial.println("Control no soportado");
      }
    }
  }
}

// Se obtiene la direccion MAC del ESP32
void setup() {
  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BDAddr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Aquí se llaman los callbacks de la librería Bluepad
  BP32.setup(&onConnectedController, &onDisconnectedController);
}

void loop() {
  // Aquí finalmente llamamos los datos y los mostramos en el monitor serial
  bool dataUpdated = BP32.update();
  if (dataUpdated) {
    processControllers();
  }
  delay(100);
}
