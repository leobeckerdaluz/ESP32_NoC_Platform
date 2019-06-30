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


void writePE(int id, int qtde_nodos_X_Y, int state){
    if (qtde_nodos_X_Y == 3){
        switch (id) {
            case 0:
                lc.setLed(0, 0, 0, state);
                lc.setLed(0, 0, 1, state);
                lc.setLed(0, 1, 0, state);
                lc.setLed(0, 1, 1, state);
                #if DEBUG_CONSOLE1
                printf("Ligando PE0 na 3x3\n");
                #endif
                break;
            case 1:
                lc.setLed(0, 0, 3, state);
                lc.setLed(0, 0, 4, state);
                lc.setLed(0, 1, 3, state);
                lc.setLed(0, 1, 4, state);
                #if DEBUG_CONSOLE1
                printf("Ligando PE1 na 3x3\n");
                #endif
                break;
            case 2:
                lc.setLed(0, 0, 6, state);
                lc.setLed(0, 0, 7, state);
                lc.setLed(0, 1, 6, state);
                lc.setLed(0, 1, 7, state);
                #if DEBUG_CONSOLE1
                printf("Ligando PE2 na 3x3\n");
                #endif
                break;
            case 3:
                lc.setLed(0, 3, 0, state);
                lc.setLed(0, 3, 1, state);
                lc.setLed(0, 4, 0, state);
                lc.setLed(0, 4, 1, state);
                #if DEBUG_CONSOLE1
                printf("Ligando PE3 na 3x3\n");
                #endif
                break;
            case 4:
                lc.setLed(0, 3, 3, state);
                lc.setLed(0, 3, 4, state);
                lc.setLed(0, 4, 3, state);
                lc.setLed(0, 4, 4, state);
                #if DEBUG_CONSOLE1
                printf("Ligando PE4 na 3x3\n");
                #endif
                break;
            case 5:
                lc.setLed(0, 3, 6, state);
                lc.setLed(0, 3, 7, state);
                lc.setLed(0, 4, 6, state);
                lc.setLed(0, 4, 7, state);
                #if DEBUG_CONSOLE1
                printf("Ligando PE5 na 3x3\n");
                #endif
                break;
            case 6:
                lc.setLed(0, 6, 0, state);
                lc.setLed(0, 6, 1, state);
                lc.setLed(0, 7, 0, state);
                lc.setLed(0, 7, 1, state);
                #if DEBUG_CONSOLE1
                printf("Ligando PE6 na 3x3\n");
                #endif
                break;
            case 7:
                lc.setLed(0, 6, 3, state);
                lc.setLed(0, 6, 4, state);
                lc.setLed(0, 7, 3, state);
                lc.setLed(0, 7, 4, state);
                #if DEBUG_CONSOLE1
                printf("Ligando PE7 na 3x3\n");
                #endif
                break;
            case 8:
                lc.setLed(0, 6, 6, state);
                lc.setLed(0, 6, 7, state);
                lc.setLed(0, 7, 6, state);
                lc.setLed(0, 7, 7, state);
                #if DEBUG_CONSOLE1
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
                lc.setLed(0, 0, 0, state);
                lc.setLed(0, 0, 1, state);
                lc.setLed(0, 0, 2, state);
                lc.setLed(0, 1, 0, state);
                lc.setLed(0, 1, 1, state);
                lc.setLed(0, 1, 2, state);
                lc.setLed(0, 2, 0, state);
                lc.setLed(0, 2, 1, state);
                lc.setLed(0, 2, 2, state);
                #if DEBUG_CONSOLE1
                printf("Ligando PE0 na 2x2\n");
                #endif
                break;
            case 1:
                lc.setLed(0, 0, 5, state);
                lc.setLed(0, 0, 6, state);
                lc.setLed(0, 0, 7, state);
                lc.setLed(0, 1, 5, state);
                lc.setLed(0, 1, 6, state);
                lc.setLed(0, 1, 7, state);
                lc.setLed(0, 2, 5, state);
                lc.setLed(0, 2, 6, state);
                lc.setLed(0, 2, 7, state);
                #if DEBUG_CONSOLE1
                printf("Ligando PE1 na 2x2\n");
                #endif
                break;
            case 2:
                lc.setLed(0, 5, 0, state);
                lc.setLed(0, 5, 1, state);
                lc.setLed(0, 5, 2, state);
                lc.setLed(0, 6, 0, state);
                lc.setLed(0, 6, 1, state);
                lc.setLed(0, 6, 2, state);
                lc.setLed(0, 7, 0, state);
                lc.setLed(0, 7, 1, state);
                lc.setLed(0, 7, 2, state);
                #if DEBUG_CONSOLE1
                printf("Ligando PE2 na 2x2\n");
                #endif
                break;
            case 3:
                lc.setLed(0, 5, 5, state);
                lc.setLed(0, 5, 6, state);
                lc.setLed(0, 5, 7, state);
                lc.setLed(0, 6, 5, state);
                lc.setLed(0, 6, 6, state);
                lc.setLed(0, 6, 7, state);
                lc.setLed(0, 7, 5, state);
                lc.setLed(0, 7, 6, state);
                lc.setLed(0, 7, 7, state);
                #if DEBUG_CONSOLE1
                printf("Ligando PE3 na 2x2\n");
                #endif
                break;

            default:
                break;
        }
    }
    else{
        switch (id) {
            case 0:
                printf("Ligando PE0 na ANEL_1D\n");
                break;
            case 1:
                printf("Ligando PE1 na ANEL_1D\n");
                break;
            case 2:
                printf("Ligando PE2 na ANEL_1D\n");
                break;
            case 3:
                printf("Ligando PE3 na ANEL_1D\n");
                break;
            case 4:
                printf("Ligando PE4 na ANEL_1D\n");
                break;
            case 5:
                printf("Ligando PE4 na ANEL_1D\n");
                break;
            case 6:
                printf("Ligando PE4 na ANEL_1D\n");
                break;
            case 7:
                printf("Ligando PE4 na ANEL_1D\n");
                break;

            default:
                printf("Ligando tudo na ANEL_1D\n");
                break;
        }
    }   
}

// derlei
// #include <unistd.h>

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
        delay(100);
        for (int id=0; id<qtde_nodos_X_Y*qtde_nodos_X_Y; id++)
            writePE(id, qtde_nodos_X_Y, false);
        delay(100);
	    
        lc.setIntensity(0,intensity);  // Set intensity levels
    }
}

void BlinkXY(int qtde_nodos_X_Y){
    // Inicializa o MAX7219
	lc.shutdown(0,false);  // Wake up displays
	lc.setIntensity(0,8);  // Set intensity levels
	lc.clearDisplay(0);  // Clear Displays

    for (int blink=0; blink<4; blink++){
        for (int id=0; id<qtde_nodos_X_Y*qtde_nodos_X_Y; id++)
            writePE(id, qtde_nodos_X_Y, true);
        delay(200);
        for (int id=0; id<qtde_nodos_X_Y*qtde_nodos_X_Y; id++)
            writePE(id, qtde_nodos_X_Y, false);
        delay(200);
    }
}

void ShowCurrentID(int current_id, int target, int message_id, int qtde_nodos_X_Y){
    // Inicializa o MAX7219
	lc.shutdown(0,false);  // Wake up displays
	lc.setIntensity(0,8);  // Set intensity levels
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
