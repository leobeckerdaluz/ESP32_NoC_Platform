//This example shows how to associate stream connection (database data changes monitoring) with 
//store, read, update database operation.

//The time that allows stream to establish is required when using the same Firebase Data object 
//together with store, read and update operations.

//Required HTTPClientESP32Ex library to be installed  https://github.com/mobizt/HTTPClientESP32Ex

#include <WiFi.h>
#include "FirebaseESP32.h"
#include <ArduinoJson.h>
#include <stdio.h>
#include "defines.h"

//Define Firebase Data object
FirebaseData firebaseData;
enum all_topologies { anel_1D, anel_2D, mesh };
int qtde_nodos_X_Y = 4;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Serial.println("------------------------------------");
  Serial.println("Begin stream connection...");

  if (!Firebase.beginStream(firebaseData, "/"))
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println();
  }
}

void loop() {
	if (!Firebase.readStream(firebaseData)) {
		Serial.println("------------------------------------");
		Serial.println("Read stream");
		Serial.println("FAILED");
		Serial.println("REASON: " + firebaseData.errorReason());
		Serial.println();
	}

	if (firebaseData.streamTimeout()) {
		Serial.println("Stream timeout, resume streaming...");
		Serial.println();
	}

	if (firebaseData.streamAvailable()) {
		String event_path = firebaseData.dataPath();
		Serial.println("Stream Data available...");
		Serial.println("STREAM PATH: " + firebaseData.streamPath());
		Serial.println("EVENT PATH: " + event_path);
		Serial.println("DATA TYPE: " + firebaseData.dataType());
		Serial.println("EVENT TYPE: " + firebaseData.eventType());

		if (event_path == "/NoC_Setup/X_Y"){
			int new_X_Y = firebaseData.intData();
			qtde_nodos_X_Y = new_X_Y;
			Serial.println("X_Y alterado com sucesso!");
			Serial.println(new_X_Y);
		}
		else if (event_path == "/NoC_Setup/topologia"){
			String topologia = firebaseData.stringData();

			Serial.println("Topologia alterado com sucesso!");
		}
		if(event_path.indexOf("/Messages") >= 0){
			String json = firebaseData.jsonData();

			Serial.println("sucesso!");
			Serial.println(json);

			// ---------- Deserialize ---------- 
			StaticJsonDocument<200> doc;

			// Deserialize the JSON document
			DeserializationError error = deserializeJson(doc, json);

			// Test if parsing succeeds.
			if (error) {
			Serial.print(F("deserializeJson() failed: "));
			Serial.println(error.c_str());
			return;
			}

			String message = doc["data"]["msg"];
			String date = doc["data"]["date"];
			int source = doc["data"]["src"];
			int target = doc["data"]["target"];
			// ---------------------------------

			// Print values.
			Serial.println(source);
			Serial.println(target);
			Serial.println(message);
			Serial.println(date);

			// Message Parameters
			// int target = 5;
			// int source = 1;
			int message_id = 12345;

			// Setup parameters
			int topologia = anel_1D;

			// Run PEs task
			// run_PEs(source, target, message_id, topologia, qtde_nodos_X_Y);
		}

		Serial.println();
	}
}









// /**
//  * The MIT License (MIT)
//  *
//  * Copyright (c) 2018 by ThingPulse, Daniel Eichhorn
//  * Copyright (c) 2018 by Fabrice Weinberg
//  *
//  * Permission is hereby granted, free of charge, to any person obtaining a copy
//  * of this software and associated documentation files (the "Software"), to deal
//  * in the Software without restriction, including without limitation the rights
//  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  * copies of the Software, and to permit persons to whom the Software is
//  * furnished to do so, subject to the following conditions:
//  *
//  * The above copyright notice and this permission notice shall be included in all
//  * copies or substantial portions of the Software.
//  *
//  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  * SOFTWARE.
//  *
//  * ThingPulse invests considerable time and money to develop these open source libraries.
//  * Please support us by buying our products (and not the clones) from
//  * https://thingpulse.com
//  *
//  */

// // Include the correct display library
// // For a connection via I2C using Wire include
// #include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
// // or #include "SH1106Wire.h", legacy include: `#include "SH1106.h"`
// // For a connection via I2C using brzo_i2c (must be installed) include
// // #include <brzo_i2c.h> // Only needed for Arduino 1.6.5 and earlier
// // #include "SSD1306Brzo.h"
// // #include "SH1106Brzo.h"
// // For a connection via SPI include
// // #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
// // #include "SSD1306Spi.h"
// // #include "SH1106SPi.h"

// // Include custom images
// #include "images.h"

// // Initialize the OLED display using SPI
// // D5 -> CLK
// // D7 -> MOSI (DOUT)
// // D0 -> RES
// // D2 -> DC
// // D8 -> CS
// // SSD1306Spi        display(D0, D2, D8);
// // or
// // SH1106Spi         display(D0, D2);

// // Initialize the OLED display using brzo_i2c
// // D3 -> SDA
// // D5 -> SCL
// // SSD1306Brzo display(0x3c, D3, D5);
// // or
// // SH1106Brzo  display(0x3c, D3, D5);

// // Initialize the OLED display using Wire library
// SSD1306Wire  display(0x3c, 3, 5);
// // SH1106 display(0x3c, D3, D5);


// #define DEMO_DURATION 3000
// typedef void (*Demo)(void);

// int demoMode = 0;
// int counter = 1;

// void setup() {
//   Serial.begin(115200);
//   Serial.println();
//   Serial.println();


//   // Initialising the UI will init the display too.
//   display.init();

//   display.flipScreenVertically();
//   display.setFont(ArialMT_Plain_10);

// }

// void drawFontFaceDemo() {
//     // Font Demo1
//     // create more fonts at http://oleddisplay.squix.ch/
//     display.setTextAlignment(TEXT_ALIGN_LEFT);
//     display.setFont(ArialMT_Plain_10);
//     display.drawString(0, 0, "Hello world");
//     display.setFont(ArialMT_Plain_16);
//     display.drawString(0, 10, "Hello world");
//     display.setFont(ArialMT_Plain_24);
//     display.drawString(0, 26, "Hello world");
// }

// void drawTextFlowDemo() {
//     display.setFont(ArialMT_Plain_10);
//     display.setTextAlignment(TEXT_ALIGN_LEFT);
//     display.drawStringMaxWidth(0, 0, 128,
//       "Lorem ipsum\n dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore." );
// }

// void drawTextAlignmentDemo() {
//     // Text alignment demo
//   display.setFont(ArialMT_Plain_10);

//   // The coordinates define the left starting point of the text
//   display.setTextAlignment(TEXT_ALIGN_LEFT);
//   display.drawString(0, 10, "Left aligned (0,10)");

//   // The coordinates define the center of the text
//   display.setTextAlignment(TEXT_ALIGN_CENTER);
//   display.drawString(64, 22, "Center aligned (64,22)");

//   // The coordinates define the right end of the text
//   display.setTextAlignment(TEXT_ALIGN_RIGHT);
//   display.drawString(128, 33, "Right aligned (128,33)");
// }

// void drawRectDemo() {
//       // Draw a pixel at given position
//     for (int i = 0; i < 10; i++) {
//       display.setPixel(i, i);
//       display.setPixel(10 - i, i);
//     }
//     display.drawRect(12, 12, 20, 20);

//     // Fill the rectangle
//     display.fillRect(14, 14, 17, 17);

//     // Draw a line horizontally
//     display.drawHorizontalLine(0, 40, 20);

//     // Draw a line horizontally
//     display.drawVerticalLine(40, 0, 20);
// }

// void drawCircleDemo() {
//   for (int i=1; i < 8; i++) {
//     display.setColor(WHITE);
//     display.drawCircle(32, 32, i*3);
//     if (i % 2 == 0) {
//       display.setColor(BLACK);
//     }
//     display.fillCircle(96, 32, 32 - i* 3);
//   }
// }

// void drawProgressBarDemo() {
//   int progress = (counter / 5) % 100;
//   // draw the progress bar
//   display.drawProgressBar(0, 32, 120, 10, progress);

//   // draw the percentage as String
//   display.setTextAlignment(TEXT_ALIGN_CENTER);
//   display.drawString(64, 15, String(progress) + "%");
// }

// void drawImageDemo() {
//     // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
//     // on how to create xbm files
//     display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
// }

// Demo demos[] = {drawFontFaceDemo, drawTextFlowDemo, drawTextAlignmentDemo, drawRectDemo, drawCircleDemo, drawProgressBarDemo, drawImageDemo};
// int demoLength = (sizeof(demos) / sizeof(Demo));
// long timeSinceLastModeSwitch = 0;

// void loop() {
//   // clear the display
//   display.clear();
//   // draw the current demo method
//   demos[demoMode]();

//   display.setTextAlignment(TEXT_ALIGN_RIGHT);
//   display.drawString(10, 128, String(millis()));
//   // write the buffer to the display
//   display.display();

//   if (millis() - timeSinceLastModeSwitch > DEMO_DURATION) {
//     demoMode = (demoMode + 1)  % demoLength;
//     timeSinceLastModeSwitch = millis();
//   }
//   counter++;
//   delay(10);
// }





// //We always have to include the library
// #include "LedControl.h"

// /*
//  Now we need a LedControl to work with.
//  ***** These pin numbers will probably not work with your hardware *****
//  pin 12 is connected to the DataIn 
//  pin 11 is connected to the CLK 
//  pin 10 is connected to LOAD 
//  We have only a single MAX72XX.
//  */
// LedControl lc=LedControl(12,11,10,1);

// /* we always wait a bit between updates of the display */
// unsigned long delaytime=100;

// void setup() {
//   /*
//    The MAX72XX is in power-saving mode on startup,
//    we have to do a wakeup call
//    */
//   lc.shutdown(0,false);
//   /* Set the brightness to a medium values */
//   lc.setIntensity(0,8);
//   /* and clear the display */
//   lc.clearDisplay(0);
// }

// /*
//  This method will display the characters for the
//  word "Arduino" one after the other on the matrix. 
//  (you need at least 5x7 leds to see the whole chars)
//  */
// void writeArduinoOnMatrix() {
//   /* here is the data for the characters */
//   byte a[5]={B01111110,B10001000,B10001000,B10001000,B01111110};
//   byte r[5]={B00111110,B00010000,B00100000,B00100000,B00010000};
//   byte d[5]={B00011100,B00100010,B00100010,B00010010,B11111110};
//   byte u[5]={B00111100,B00000010,B00000010,B00000100,B00111110};
//   byte i[5]={B00000000,B00100010,B10111110,B00000010,B00000000};
//   byte n[5]={B00111110,B00010000,B00100000,B00100000,B00011110};
//   byte o[5]={B00011100,B00100010,B00100010,B00100010,B00011100};

//   /* now display them one by one with a small delay */
//   lc.setRow(0,0,a[0]);
//   lc.setRow(0,1,a[1]);
//   lc.setRow(0,2,a[2]);
//   lc.setRow(0,3,a[3]);
//   lc.setRow(0,4,a[4]);
//   delay(delaytime);
//   lc.setRow(0,0,r[0]);
//   lc.setRow(0,1,r[1]);
//   lc.setRow(0,2,r[2]);
//   lc.setRow(0,3,r[3]);
//   lc.setRow(0,4,r[4]);
//   delay(delaytime);
//   lc.setRow(0,0,d[0]);
//   lc.setRow(0,1,d[1]);
//   lc.setRow(0,2,d[2]);
//   lc.setRow(0,3,d[3]);
//   lc.setRow(0,4,d[4]);
//   delay(delaytime);
//   lc.setRow(0,0,u[0]);
//   lc.setRow(0,1,u[1]);
//   lc.setRow(0,2,u[2]);
//   lc.setRow(0,3,u[3]);
//   lc.setRow(0,4,u[4]);
//   delay(delaytime);
//   lc.setRow(0,0,i[0]);
//   lc.setRow(0,1,i[1]);
//   lc.setRow(0,2,i[2]);
//   lc.setRow(0,3,i[3]);
//   lc.setRow(0,4,i[4]);
//   delay(delaytime);
//   lc.setRow(0,0,n[0]);
//   lc.setRow(0,1,n[1]);
//   lc.setRow(0,2,n[2]);
//   lc.setRow(0,3,n[3]);
//   lc.setRow(0,4,n[4]);
//   delay(delaytime);
//   lc.setRow(0,0,o[0]);
//   lc.setRow(0,1,o[1]);
//   lc.setRow(0,2,o[2]);
//   lc.setRow(0,3,o[3]);
//   lc.setRow(0,4,o[4]);
//   delay(delaytime);
//   lc.setRow(0,0,0);
//   lc.setRow(0,1,0);
//   lc.setRow(0,2,0);
//   lc.setRow(0,3,0);
//   lc.setRow(0,4,0);
//   delay(delaytime);
// }

// /*
//   This function lights up a some Leds in a row.
//  The pattern will be repeated on every row.
//  The pattern will blink along with the row-number.
//  row number 4 (index==3) will blink 4 times etc.
//  */
// void rows() {
//   for(int row=0;row<8;row++) {
//     delay(delaytime);
//     lc.setRow(0,row,B10100000);
//     delay(delaytime);
//     lc.setRow(0,row,(byte)0);
//     for(int i=0;i<row;i++) {
//       delay(delaytime);
//       lc.setRow(0,row,B10100000);
//       delay(delaytime);
//       lc.setRow(0,row,(byte)0);
//     }
//   }
// }

// /*
//   This function lights up a some Leds in a column.
//  The pattern will be repeated on every column.
//  The pattern will blink along with the column-number.
//  column number 4 (index==3) will blink 4 times etc.
//  */
// void columns() {
//   for(int col=0;col<8;col++) {
//     delay(delaytime);
//     lc.setColumn(0,col,B10100000);
//     delay(delaytime);
//     lc.setColumn(0,col,(byte)0);
//     for(int i=0;i<col;i++) {
//       delay(delaytime);
//       lc.setColumn(0,col,B10100000);
//       delay(delaytime);
//       lc.setColumn(0,col,(byte)0);
//     }
//   }
// }

// /* 
//  This function will light up every Led on the matrix.
//  The led will blink along with the row-number.
//  row number 4 (index==3) will blink 4 times etc.
//  */
// void single() {
//   for(int row=0;row<8;row++) {
//     for(int col=0;col<8;col++) {
//       delay(delaytime);
//       lc.setLed(0,row,col,true);
//       delay(delaytime);
//       for(int i=0;i<col;i++) {
//         lc.setLed(0,row,col,false);
//         delay(delaytime);
//         lc.setLed(0,row,col,true);
//         delay(delaytime);
//       }
//     }
//   }
// }

// void loop() { 
//   writeArduinoOnMatrix();
//   rows();
//   columns();
//   single();
// }