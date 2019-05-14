/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "defines.h"

enum all_topologies { anel_1D, anel_2D, mesh };


int get_next_by_dir(int id, int direction_mode, enum all_topologies topologia_local, int qtde_nodos_X_Y) {
    // -----------------
    // --> direction_mode
    // 0 - left
    // 1 - right
    // 2 - top
    // 3 - bottom
    // -----------------
    // --> id_topologia
    // 0 - anel 1D
    // 1 - Anel 2D
    // 2 - Mesh
    // -----------------

    int left, right, top, bottom;
    left = right = top = bottom = BIG_NUMBER;

    // TOPOLOGIA_ANEL_1D
    if (topologia_local == 0) {
        left = (id==0) ? (qtde_nodos_X_Y*qtde_nodos_X_Y)-1 : id-1;
        right = (id==(qtde_nodos_X_Y*qtde_nodos_X_Y)-1) ? 0 : id+1;
        top = BIG_NUMBER;
        bottom = BIG_NUMBER;
    }

    // TOPOLOGIA_ANEL_2D
    else if (topologia_local == 1) {
        // left = (id == (0 + (int)(id/qtde_nodos_X_Y)*qtde_nodos_X_Y)) ? id+(qtde_nodos_X_Y-1) : id-1;
        left = (id%qtde_nodos_X_Y == 0) ? id+qtde_nodos_X_Y-1 : id-1;
        // right = (id == (2 + (int)(id/qtde_nodos_X_Y)*qtde_nodos_X_Y)) ? id-qtde_nodos_X_Y+1 : id+1;
        right = (id%qtde_nodos_X_Y == qtde_nodos_X_Y-1) ? id-qtde_nodos_X_Y+1 : id+1;
        top = (id >= qtde_nodos_X_Y*(qtde_nodos_X_Y-1)) ? id-(qtde_nodos_X_Y*qtde_nodos_X_Y)+qtde_nodos_X_Y : id + qtde_nodos_X_Y;
        bottom = (id < qtde_nodos_X_Y) ? (qtde_nodos_X_Y*qtde_nodos_X_Y)-qtde_nodos_X_Y+id : id - qtde_nodos_X_Y;
    }

    // TOPOLOGIA_MESH
    // else {
    else if (topologia_local == 2) {
        // left = (id == (0 + (int)(id/qtde_nodos_X_Y)*qtde_nodos_X_Y))) ? BIG_NUMBER : id-1;
        left = (id%qtde_nodos_X_Y == 0) ? BIG_NUMBER : id-1;
        // right = (id == (2 + (int)(id/qtde_nodos_X_Y*qtde_nodos_X_Y))) ? BIG_NUMBER : id+1;
        right = (id%qtde_nodos_X_Y == qtde_nodos_X_Y-1) ? BIG_NUMBER : id+1;
        top = (id >= qtde_nodos_X_Y*(qtde_nodos_X_Y-1)) ? BIG_NUMBER : id + qtde_nodos_X_Y;
        bottom = (id < qtde_nodos_X_Y) ? BIG_NUMBER : id - qtde_nodos_X_Y;
    }

    // Return left
    if (direction_mode == 0)
        return left;

    // Return right
    else if (direction_mode == 1)
        return right;

    // Return top
    else if (direction_mode == 2)
        return top;

    // Return bottom
    else if (direction_mode == 3)
        return bottom;

    else 
        return BIG_NUMBER;
}

void run_PEs(int source, int target, int message_id, int id_topologia, int qtde_nodos_X_Y){
    int target_X;
    int target_Y;
    get_XY(target, &target_X, &target_Y);

    int current = source;
    while(current != target){
        int current_X;
        int current_Y;
        get_XY(source, &current_X, &current_Y);

        int direction_linha;
        int dist_linhas;
        int direction_coluna;
        int dist_colunas;

        int left = get_next_by_dir(current, 0, topologia, qtde_nodos_X_Y);
        int right = get_next_by_dir(current, 1, topologia, qtde_nodos_X_Y);
        int top = get_next_by_dir(current, 2, topologia, qtde_nodos_X_Y);
        int bottom = get_next_by_dir(current, 3, topologia, qtde_nodos_X_Y);

        // Se TOPOLOGIA_ANEL_1D
        if (topologia == 0){
            get_direction(current, target, &dist_linhas, &direction_linha);
        }
        else{
            get_direction_2D(current_X, current_Y, target_X, target_Y, &dist_linhas, &direction_linha, &dist_colunas, &direction_coluna);
            printf("[%d][%d] to [%d][%d], dist_linhas:%d, dir_linhas:%d, dist_colunas:%d, dir_coluna:%d\n", current_Y, current_X, target_Y, target_X, dist_linhas, direction_linha, dist_colunas, direction_coluna);
        }

        if(direction_linha == 0) {
            // Vai para esquerda
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições à esquerda!\n", current, message_id, target, dist_linhas);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", current, message_id, left);
        }
        else if (direction_linha == 2) {
            // Vai para direita
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições à direita!\n", current, message_id, target, dist_linhas);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", current, message_id, right);
        }

        // TOPOLOGIA_ANEL_1D
        else if (direction_linha == 1 && topologia == 0) {
            printf("Router%d -> R&A -> A msg %d em trânsito chegou no destino em X(%d) e em Y(%d)!\n", current, message_id, target_Y, target_X);
            
        }
        else if (direction_linha == 3 && topologia == 0) {
            // Tanto faz
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Está à %d posições tanto à direita como à esquerda. Enviando para a direita!\n", current, message_id, target, dist_linhas);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", current, message_id, right);
        }

        // OUTRA TOPOLOGIA
        else if (direction_linha == 1) {
            // Chegou no destino em X
            printf("Router%d -> R&A -> A msg %d em trânsito chegou no destino em X!\n", current, message_id);
            if (direction_coluna == 0){
                printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições pra baixo!\n", current, message_id, target, dist_linhas);
            }
            else if (direction_coluna == 2){
                printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições pra cima!\n", current, message_id, target, dist_linhas);
            }
            else if (direction_coluna == 1){
                printf("Router%d -> R&A -> A msg %d em trânsito chegou no destino em X(%d) e em Y(%d)!\n", current, message_id, target_Y, target_X);
            }
        }
    }
}

void app_main() {
    // Message Parameters
    int target = 5;
    int source = 1;
    int message_id = BIG_NUMBER;
    
    // Setup parameters
    int topologia = 0;
    int qtde_nodos_X_Y = 2;

    enum all_topologies topologia;
    topologia = anel_1D;
    
    run_PEs(source, target, message_id, topologia, qtde_nodos_X_Y);
}
