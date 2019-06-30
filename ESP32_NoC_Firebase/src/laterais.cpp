// #include <iostream>
// #include <stdio.h>
// #include <stdlib.h>
// #include "../include/defines.h"

// // ---------------------------------------------------------------------------
// // Método que executa a comunicação
// void run_PEs(int source, int target, int message_id, int topologia, int qtde_nodos_X_Y){
//     bool ArrivedResult = 1; 
    
//     ShowMessageDisplay(source, target, message_id);

//     int current_id = source;
//     while(current_id != target) {
//         here:
        
//         printf("----------------------------------------------------------------------------\n");
        
//         ShowCurrentID(current_id, target, message_id);

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



// int main() {
//     int qtde_nodos_X_Y = 3;
//     int source = 1;
//     int target = 8;
//     int topologia = ANEL_2D;
//     int message_id = 12345;

//     run_PEs(source, target, message_id, topologia, qtde_nodos_X_Y);

//     return 0;
// }