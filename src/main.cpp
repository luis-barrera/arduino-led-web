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

// Content sent to client
const char *HTML =
 #include "index.html"
;

// Prototypes
void handle_root();
void led_on();
void led_off();
void handle_client(WiFiClient client, String* header);

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

    handle_client(client, &header);
    // // Make a String to hold incoming data from the client
    // String currentLine = "";

    // // Loop while the client's connected
    // while (client.connected()) {

    //   // If there's bytes to read from the client,
    //   if (client.available()) {

    //     // Read a byte
    //     char c = client.read();

    //     // Adds client's data to header variable
    //     header += header + c;

    //     // If the byte is a newline character
    //     if (c == '\n') {
    //     // If the current line is blank, you got two newline characters in a row
    //     // that's the end of the client HTTP request, so send a response:

    //       if (currentLine.length() == 0) {
    //         // HTTP headers always start with a response code (e.g. HTTP/1.1 200
    //         // OK) and a content-type so the client knows what's coming, then a
    //         // blank line:
    //         client.println("HTTP/1.1 200 OK");
    //         client.println("Content-type:text/html");
    //         client.println("Connection: close");
    //         client.println();

    //         // Reacts to header's content
    //         if (header.indexOf("GET /on") >= 0) {
    //           led_on();
    //         } else if (header.indexOf("GET /off") >= 0) {
    //           led_off();
    //         }

    //         // TODO: create function that reads header content to display only
    //         // one button
    //         client.println(HTML);

    //         // To end the response to client, print an empty line
    //         client.println();

    //         // Break connection to client
    //         break;

    //       // Cleans contens readed from client
    //       } else {
    //         currentLine = "";
    //       }

    //     } else if (c != '\r'){
    //       currentLine += c;
    //     }
    //   }
    // }

    // // Clear the header variable
    // header = "";

    // // Close the connection
    // client.stop();
  }
}

// TODO
void handle_client(WiFiClient client, String* header_ptr){
  String header = *header_ptr;
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
          if (header.indexOf("GET /on") >= 0) {
            led_on();
          } else if (header.indexOf("GET /off") >= 0) {
            led_off();
          }

          // TODO: create function that reads header content to display only
          // one button
          client.println(HTML);

          // To end the response to client, print an empty line
          client.println();

          // Break connection to client
          break;

        // Cleans contens readed from client
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

// TODO
// void handle_header(String* header_ptr){
//   String header_str = *header_ptr;
//   // turns the GPIOs on and off
//   if (header.indexOf("GET /on") >= 0) {
//     led_on();
//   } else if (header.indexOf("GET /off") >= 0) {
//     led_off();
//   }
// }

void led_on() {
  analogWrite(RGB_BLUE_PIN, 255);
  analogWrite(RGB_RED_PIN, 255);
  analogWrite(RGB_GREEN_PIN, 255);
}

void led_off() {
  analogWrite(RGB_BLUE_PIN, 0);
  analogWrite(RGB_RED_PIN, 0);
  analogWrite(RGB_GREEN_PIN, 0);
}
