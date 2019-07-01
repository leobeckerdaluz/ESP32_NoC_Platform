#include <stdio.h>
#include "defines.h"

// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`

// Initialize the OLED display using Wire library
// D5 -> SCL
// D3 -> RX0
SSD1306Wire  display(0x3c, 3, 5);


void SetDisplayMessage(int source_id, int target_id, String message, String datetime) {
	// Initialising the UI will init the display too.
    display.init();
    display.flipScreenVertically();

	String source = String(source_id);
	String target = String(target_id);
	String time = datetime.substring(datetime.indexOf('-')+2, datetime.length());

	// create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawString(64, 0, "src: " + source + "      trg: " + target);
    display.drawString(64, 20, time);
    display.drawString(64, 40, message);
    
	// write the buffer to the display
    display.display();
}

void DisplaySizeAndTopologia(int qtde_nodos_X_Y, int topologia) {
    display.init();
    display.flipScreenVertically();
	
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_24);
	
	if (topologia == MESH){
		display.drawString(64, 0, "MESH");
	}
	else if (topologia == ANEL_2D){
		display.drawString(64, 0, "ANEL_2D");
	}
	else if (topologia == ANEL_1D){
		display.drawString(64, 0, "ANEL_1D");
	}

	if (qtde_nodos_X_Y == 2){
		display.drawString(64, 30, "2x2");
	}
	else if (qtde_nodos_X_Y == 3){
		display.drawString(64, 30, "3x3");
	}
	else {
		display.drawString(64, 30, String(qtde_nodos_X_Y)+"x1");
	}

	// write the buffer to the display
    display.display();
}

void DisplayLoading(){
	display.init();
    display.flipScreenVertically();
	
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.setFont(ArialMT_Plain_16);

	display.drawString(64, 20, "Inicializando...");

	// write the buffer to the display
    display.display();
}

void ClearDisplay(){
	// Initialising the UI will init the display too.
    display.init();
    display.flipScreenVertically();
	
	// clear the display
    display.clear();
	// write the buffer to the display
    display.display();
}

// void drawImageDemo() {
//     // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
//     // on how to create xbm files
//     display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
// }