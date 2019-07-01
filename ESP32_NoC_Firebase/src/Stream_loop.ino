//Required HTTPClientESP32Ex library to be installed  https://github.com/mobizt/HTTPClientESP32Ex

#include <WiFi.h>
#include "FirebaseESP32.h"
#include <ArduinoJson.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "defines.h"

//Define Firebase Data object
FirebaseData firebaseData;

// Variáveis globais configuradas pelo Firebase
int firebase_qtde_nodos_X_Y = 3;
int firebase_topologia = MESH;

// ---------------------------------------------------------------------------
// Método que executa a comunicação
void run_PEs(int source, int target, int message_id, int topologia, int qtde_nodos_X_Y){
    bool ArrivedResult = 1; 
    
    // ShowMessageDisplay(source, target, message_id);

    int current_id = source;
    while(current_id != target) {
        here:
        
        printf("----------------------------------------------------------------------------\n");
        
        ShowCurrentID(current_id, target, message_id, qtde_nodos_X_Y);

        int direction;
        int distance;
        
        // Obtém a direção e a distância do próximo router
        get_direction(current_id, target, &distance, &direction, topologia, qtde_nodos_X_Y);
        printf("[%d] to [%d] | distance:%d | direction:%d\n", current_id, target, distance, direction);

        // Verifica a direção retornada
        if(direction == LEFT) {
            // Vai para esquerda
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições à esquerda!\n", current_id, message_id, target, distance);
            // Obtém o id do router da esquerda
            int left = get_next_by_dir(current_id, LEFT, topologia, qtde_nodos_X_Y);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", current_id, message_id, left);
            current_id = left;
        }
        else if ((direction == RIGHT) || (direction == ANY_X)) {
            // Vai para direita
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. ", current_id, message_id, target);
            if (direction == ANY_X)
                printf("Está à %d posições pra esquerda ou direita. Enviando para a direita!\n", distance);
            else
                printf("Ainda precisa andar %d posições à direita!\n", distance);
            // Obtém o id do router da direita
            int right = get_next_by_dir(current_id, RIGHT, topologia, qtde_nodos_X_Y);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", current_id, message_id, right);
            current_id = right;
        }
        else if ((direction == TOP) || (direction == ANY_Y)) {
            // Vai para cima
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições pra cima!\n", current_id, message_id, target, distance);
            // Obtém o id do router de cima
            int top = get_next_by_dir(current_id, TOP, topologia, qtde_nodos_X_Y);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", current_id, message_id, top);
            current_id = top;
        }
        else if (direction == BOTTOM) {
            // Vai para baixo
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições pra baixo!\n", current_id, message_id, target, distance);
            // Obtém o id do router de cima
            int bottom = get_next_by_dir(current_id, BOTTOM, topologia, qtde_nodos_X_Y);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", current_id, message_id, bottom);
            current_id = bottom;
        }
        else if (direction == ARRIVED) {
            printf("Router%d -> R&A -> A msg %d em trânsito chegou no destino em [%d]!\n", current_id, message_id, target);
        }
    }
    
    if (ArrivedResult == 1){
        ArrivedResult = 0;
        goto here;
    }
}


void setup() {
	// Inicializa a serial
	Serial.begin(115200);
	Serial.println();
	Serial.println();

	// Mostra uma mensagem de espera
	DisplayLoading();
	Serial.println("-----------------------------------------------");
	Serial.println("---> Diplay OLED inicializado!");
	Serial.println("-----------------------------------------------");

	// Inicializa o WiFi
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	Serial.print("Connecting to Wi-Fi");
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(300);
	}
	Serial.println();
	Serial.print("Connected with IP: ");
	Serial.println(WiFi.localIP());
	Serial.println();

	// Inicializa conexão com o Firebase
	Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
	Firebase.reconnectWiFi(true);

	// Inicializa o streaming
	Serial.println("-----------------------------------------------");
	Serial.println("Begin stream connection...");
	if (!Firebase.beginStream(firebaseData, "/")) {
		Serial.println("FAILED STREAM!");
		Serial.println("REASON: " + firebaseData.errorReason() + "\n");
	}


	String FirebaseSetupPath = "NoC_Setup/data";

	// Obtém os dados de configuração do Firebase
	if (Firebase.pathExist(firebaseData, FirebaseSetupPath)) {
		if (Firebase.getJSON(firebaseData, FirebaseSetupPath)){
			Serial.println("Configuração obtida do Firebase:");
			String json = firebaseData.jsonData();
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

			String topologia = doc["topologia"];
			int x_y = doc["x_y"];
			// ---------------------------------

			Serial.println("Topologia: " + topologia);
			Serial.println("X_Y: " + String(x_y));
			Serial.println("-----------------------------------------------");
			
			// Atualiza a topologia local
			if (topologia == "MESH")
				firebase_topologia = MESH;
			else if (topologia == "ANEL_2D")
				firebase_topologia = ANEL_2D;
			else if (topologia == "ANEL_1D")
				firebase_topologia = ANEL_1D;

			firebase_qtde_nodos_X_Y = x_y;
			
			// Mostra os dados de topologia e tamanho da NoC
			DisplaySizeAndTopologia(firebase_qtde_nodos_X_Y, firebase_topologia);
		}
	}
	else {
		Serial.println("Path " + FirebaseSetupPath + " não existe!");
	}

	// Inicializa o MAX7219
	StartAll(firebase_qtde_nodos_X_Y);
	Serial.println("-----------------------------------------------");
	Serial.println("---> MAX7219 inicializado!");
	Serial.println("-----------------------------------------------");

	
}

void loop() {
	if (!Firebase.readStream(firebaseData)) {
		Serial.println("-----------------------------------------------");
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

		// Se o evento foi uma configuração
		if (event_path == "/NoC_Setup"){
			Serial.println("Configuração obtida do Firebase:");
			String json = firebaseData.jsonData();
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

			String topologia = doc["data"]["topologia"];
			int x_y = doc["data"]["x_y"];
			// ---------------------------------

			Serial.println("Topologia: " + topologia);
			Serial.println("X_Y: " + String(x_y));
			Serial.println("-----------------------------------------------");
			
			if (topologia == "MESH")
				firebase_topologia = MESH;
			else if (topologia == "ANEL_2D")
				firebase_topologia = ANEL_2D;
			else if (topologia == "ANEL_1D")
				firebase_topologia = ANEL_1D;

			firebase_qtde_nodos_X_Y = x_y;

			// Mostra a topologia e o tamanho no OLED
			DisplaySizeAndTopologia(firebase_qtde_nodos_X_Y, firebase_topologia);
			// Pisca todos os elementos dessa topologia na matriz
			BlinkXY(firebase_qtde_nodos_X_Y);
			
			Serial.println("X_Y e Topologia alterados com sucesso!");
			Serial.println("X_Y: " + String(firebase_qtde_nodos_X_Y));
			Serial.println("Topologia: " + firebase_topologia);
		}
		// Se o evento foi uma mensagem
		else if(event_path.indexOf("/Messages") >= 0){
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
			int topologia = firebase_topologia;

			// Run PEs task
			SetDisplayMessage(source, target, message, date);
			run_PEs(source, target, message_id, topologia, firebase_qtde_nodos_X_Y);
			// ClearDisplay();
		}

		Serial.println();
	}
}