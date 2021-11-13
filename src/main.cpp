#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "FastLED.h"

#pragma region FastLED vars

#define DATA_PIN    6
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    8 
#define BRIGHTNESS  50
CRGB leds[NUM_LEDS];

#pragma endregion

#pragma region ESP8266WebServer vars

const char* ssid = "110110101001";
const char* password = "11111111";
 
ESP8266WebServer server(80);
IPAddress ip(192, 168, 43, 201); 
IPAddress gateway(192, 168, 43, 1); 
IPAddress subnet(255, 255, 255, 0); 

void handle_home_page() {
 
    char body[1024];
  sprintf(body,  "<html> <head>   <title>ESP8266 Page</title> <meta name='viewport' content='width=device-width, initial-scale=1.0'>  <style>     h1 {text-align:center; }     td {font-size: 50%%; padding-top: 30px;}     .temp {font-size:150%%; color: #FF0000;}     .press {font-size:150%%; color: #00FF00;}     .hum {font-size:150%%; color: #0000FF;}   </style> </head>  <body>    <h1>ESP8266 Sensor Page</h1>    <div id='div1'>        <table>           <tr>            <td>Temperature</td><td class='temp'>%d</td>          </tr>          <tr>   <td>Pressure</td><td class='press'>%.2f</td>   </tr>    <tr>   <td>Humidity</td><td class='hum'>%.2f</td>  </tr><tr><td><form action=\"\\slider\" > <button>Slider</button> </form></td></tr> </div> </body>  </html>", 1, 2.4, 1.1);
  server.send(200, "text/html", body);
  
}

void handle_slider_page() {
  
  char body[2048];
  sprintf(body,  "<html> <head> <title>ESP8266 Page</title> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <style> .slidecontainer { width: 100%%; } .slider { -webkit-appearance: none; width: 100%%; height: 15px; border-radius: 5px; background: #d3d3d3; outline: none; opacity: 0.7; -webkit-transition: .2s; transition: opacity .2s; } .slider:hover { opacity: 1; } .slider::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 25px; height: 25px; border-radius: 50%%; background: #04AA6D; cursor: pointer; } .slider::-moz-range-thumb { width: 25px; height: 25px; border-radius: 50%%; background: #04AA6D; cursor: pointer; } </style> </head> <body> <h1>SliderPage</h1> <div class='slidecontainer'> <input type='range' min='1' max='100' value='50' class='slider' id='myRange'> <p>Value: <span id='demo'></span></p> </div> <script> var xhr = new XMLHttpRequest(); var slider = document.getElementById('myRange'); var output = document.getElementById('demo'); output.innerHTML = slider.value; var intervaltimer = setTimeout(intervalfunc,200); slider.oninput = function() { output.innerHTML = this.value; clearTimeout(intervaltimer); intervaltimer = setTimeout(intervalfunc,100); }; function intervalfunc(){ xhr.open('POST','/sliderupdate?value='+slider.value); xhr.send(null); } </script> </body> </html>");
  server.send(200, "text/html", body);
}

void handle_slider_update(){
  
  // Serial.println(server.arg("value"));
  int val = map(server.arg("value").toInt(),0,100,0,255);
  leds[1] = CRGB(val,0,0);
  leds[2] = CRGB(0,val,0);
  leds[3] = CRGB(0,val,0);
  leds[4] = CRGB(0,0,val);
  leds[5] = CRGB(0,0,val);
  leds[6] = CRGB(0,0,val);
  leds[0] = CRGB(0,0,0);
  FastLED.show();
  server.send(200,"","");

}
#pragma endregion

void setup() {
  Serial.begin(115200);
  delay(10);
 
  #pragma region ESP8266WebServer setup

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.config(ip,gateway,subnet);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  //Serial.println(WiFi.gatewayIP().toString().c_str());
 
  server.on("/led", handle_home_page);
  server.on("/slider", handle_slider_page);
  server.on("/sliderupdate", HTTP_POST, handle_slider_update);

  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  Serial.printf("Gataway IP: %s\n", WiFi.gatewayIP().toString().c_str());
  Serial.print("Subnet mask: ");
  Serial.println(WiFi.subnetMask());

  #pragma endregion
 
  #pragma region FastLED setup

  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);
  FastLED.setBrightness(BRIGHTNESS);
  delay(1000);
  leds[1] = CRGB(255,0,0);
  leds[2] = CRGB(0,255,0);
  leds[3] = CRGB(0,255,0);
  leds[4] = CRGB(0,0,255);
  leds[5] = CRGB(0,0,255);
  leds[6] = CRGB(0,0,255);
  leds[0] = CRGB(0,0,0);
  FastLED.show();
  #pragma endregion
}


void loop() {
  server.handleClient();
}