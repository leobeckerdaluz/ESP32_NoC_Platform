#include <iostream>
#include <stdlib.h>
// #include "../include/defines.h"
#include <stdio.h>
#include "defines.h"

#include <Arduino.h>

#include "LedControl.h"

//// Inicializa o MAX7219
// LedControl lc=LedControl(13,14,2,1);
// LedControl lc=LedControl(23,14,13,1);
// LedControl lc=LedControl(32,33,25,1);
// LedControl lc=LedControl(12,14,15,1);
// LedControl lc=LedControl(23,18,5,1);
LedControl lc = LedControl(13,14,12,1); // MISO, CS, CLK
// LedControl lc=LedControl(12,10,11,1);
// LedControl lc=LedControl(19,5,18,1);
// LedControl lc=LedControl(38,35,34,1);

// 3x3
void setPE0_3x3(bool state){
    lc.setLed(0, 0, 0, state);
    lc.setLed(0, 0, 1, state);
    lc.setLed(0, 1, 0, state);
    lc.setLed(0, 1, 1, state);
}
void setPE1_3x3(bool state){
    lc.setLed(0, 0, 3, state);
    lc.setLed(0, 0, 4, state);
    lc.setLed(0, 1, 3, state);
    lc.setLed(0, 1, 4, state);
}
void setPE2_3x3(bool state){
    lc.setLed(0, 0, 6, state);
    lc.setLed(0, 0, 7, state);
    lc.setLed(0, 1, 6, state);
    lc.setLed(0, 1, 7, state);
}
void setPE3_3x3(bool state){
    lc.setLed(0, 3, 0, state);
    lc.setLed(0, 3, 1, state);
    lc.setLed(0, 4, 0, state);
    lc.setLed(0, 4, 1, state);          
}
void setPE4_3x3(bool state){
    lc.setLed(0, 3, 3, state);
    lc.setLed(0, 3, 4, state);
    lc.setLed(0, 4, 3, state);
    lc.setLed(0, 4, 4, state);
}
void setPE5_3x3(bool state){
    lc.setLed(0, 3, 6, state);
    lc.setLed(0, 3, 7, state);
    lc.setLed(0, 4, 6, state);
    lc.setLed(0, 4, 7, state);
}
void setPE6_3x3(bool state){
    lc.setLed(0, 6, 0, state);
    lc.setLed(0, 6, 1, state);
    lc.setLed(0, 7, 0, state);
    lc.setLed(0, 7, 1, state);
}
void setPE7_3x3(bool state){
    lc.setLed(0, 6, 3, state);
    lc.setLed(0, 6, 4, state);
    lc.setLed(0, 7, 3, state);
    lc.setLed(0, 7, 4, state);
}
void setPE8_3x3(bool state){
    lc.setLed(0, 6, 6, state);
    lc.setLed(0, 6, 7, state);
    lc.setLed(0, 7, 6, state);
    lc.setLed(0, 7, 7, state);
}

// 2x2
void setPE0_2x2(bool state){
    lc.setLed(0, 0, 0, state);
    lc.setLed(0, 0, 1, state);
    lc.setLed(0, 0, 2, state);
    lc.setLed(0, 1, 0, state);
    lc.setLed(0, 1, 1, state);
    lc.setLed(0, 1, 2, state);
    lc.setLed(0, 2, 0, state);
    lc.setLed(0, 2, 1, state);
    lc.setLed(0, 2, 2, state);
}
void setPE1_2x2(bool state){
    lc.setLed(0, 0, 5, state);
    lc.setLed(0, 0, 6, state);
    lc.setLed(0, 0, 7, state);
    lc.setLed(0, 1, 5, state);
    lc.setLed(0, 1, 6, state);
    lc.setLed(0, 1, 7, state);
    lc.setLed(0, 2, 5, state);
    lc.setLed(0, 2, 6, state);
    lc.setLed(0, 2, 7, state);
}
void setPE2_2x2(bool state){
    lc.setLed(0, 5, 0, state);
    lc.setLed(0, 5, 1, state);
    lc.setLed(0, 5, 2, state);
    lc.setLed(0, 6, 0, state);
    lc.setLed(0, 6, 1, state);
    lc.setLed(0, 6, 2, state);
    lc.setLed(0, 7, 0, state);
    lc.setLed(0, 7, 1, state);
    lc.setLed(0, 7, 2, state);
}
void setPE3_2x2(bool state){
    lc.setLed(0, 5, 5, state);
    lc.setLed(0, 5, 6, state);
    lc.setLed(0, 5, 7, state);
    lc.setLed(0, 6, 5, state);
    lc.setLed(0, 6, 6, state);
    lc.setLed(0, 6, 7, state);
    lc.setLed(0, 7, 5, state);
    lc.setLed(0, 7, 6, state);
    lc.setLed(0, 7, 7, state);
}


void writePE(int id, int qtde_nodos_X_Y, bool state){
    if (qtde_nodos_X_Y == 3){
        switch (id) {
            case 0:
                #if ROTATE90_MATRIX
                    setPE6_3x3(state);
                #elif
                    setPE0_3x3(state);
                #endif
                #if DEBUG_CONSOLE
                    printf("Ligando PE0 na 3x3\n");
                #endif
                break;
            case 1:
                #if ROTATE90_MATRIX
                    setPE3_3x3(state);
                #elif
                    setPE1_3x3(state);
                #endif
                #if DEBUG_CONSOLE
                    printf("Ligando PE1 na 3x3\n");
                #endif
                break;
            case 2:
                #if ROTATE90_MATRIX
                    setPE0_3x3(state);
                #elif
                    setPE2_3x3(state);
                #endif
                #if DEBUG_CONSOLE
                    printf("Ligando PE2 na 3x3\n");
                #endif
                break;
            case 3:
                #if ROTATE90_MATRIX
                    setPE7_3x3(state);
                #elif
                    setPE3_3x3(state);
                #endif
                #if DEBUG_CONSOLE
                    printf("Ligando PE3 na 3x3\n");
                #endif
                break;
            case 4:
                #if ROTATE90_MATRIX
                    setPE4_3x3(state);
                #elif
                    setPE4_3x3(state);
                #endif
                #if DEBUG_CONSOLE
                    printf("Ligando PE4 na 3x3\n");
                #endif
                break;
            case 5:
                #if ROTATE90_MATRIX
                    setPE1_3x3(state);
                #elif
                    setPE5_3x3(state);
                #endif
                #if DEBUG_CONSOLE
                    printf("Ligando PE5 na 3x3\n");
                #endif
                break;
            case 6:
                #if ROTATE90_MATRIX
                    setPE8_3x3(state);
                #elif
                    setPE6_3x3(state);
                #endif
                #if DEBUG_CONSOLE
                    printf("Ligando PE6 na 3x3\n");
                #endif
                break;
            case 7:
                #if ROTATE90_MATRIX
                    setPE5_3x3(state);
                #elif
                    setPE7_3x3(state);
                #endif
                #if DEBUG_CONSOLE
                printf("Ligando PE7 na 3x3\n");
                #endif
                break;
            case 8:
                #if ROTATE90_MATRIX
                    setPE2_3x3(state);
                #elif
                    setPE8_3x3(state);
                #endif
                #if DEBUG_CONSOLE
                    printf("Ligando PE8 na 3x3\n");
                #endif
                break;
            default:
                break;
        }
    }
    else if (qtde_nodos_X_Y == 2){
        switch (id) {
            case 0:
                #if ROTATE90_MATRIX
                    setPE2_2x2(state);
                #elif
                    setPE0_2x2(state);
                #endif
                #if DEBUG_CONSOLE
                printf("Ligando PE0 na 2x2\n");
                #endif
                break;
            case 1:
                #if ROTATE90_MATRIX
                    setPE0_2x2(state);
                #elif
                    setPE1_2x2(state);
                #endif
                #if DEBUG_CONSOLE
                    printf("Ligando PE1 na 2x2\n");
                #endif
                break;
            case 2:
                #if ROTATE90_MATRIX
                    setPE3_2x2(state);
                #elif
                    setPE2_2x2(state);
                #endif
                #if DEBUG_CONSOLE
                    printf("Ligando PE2 na 2x2\n");
                #endif
                break;
            case 3:
                #if ROTATE90_MATRIX
                    setPE1_2x2(state);
                #elif
                    setPE3_2x2(state);
                #endif
                #if DEBUG_CONSOLE
                    printf("Ligando PE3 na 2x2\n");
                #endif
                break;
            default:
                break;
        }
    }
    else{
        byte byte_state = B00000000;
        if (state)  byte_state = B11111111;

        #if ROTATE90_MATRIX
            lc.setColumn(0, 0, byte_state);
            lc.setColumn(0, 1, byte_state);
            lc.setColumn(0, 6, byte_state);
            lc.setColumn(0, 7, byte_state);
            lc.setLed(0, qtde_nodos_X_Y-1-id, 3, state);
            lc.setLed(0, qtde_nodos_X_Y-1-id, 4, state);
        #elif
            lc.setRow(0, 0, byte_state);
            lc.setRow(0, 1, byte_state);
            lc.setRow(0, 6, byte_state);
            lc.setRow(0, 7, byte_state);
            lc.setLed(0, 3, id, state);
            lc.setLed(0, 4, id, state);
        #endif

        #if DEBUG_CONSOLE
            printf("Ligando PE0 na ANEL_1D\n");
        #endif
    }   
}


void ShowMessageDisplay(int source, int target, int message_id){
    printf("\n===================================\n");
    printf("===================================\n");
    printf("%d -> %d\n", source, target);
    printf("Mensagem: %d\n", message_id);
    printf("===================================\n");
    printf("===================================\n\n");
}

void StartAll(int qtde_nodos_X_Y){
    // Inicializa o MAX7219
	lc.shutdown(0,false);  // Wake up displays
	lc.setIntensity(0,0);  // Set intensity levels
	lc.clearDisplay(0);  // Clear Displays

    for (int intensity=0; intensity<10; intensity++){
        for (int id=0; id<qtde_nodos_X_Y*qtde_nodos_X_Y; id++)
            writePE(id, qtde_nodos_X_Y, true);
        delay(20);
        for (int id=0; id<qtde_nodos_X_Y*qtde_nodos_X_Y; id++)
            writePE(id, qtde_nodos_X_Y, false);
        delay(20);
	    
        lc.setIntensity(0,intensity);  // Set intensity levels
    }
    for (int intensity=10; intensity>=0; intensity--){
        for (int id=0; id<qtde_nodos_X_Y*qtde_nodos_X_Y; id++)
            writePE(id, qtde_nodos_X_Y, true);
        delay(20);
        for (int id=0; id<qtde_nodos_X_Y*qtde_nodos_X_Y; id++)
            writePE(id, qtde_nodos_X_Y, false);
        delay(20);
	    
        lc.setIntensity(0,intensity);  // Set intensity levels
    }
}

void BlinkXY(int qtde_nodos_X_Y){
    // Inicializa o MAX7219
	lc.shutdown(0,false);  // Wake up displays
	lc.setIntensity(0,10);  // Set intensity levels
	lc.clearDisplay(0);  // Clear Displays

    for (int blink=0; blink<4; blink++){
        for (int id=0; id<qtde_nodos_X_Y*qtde_nodos_X_Y; id++)
            writePE(id, qtde_nodos_X_Y, true);
        delay(100);
        for (int id=0; id<qtde_nodos_X_Y*qtde_nodos_X_Y; id++)
            writePE(id, qtde_nodos_X_Y, false);
        delay(100);
    }
}

void ShowCurrentID(int current_id, int target, int message_id, int qtde_nodos_X_Y){
    // Inicializa o MAX7219
	lc.shutdown(0,false);  // Wake up displays
	lc.setIntensity(0,10);  // Set intensity levels
	lc.clearDisplay(0);  // Clear Displays

    if (current_id == target){
        for (int i=0; i<3; i++){
            printf("===================================\n");
            printf("LIGA LED DO %d\n", current_id);
            
            writePE(current_id, qtde_nodos_X_Y, true);
            delay(300);
            writePE(current_id, qtde_nodos_X_Y, false);
            delay(200);
            
            printf("Mostra no display a mensagem atual %d\n", message_id);
            printf("===================================\n");
        }
    }
    else{
        printf("\n===================================\n");
        printf("LIGA LED DO %d\n", current_id);

        writePE(current_id, qtde_nodos_X_Y, true);
        delay(500);
        writePE(current_id, qtde_nodos_X_Y, false);

        printf("Mostra no display a mensagem atual %d\n", message_id);
        printf("===================================\n\n");
    }
}
