# Setting up the Controller

#### Devices Required:

1. [Open Manipulator-X Robot from Dynamixel ](https://emanual.robotis.com/docs/en/platform/openmanipulator_x/overview/)
2. [Open CR Chip](https://emanual.robotis.com/docs/en/parts/controller/opencr10/)
3. [ESP8266 Wifi Module](https://www.sparkfun.com/products/17146)
4. Jumper Wires, Breadboard

#### How to connect the devices:

###### **Connect the ESP8266 to OpenCR**:

* Connect the `VCC` of the ESP8266 to the `3.3V` of OpenCR.
* Connect the `GND` of the ESP8266 to the `GND` of OpenCR.
* Connect the `TX` of the ESP8266 to the `RX` (or a suitable UART pin) of OpenCR.
* Connect the `RX` of the ESP8266 to the `TX` (or a suitable UART pin) of OpenCR.

###### **Connect OpenManipulator-X to OpenCR**:

Follow the official documentation to ensure the OpenManipulator-X is correctly connected to the OpenCR board. (For my setup, I just had to connect the combined 4 wires to the chip, power the Open CR and connect the Open Manipulator X to an external source.)

<img src="image/README/1694471878390.png" alt="drawing" width="200"/>

#### Running the Code

###### **Upload the Code to OpenCR**:

* Ensure you have the necessary libraries installed in your Arduino IDE (or whichever platform you're using). This includes the `ESP8266WiFi` and `ESP8266WebServer` libraries.
* Connect the OpenCR board to your computer.
* Select the correct board and port in the Arduino IDE.
* Upload the code.
* Run the server/server.ino

###### **Run Website:**

* cd inside the robotics_ui/ folder
* npm start to run the website
