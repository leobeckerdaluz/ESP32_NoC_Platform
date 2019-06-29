#include <iostream>
#include <stdlib.h>
#include <thread>
#include <vector>
#include "../include/defines.h"
#include <stdio.h>
// derlei
#include <unistd.h>

enum all_topologies { ANEL_1D, ANEL_2D, MESH };
enum all_directions { LEFT, RIGHT, TOP, BOTTOM, ANY, ARRIVED };

void get_direction(int current_id, int target, int *distance, int *direction, int topologia_id, int qtd_nodos_X_Y);
void get_XY(int id, int *id_X, int *id_Y, int qtd_nodos_X_Y);
int getID_by_XY(int x_source, int y_source, int qtd_nodos_X_Y);
int get_next_by_dir(int id, int direction_mode, int topologia_local, int qtde_nodos_X_Y);


// ---------------------------------------------------------------------------
// Método que executa a comunicação
void run_PEs(int source, int target, int message_id, int topologia, int qtde_nodos_X_Y){
    int current_id = source;
    while(current_id != target) {


        usleep(1000000);
        printf("PAROOOOOOOOOOUUUUUUUUUUUUUUUU\n");


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
        else if (direction == RIGHT) {
            // Vai para direita
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições à direita!\n", current_id, message_id, target, distance);
            // Obtém o id do router da direita
            int right = get_next_by_dir(current_id, RIGHT, topologia, qtde_nodos_X_Y);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", current_id, message_id, right);
            current_id = right;
        }
        else if (direction == TOP) {
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


        // TOPOLOGIA_ANEL_1D
        else if (direction == ARRIVED && topologia == ANEL_1D) {
            printf("Router%d -> R&A -> A msg %d em trânsito chegou no destino em [%d]!\n", current_id, message_id, target);
        }
        else if (direction == ANY && topologia == ANEL_1D) {
            // Tanto faz
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Está à %d posições tanto à direita como à esquerda. Enviando para a direita!\n", current_id, message_id, target, distance);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", current_id, message_id, right);
            current_id = right;
        }
    }
}


// ---------------------------------------------------------------------------
// Método que obtém a distância e a direção em relação à um target.
void get_direction(int current_id, int target, int *distance, int *direction, int topologia_id, int qtde_nodos_X_Y){
    if (topologia_id == ANEL_1D){
        // Obtém a metade do vetor
        int halfVector = qtde_nodos_X_Y/2;
        // Obtém o módulo da distância
        int abs_dist = abs(target - current_id);

        // Distância é 0, ou seja, já chegou no destino
        if(abs_dist == 0) {
            *direction = ARRIVED;
            *distance = 0;
            return;
        }
        // Distância é menor que a metade do vetor
        else if(abs_dist < halfVector){
            *direction = target < current_id ? LEFT : RIGHT;
            *distance = abs_dist;
        }
        // Distância é maior que a metade do vetor
        else if(abs_dist > halfVector){
            *direction = target < current_id ? RIGHT : LEFT;
            *distance = qtde_nodos_X_Y - abs_dist;
        }
        // Distância é igual à metade do vetor, então tanto faz
        else{
            *direction = ANY;
            *distance = halfVector;
        }
    }
    else{
        int current_X;
        int current_Y;
        get_XY(current_id, &current_X, &current_Y, qtde_nodos_X_Y);

        int target_X;
        int target_Y;
        get_XY(target, &target_X, &target_Y, qtde_nodos_X_Y);

        printf("target: %d, targetX: %d, targetY: %d\n", target, target_X, target_Y);
        printf("current: %d, currentX: %d, currentY: %d\n", current_id, current_X, current_Y);

        // TOPOLOGIA_MESH
        if (topologia_id == MESH){
            // Obtém a distância e a direção para X
            int distance_linha =  (target_X - current_X);       
            int direction_linha;
            if(distance_linha > 0)
                direction_linha = RIGHT;   // direita
            else if(distance_linha < 0)
                direction_linha = LEFT;   // esquerda
            else
                direction_linha = ARRIVED;   // chegou na coluna do target.

            // Obtém a distância e a direção para Y
            int distance_coluna =  (target_Y - current_Y);      
            int direction_coluna;
            if(distance_coluna > 0)
                direction_coluna = TOP;  // cima
            else if(distance_coluna < 0)
                direction_coluna = BOTTOM;  // baixo
            else
                direction_coluna = ARRIVED;  // chegou na linha do target.        
            
            // Se coluna e linha chegaram no destino, envia a informação que acabou
            if (direction_linha == ARRIVED && direction_coluna == ARRIVED){
                *distance = 0;
                *direction = ARRIVED;
            }
            // Enquanto a linha não chegou no destino, envia somente a informação dela
            else if (abs(distance_linha) > 0){
                *distance = abs(distance_linha);
                *direction = direction_linha;
            }
            // Enquanto a coluna não chegou no destino, envia somente a informação dela
            else if (abs(distance_coluna) >= 0) {
                *distance = abs(distance_coluna);
                *direction = direction_coluna;
            }

            printf("[%d][%d] to [%d][%d] | distance:%d | direction:%d\n", current_Y, current_X, target_Y, target_X, distance, direction);
        }
        // TOPOLOGIA_ANEL_2D
        else if (topologia_id == ANEL_2D) {
            int halfVector = qtde_nodos_X_Y/2;

            // Obtém a distância e a direção para X
            int distance_linha =  abs(target_X - current_X);  
            int direction_linha; 
            if(distance_linha == 0)  {
                direction_linha = ARRIVED;
            }  
            else if(distance_linha >= halfVector){
                direction_linha = target_X < current_X ? RIGHT : LEFT;
                distance_linha = qtde_nodos_X_Y - distance_linha;
            }
            else if(distance_linha < halfVector) {
                direction_linha = target_X < current_X ? LEFT : RIGHT;
            }

            // Obtém a distância e a direção para Y
            int distance_coluna =  abs(target_Y - current_Y);  
            int direction_coluna; 
            if(distance_coluna == 0) {
                direction_coluna = ARRIVED;
            }  
            else if(distance_coluna >= halfVector) {
                direction_coluna = (target_Y < current_Y) ? TOP : BOTTOM;
                distance_coluna = qtde_nodos_X_Y - distance_coluna;
            }
            else if(distance_coluna < halfVector) {
                direction_coluna = (target_Y < current_Y) ? BOTTOM : TOP;
            }


            // Se coluna e linha chegaram no destino, envia a informação que acabou
            if (direction_linha == ARRIVED && direction_coluna == ARRIVED){
                *distance = 0;
                *direction = ARRIVED;
            }
            // Enquanto a linha não chegou no destino, envia somente a informação dela
            else if (abs(distance_linha) > 0){
                *distance = abs(distance_linha);
                *direction = direction_linha;
            }
            // Enquanto a coluna não chegou no destino, envia somente a informação dela
            else if (abs(distance_coluna) >= 0) {
                *distance = abs(distance_coluna);
                *direction = direction_coluna;
            }

            printf("[%d][%d] to [%d][%d] | distance:%d | direction:%d\n", current_Y, current_X, target_Y, target_X, distance, direction);
        }
    }
}


// ---------------------------------------------------------------------------
// Método que obtém os valores de X e Y de um id
void get_XY(int id, int *id_X, int *id_Y, int qtde_nodos_X_Y){
    *id_Y = id/qtde_nodos_X_Y;
    *id_X = id%qtde_nodos_X_Y;
}


// ---------------------------------------------------------------------------
// Método que obtém o id a partir de valores de X e Y
int getID_by_XY(int x_source, int y_source, int qtde_nodos_X_Y){
    int parteY = x_source*qtde_nodos_X_Y;
    int parteX = y_source;

    return parteX + parteY;
}


// ---------------------------------------------------------------------------
// Método que obtém o próximo id com base na direção desejada
int get_next_by_dir(int id, int direction_mode, int topologia_local, int qtde_nodos_X_Y) {
    int left, right, top, bottom;
    left = right = top = bottom = BIG_NUMBER;

    if (topologia_local == ANEL_1D) {
        left = (id==0) ? (qtde_nodos_X_Y * qtde_nodos_X_Y) - 1 : id-1;
        right = (id == (qtde_nodos_X_Y*qtde_nodos_X_Y) - 1) ? 0 : id+1;
    }
    else if (topologia_local == ANEL_2D) {
        left = (id%qtde_nodos_X_Y == 0) ? id + qtde_nodos_X_Y - 1 : id-1;
        right = (id%qtde_nodos_X_Y == qtde_nodos_X_Y - 1) ? id-qtde_nodos_X_Y + 1 : id+1;
        top = (id >= qtde_nodos_X_Y*(qtde_nodos_X_Y - 1)) ? id - (qtde_nodos_X_Y*qtde_nodos_X_Y) + qtde_nodos_X_Y : id + qtde_nodos_X_Y;
        bottom = (id < qtde_nodos_X_Y) ? (qtde_nodos_X_Y*qtde_nodos_X_Y) - qtde_nodos_X_Y + id : id - qtde_nodos_X_Y;
    }
    else if (topologia_local == MESH) {
        left = (id%qtde_nodos_X_Y == 0) ? BIG_NUMBER : id-1;
        right = (id%qtde_nodos_X_Y == qtde_nodos_X_Y - 1) ? BIG_NUMBER : id+1;
        top = (id >= qtde_nodos_X_Y*(qtde_nodos_X_Y - 1)) ? BIG_NUMBER : id + qtde_nodos_X_Y;
        bottom = (id < qtde_nodos_X_Y) ? BIG_NUMBER : id - qtde_nodos_X_Y;
    }

    if (direction_mode == LEFT)
        return left;
    else if (direction_mode == RIGHT)
        return right;
    else if (direction_mode == TOP)
        return top;
    else if (direction_mode == BOTTOM)
        return bottom;
    else 
        return BIG_NUMBER;
}