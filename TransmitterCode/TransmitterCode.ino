//PINS
//ALTSOFSERIAL RX IS PIN 8
#include <SoftwareSerial.h>

SoftwareSerial gps(4,3);

void shiftWrite(String message) {
  message = "[" + message + "]";

  bool sendData = true;
  for (int i = 0; i < message.length() && sendData == true; i++) {
    if (message[i] == ']') {
      sendData = false;
    }
    Serial.write(message[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting!");
  gps.begin(115200);
}

// Message related data variables
bool msg_start = false;
bool msg_end = false;
char msg[256];
int msg_index = 0;

void loop() {
  gps.listen();
  if (gps.available()) {
    // Read data
    byte data = gps.read();
    if (data == '$') {
      // Reset all variables
      msg_start = true;
      msg_end = false;
      memset(msg, 0, sizeof msg);
      msg_index = 1;
      msg[0] = '$';
      
    } else if (data == '\n') {
      // Signify message end
      msg_start = false;
      msg_end = true;

      // Transmit message
      shiftWrite(msg);
      
    } else {
      // Build the message
      msg[msg_index] = data;
      msg_index++;
    }
  }
}


