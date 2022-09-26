#include <Arduino.h>
#include <WiFi.h>

// RGB LED Module PINS
#define RGB_RED_PIN 12
#define RGB_BLUE_PIN 14
#define RGB_GREEN_PIN 27

// WiFi Credentials
const char* ssid = "Follow the Kittens"; // Name
const char* password = "TuringFelix49";  // Password

// Creates an server object in port 80
WiFiServer server(80);

// Variable that saves petitions from client
String header;

// Led power status
char power_status = 0;
// Is the rainbow_mode active? 0 for not, 1 for yes
char rainbow_status = 0;
// Counter that help us to decise when to change de color in rainbow_mode
int rainbow_counter = 0;
// Initial values for heach color to make it more dinamic
short blue_value = 1;
short red_value = 31;
short green_value = 101;

// Content sent to client
const char *HTML =
 #include "index.html"
;

// Prototypes
void led_on();
void led_off();
void led_toggle();
void rainbow_mode();
void handle_client(WiFiClient, String);
void handle_header(String);

void setup() {
  // Setup pins
  pinMode(RGB_BLUE_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_RED_PIN, OUTPUT);

  // Initialises RGB LED
  analogWrite(RGB_BLUE_PIN, 0);
  analogWrite(RGB_RED_PIN, 0);
  analogWrite(RGB_GREEN_PIN, 0);

  // Connect to WiFi using credentials
  WiFi.begin(ssid, password);
  // Waits for connection for complete
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Starts the web server
  server.begin();

  delay(100);
}

void loop() {
  // Listen for incoming clients
  WiFiClient client = server.available();

  // If a new client connects
  if (client) {
    // Handle the client and set the headers
    handle_client(client, header);
  }

  // Counter to know if we need to change color in rainbow_mode
  rainbow_counter++;

  // If there is 4000 steps in counter and have rainbow status on
  if ((rainbow_counter % 4000 == 0) && rainbow_status){
    // Change value per color
    blue_value += 6;
    red_value += 4;
    green_value += 2;

    // If value is bigger than 255 change it
    if (blue_value > 255){ blue_value -= 512; }
    if (red_value > 255){ red_value -= 512; }
    if (green_value > 255){ green_value -= 512; }

    // Use absolute value to force values between 0 and 255
    analogWrite(RGB_BLUE_PIN, abs(blue_value));
    analogWrite(RGB_RED_PIN, abs(red_value));
    analogWrite(RGB_GREEN_PIN, abs(green_value));
  }
}

// Handle client requests and responses
void handle_client(WiFiClient client, String header){
  // Make a String to hold incoming data from the client
  String currentLine = "";

  // Loop while the client's connected
  while (client.connected()) {

    // If there's bytes to read from the client,
    if (client.available()) {

      // Read a byte
      char c = client.read();

      // Adds client's data to header variable
      header += header + c;

      // If the byte is a newline character
      if (c == '\n') {
      // If the current line is blank, you got two newline characters in a row
      // that's the end of the client HTTP request, so send a response:

        if (currentLine.length() == 0) {
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200
          // OK) and a content-type so the client knows what's coming, then a
          // blank line:
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();

          // Reacts to header's content
          handle_header(header);

          // Sends html page to client
          client.println(HTML);

          // To end the response to client, print an empty line
          client.println();

          // Break connection to client
          break;

        // Cleans contents readed from client
        } else {
          currentLine = "";
        }

      } else if (c != '\r'){
        currentLine += c;
      }
    }
  }

  // Clear the header variable
  header = "";

  // Close the connection
  client.stop();
}

// Handle contents in header
void handle_header(String header){
  // Reacts to header's content
  if (header.indexOf("GET /toggle") >= 0) {
    led_toggle();
  } else if (header.indexOf("GET /rainbow") >= 0){
    rainbow_mode();
  }
}

// Toggles led power
void led_toggle(){
  if (power_status) {
    power_status = 0;
    rainbow_status = 0;
    led_off();
  } else {
    power_status = 1;
    led_on();
  }
}

// Turns rainbow_mode on
void rainbow_mode(){
  if (rainbow_status) {
    power_status = 0;
    rainbow_status = 0;
    led_off();
  } else {
    power_status = 1;
    rainbow_status = 1;
  }
}

// Turns led on
void led_on() {
  analogWrite(RGB_BLUE_PIN, 255);
  analogWrite(RGB_RED_PIN, 255);
  analogWrite(RGB_GREEN_PIN, 255);
}

// Turns led off
void led_off() {
  analogWrite(RGB_BLUE_PIN, 0);
  analogWrite(RGB_RED_PIN, 0);
  analogWrite(RGB_GREEN_PIN, 0);
}
