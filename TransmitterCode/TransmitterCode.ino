//PINS
//ALTSOFSERIAL RX IS PIN 8
#include <SoftwareSerial.h>

#define TRANSMIT_MODE 1 // Use mode number from below

// Transmit modes
#define OFFLINE_TEST 0  // Fake NMEA sentences
#define NO_GPS_TEST  1  // Sends just millis
#define FULL_MODE    2  // Full run mode for the telemetry

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
  Serial.write('\n');
}

void setup() {
  Serial.begin(9600);
//  Serial.println("Starting!");
  gps.begin(115200);
}

// Message related data variables
bool msg_start = false;
bool msg_end = false;
char msg[256];
int msg_index = 0;

void loop() {
#if TRANSMIT_MODE == OFFLINE_TEST
  String msg0 = "$GPGGA,184353.07,1929.045,S,02410.506,E,1,04,2.6,100.00,M,-33.9,M,,0000*6D";
  shiftWrite(msg0);
  delay(1000);

  String msg1 = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
  shiftWrite(msg1);
  delay(1000);
#endif /* TRANSMIT_MODE == OFFLINE_TEST */

#if TRANSMIT_MODE == NO_GPS_TEST
  unsigned long time;
  time = millis();
  shiftWrite("Time: " + String(time));    //prints time since program started
  delay(200);          // wait 200ms to send another packet of data
#endif /* TRANSMIT_MODE == NO_GPS_TEST */
  
#if TRANSMIT_MODE == FULL_MODE
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
#endif /* TRANSMIT_MODE == FULL_MODE */
}
