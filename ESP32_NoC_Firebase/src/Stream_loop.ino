// //Required HTTPClientESP32Ex library to be installed  https://github.com/mobizt/HTTPClientESP32Ex

// #include <WiFi.h>
// #include "FirebaseESP32.h"
// #include <ArduinoJson.h>
// #include <stdio.h>
// #include <iostream>
// #include <stdlib.h>
// #include "defines.h"

// //Define Firebase Data object
// FirebaseData firebaseData;

// // Variáveis globais configuradas pelo Firebase
// int firebase_qtde_nodos_X_Y = 3;
// int firebase_topologia = MESH;

// // ---------------------------------------------------------------------------
// // Método que executa a comunicação
// void run_PEs(int source, int target, int message_id, int topologia, int qtde_nodos_X_Y){
//     bool ArrivedResult = 1; 
    
//     // ShowMessageDisplay(source, target, message_id);

//     int current_id = source;
//     while(current_id != target) {
//         here:
        
//         printf("----------------------------------------------------------------------------\n");
        
//         ShowCurrentID(current_id, target, message_id, qtde_nodos_X_Y);

//         int direction;
//         int distance;
        
//         // Obtém a direção e a distância do próximo router
//         get_direction(current_id, target, &distance, &direction, topologia, qtde_nodos_X_Y);
//         printf("[%d] to [%d] | distance:%d | direction:%d\n", current_id, target, distance, direction);

//         // Verifica a direção retornada
//         if(direction == LEFT) {
//             // Vai para esquerda
//             printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições à esquerda!\n", current_id, message_id, target, distance);
//             // Obtém o id do router da esquerda
//             int left = get_next_by_dir(current_id, LEFT, topologia, qtde_nodos_X_Y);
//             printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", current_id, message_id, left);
//             current_id = left;
//         }
//         else if ((direction == RIGHT) || (direction == ANY_X)) {
//             // Vai para direita
//             printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. ", current_id, message_id, target);
//             if (direction == ANY_X)
//                 printf("Está à %d posições pra esquerda ou direita. Enviando para a direita!\n", distance);
//             else
//                 printf("Ainda precisa andar %d posições à direita!\n", distance);
//             // Obtém o id do router da direita
//             int right = get_next_by_dir(current_id, RIGHT, topologia, qtde_nodos_X_Y);
//             printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", current_id, message_id, right);
//             current_id = right;
//         }
//         else if ((direction == TOP) || (direction == ANY_Y)) {
//             // Vai para cima
//             printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições pra cima!\n", current_id, message_id, target, distance);
//             // Obtém o id do router de cima
//             int top = get_next_by_dir(current_id, TOP, topologia, qtde_nodos_X_Y);
//             printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", current_id, message_id, top);
//             current_id = top;
//         }
//         else if (direction == BOTTOM) {
//             // Vai para baixo
//             printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições pra baixo!\n", current_id, message_id, target, distance);
//             // Obtém o id do router de cima
//             int bottom = get_next_by_dir(current_id, BOTTOM, topologia, qtde_nodos_X_Y);
//             printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", current_id, message_id, bottom);
//             current_id = bottom;
//         }
//         else if (direction == ARRIVED) {
//             printf("Router%d -> R&A -> A msg %d em trânsito chegou no destino em [%d]!\n", current_id, message_id, target);
//         }
//     }
    
//     if (ArrivedResult == 1){
//         ArrivedResult = 0;
//         goto here;
//     }
// }


// void setup() {
// 	// Inicializa a serial
// 	Serial.begin(115200);
// 	Serial.println();
// 	Serial.println();

// 	// Inicializa o WiFi
// 	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
// 	Serial.print("Connecting to Wi-Fi");
// 	while (WiFi.status() != WL_CONNECTED) {
// 		Serial.print(".");
// 		delay(300);
// 	}
// 	Serial.println();
// 	Serial.print("Connected with IP: ");
// 	Serial.println(WiFi.localIP());
// 	Serial.println();

// 	Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
// 	Firebase.reconnectWiFi(true);

// 	Serial.println("------------------------------------");
// 	Serial.println("Begin stream connection...");

// 	if (!Firebase.beginStream(firebaseData, "/"))
// 	{
// 		Serial.println("FAILED");
// 		Serial.println("REASON: " + firebaseData.errorReason());
// 		Serial.println();
// 	}

// 	StartAll(firebase_qtde_nodos_X_Y);
// 	Serial.println("---------------------");
// 	Serial.println("MAX7219 inicializado!");
// 	Serial.println("---------------------");
// }

// void loop() {
// 	if (!Firebase.readStream(firebaseData)) {
// 		Serial.println("------------------------------------");
// 		Serial.println("Read stream");
// 		Serial.println("FAILED");
// 		Serial.println("REASON: " + firebaseData.errorReason());
// 		Serial.println();
// 	}

// 	if (firebaseData.streamTimeout()) {
// 		Serial.println("Stream timeout, resume streaming...");
// 		Serial.println();
// 	}

// 	if (firebaseData.streamAvailable()) {
// 		String event_path = firebaseData.dataPath();
// 		Serial.println("Stream Data available...");
// 		Serial.println("STREAM PATH: " + firebaseData.streamPath());
// 		Serial.println("EVENT PATH: " + event_path);
// 		Serial.println("DATA TYPE: " + firebaseData.dataType());
// 		Serial.println("EVENT TYPE: " + firebaseData.eventType());

// 		if (event_path == "/NoC_Setup/data/x_y"){
// 			int new_X_Y = firebaseData.intData();
// 			firebase_qtde_nodos_X_Y = new_X_Y;
// 			Serial.println("X_Y alterado com sucesso!");
// 			Serial.println(new_X_Y);

// 			BlinkXY(new_X_Y);
// 		}
// 		else if (event_path == "/NoC_Setup/data/topologia"){
// 			String event_topologia = firebaseData.stringData();

// 			if (event_topologia == "MESH"){
// 				firebase_topologia = MESH;
// 			}
// 			else if (event_topologia == "ANEL_1D"){
// 				firebase_topologia = ANEL_1D;
// 			}
// 			else if (event_topologia == "ANEL_2D"){
// 				firebase_topologia = ANEL_2D;
// 			}

// 			Serial.println("Topologia alterado com sucesso!");
// 		}
// 		else if(event_path.indexOf("/Messages") >= 0){
// 			String json = firebaseData.jsonData();

// 			Serial.println("sucesso!");
// 			Serial.println(json);

// 			// ---------- Deserialize ---------- 
// 			StaticJsonDocument<200> doc;

// 			// Deserialize the JSON document
// 			DeserializationError error = deserializeJson(doc, json);

// 			// Test if parsing succeeds.
// 			if (error) {
// 			Serial.print(F("deserializeJson() failed: "));
// 			Serial.println(error.c_str());
// 			return;
// 			}

// 			String message = doc["data"]["msg"];
// 			String date = doc["data"]["date"];
// 			int source = doc["data"]["src"];
// 			int target = doc["data"]["target"];
// 			// ---------------------------------

// 			// Print values.
// 			Serial.println(source);
// 			Serial.println(target);
// 			Serial.println(message);
// 			Serial.println(date);

// 			// Message Parameters
// 			// int target = 5;
// 			// int source = 1;
// 			int message_id = 12345;

// 			// Setup parameters
// 			int topologia = firebase_topologia;

// 			// Run PEs task
// 			run_PEs(source, target, message_id, topologia, firebase_qtde_nodos_X_Y);
// 		}

// 		Serial.println();
// 	}
// }





// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
// or #include "SH1106Wire.h", legacy include: `#include "SH1106.h"`
// For a connection via I2C using brzo_i2c (must be installed) include
// #include <brzo_i2c.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Brzo.h"
// #include "SH1106Brzo.h"
// For a connection via SPI include
// #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"
// #include "SH1106SPi.h"

// Initialize the OLED display using SPI
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi        display(D0, D2, D8);
// or
// SH1106Spi         display(D0, D2);

// Initialize the OLED display using brzo_i2c
// D3 -> SDA
// D5 -> SCL
// SSD1306Brzo display(0x3c, D3, D5);
// or
// SH1106Brzo  display(0x3c, D3, D5);

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, 3, 5);
// SH1106 display(0x3c, D3, D5);


#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

}

void drawFontFaceDemo() {
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Hello world");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 10, "Hello world");
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 26, "Hello world");
}

void drawTextFlowDemo() {
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawStringMaxWidth(0, 0, 128,
      "Lorem ipsum\n dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore." );
}

void drawTextAlignmentDemo() {
    // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 10, "Left aligned (0,10)");

  // The coordinates define the center of the text
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, "Center aligned (64,22)");

  // The coordinates define the right end of the text
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 33, "Right aligned (128,33)");
}

void drawRectDemo() {
      // Draw a pixel at given position
    for (int i = 0; i < 10; i++) {
      display.setPixel(i, i);
      display.setPixel(10 - i, i);
    }
    display.drawRect(12, 12, 20, 20);

    // Fill the rectangle
    display.fillRect(14, 14, 17, 17);

    // Draw a line horizontally
    display.drawHorizontalLine(0, 40, 20);

    // Draw a line horizontally
    display.drawVerticalLine(40, 0, 20);
}

void drawCircleDemo() {
  for (int i=1; i < 8; i++) {
    display.setColor(WHITE);
    display.drawCircle(32, 32, i*3);
    if (i % 2 == 0) {
      display.setColor(BLACK);
    }
    display.fillCircle(96, 32, 32 - i* 3);
  }
}

void drawProgressBarDemo() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
}

Demo demos[] = {drawFontFaceDemo, drawTextFlowDemo, drawTextAlignmentDemo, drawRectDemo, drawCircleDemo, drawProgressBarDemo};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;

void loop() {
  // clear the display
  display.clear();
  // draw the current demo method
  demos[demoMode]();

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(10, 128, String(millis()));
  // write the buffer to the display
  display.display();

  if (millis() - timeSinceLastModeSwitch > DEMO_DURATION) {
    demoMode = (demoMode + 1)  % demoLength;
    timeSinceLastModeSwitch = millis();
  }
  counter++;
  delay(10);
}

