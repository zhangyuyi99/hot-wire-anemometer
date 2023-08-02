#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#define ANALOG_READ_PIN 32
#define LOW_BATTERY_VOLTAGE 7.0  // Low battery threshold
#define BATTERY_READ_PIN 16  // Replace with your battery read pin
#define R1 10000   // resistor values in battery circuit
#define R4 5100

const char* ssid = "Jimmy";
const char* password = "12345678";

WebServer server(80);

enum Mode {
  READ_ANEMOMETER,
  SEND_PIN_READ,
  PRINT_IP,
  CALIBRATE
};

Mode mode = CALIBRATE;

const int analog_pins[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int led_pins[] = {42, 41, 13, 17, 18, 46, 11, 12, 14, 21};

void readAnemometer() {
  int result = analogRead(10);
  if (result > 950 && result < 1200)
    digitalWrite(21, HIGH);
  else
    digitalWrite(21, LOW);
  // Serial.println(result);
  delay(20);
}

void sendPinReadThroughWiFi() {
  server.handleClient();
}

void printIP() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void calibrate() {

  // Include the timestamp at the beginning of the response
  unsigned long timestamp = millis();
  Serial.print(String(timestamp));
  Serial.print(",");
  
  int pin_order[] = {4, 2, 10, 3, 8, 5, 9, 1, 7};  // specify the order here
  for (int i = 0; i < 9; i++) { // Change the loop to iterate over the length of the pin_order array
    int value = analogRead(pin_order[i]);
    Serial.print(String(value));
    if(i < 8) {
      Serial.print(",");
    }
    if (value >= 1000 && value <= 1200) {
      digitalWrite(led_pins[i], HIGH);
    } else {
      digitalWrite(led_pins[i], LOW);
    }
  }
  Serial.println();
  delay(10);  // wait for 10 milliseconds
}

void setup() {
  // Common setup
  Serial.begin(115200);

  // Setup for anemometer reading
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);
  for (int i = 0; i < 1000000; i++);
  digitalWrite(21, LOW);
  delay(500);
  digitalWrite(21, HIGH);

  // configure BATTERY_READ_PIN as input
  pinMode(BATTERY_READ_PIN, INPUT); 

  // Setup for WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to WiFi");

  for (int i = 0; i < 10; i++) {
    pinMode(analog_pins[i], INPUT);
    pinMode(led_pins[i], OUTPUT);
  }

  server.on("/", HTTP_GET, []() {
    int pin_order[] = {4, 2, 10, 3, 8, 5, 9, 1, 7};  // specify the order here
    String response = "";

    // Include the timestamp at the beginning of the response
    unsigned long timestamp = millis();
    response += String(timestamp);
    response += ",";

    Serial.print(String(timestamp)); // print timestamp to Serial
    Serial.print(",");
    
    for (int i = 0; i < 9; i++) {
      int value = analogRead(pin_order[i]);
      response += String(value);
      if(i < 8) {
        response += ",";
      }
      
      // Print pin value to Serial
      Serial.print(String(value));
      if(i < 8) {
        Serial.print(",");
      }
      
      if (value >= 1000 && value <= 1200) {
        digitalWrite(led_pins[i], HIGH);
      } else {
        digitalWrite(led_pins[i], LOW);
      }
    }

    Serial.println(); // End the line on Serial
    
    server.send(200, "text/plain", response); // Send the response via WiFi
  });




  server.on("/mode/anemometer", HTTP_GET, []() {
    mode = READ_ANEMOMETER;
    server.send(200, "text/plain", "Switched to anemometer mode");
  });

  server.on("/mode/pinread", HTTP_GET, []() {
    mode = SEND_PIN_READ;
    server.send(200, "text/plain", "Switched to pin read mode");
  });

  server.on("/mode/ip", HTTP_GET, []() {
    mode = PRINT_IP;
    server.send(200, "text/plain", "Switched to IP print mode");
  });

  server.on("/mode/calibrate", HTTP_GET, []() {
    mode = CALIBRATE;
    server.send(200, "text/plain", "Switched to calibrate mode");
  });

  server.begin();
}

void loop() {
  // handle incoming client requests
  server.handleClient();

    // Read battery voltage
  float batteryVoltage = analogRead(BATTERY_READ_PIN) * (8.4 / 4095.0);  // adjust as necessary for your ADC

  float LOW_SIGNAL_VOLTAGE = LOW_BATTERY_VOLTAGE * R4 / (R1+R4);

  while (batteryVoltage <= LOW_SIGNAL_VOLTAGE) {
    for (int i = 0; i < 10; i++) {
      digitalWrite(led_pins[i], HIGH);  // turn ON the LEDs
    }
    delay(500);  // delay for half a second
    for (int i = 0; i < 10; i++) {
      digitalWrite(led_pins[i], LOW);  // turn OFF the LEDs
    }
    delay(500);  // delay for half a second

    batteryVoltage = analogRead(BATTERY_READ_PIN) * (8.4 / 4095.0);  // update the battery voltage value
  }

  switch (mode) {
    case READ_ANEMOMETER:
      readAnemometer();
      break;
    case SEND_PIN_READ:
      sendPinReadThroughWiFi();
      break;
    case PRINT_IP:
      printIP();
      break;
    case CALIBRATE:
      calibrate();
      break;
  }
}


