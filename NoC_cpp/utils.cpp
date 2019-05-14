#include <iostream>
#include <thread>
#include <vector>
#include "defines.h"

// ----------------------------------------------------
// GET DIRECTION AND DISTANCE
void get_direction(int id, int target, int *dist_linhas, int *direction_linha){
    int halfVector = QTD_NODOS_X/2;
    int realDist = abs(target - id);

    if(realDist == 0) {
        // igual
        *direction_linha = 1;
        *dist_linhas = 0;
        return;
    }

    if(realDist < halfVector){
        *direction_linha = target<id ? 0 : 2;
        *dist_linhas = realDist;
    }
    else if(realDist > halfVector){
        *direction_linha = target<id ? 2 : 0;
        *dist_linhas = QTD_NODOS_X-realDist;
    }
    else{
        // tanto faz
        *direction_linha = 3;
        *dist_linhas = halfVector;
    }
}

// ----------------------------------------------------
// GET DIRECTION AND DISTANCE 2D
void get_direction_2D(int id_X, int id_Y, int target_X, int target_Y, int *dist_linhas, int *direction_linha, int *dist_colunas, int *direction_coluna){
    #ifdef TOPOLOGIA_MESH
        //se for negativo, direita; Se positivo, esquerda;
        *dist_linhas =  (target_X - id_X);       
        if(*dist_linhas > 0)
            *direction_linha = 2;   // direita
        else if(*dist_linhas < 0)
            *direction_linha = 0;   // esquerda
        else
            *direction_linha = 1;   // chegou na coluna do target.

        //se for negativo, subir; se positivo tem que descer;
        *dist_colunas =  (target_Y - id_Y);      
        if(*dist_colunas > 0)
            *direction_coluna = 2;  // cima
        else if(*dist_colunas < 0)
            *direction_coluna = 0;  // baixo
        else
            *direction_coluna = 1;  // chegou na linha do target.        

        // Retorna o módulo 
        *dist_colunas = abs(*dist_colunas);
        *dist_linhas = abs(*dist_linhas);
    #endif

    #ifdef TOPOLOGIA_ANEL_2D
        int halfVector = QTD_NODOS_X/2;

        //se for negativo, direita; Se positivo, esquerda;
        *dist_linhas =  abs(target_X - id_X);   
        if(*dist_linhas == 0)  {
            *direction_linha = 1;
            *dist_linhas = 0;
        }  
        else if(*dist_linhas >= halfVector){
            *direction_linha = target_X < id_X ? 2 : 0;
            *dist_linhas = QTD_NODOS_X - *dist_linhas;
        }
        else if(*dist_linhas < halfVector) {
            *direction_linha = target_X < id_X ? 0 : 2;
        }

        // ------------------------------------------------------
        // -- Se fosse fazer de outro jeito, gasta mais linhas -- 
        // ------------------------------------------------------
        // *dist_linhas =  (target_X - id_X);     
        // if(*dist_linhas > 0) {
        //     // Target maior que id
        //     if (*dist_linhas > halfVector)      *direction_linha = 0;
        //     else if (*dist_linhas < halfVector)     *direction_linha = 2;
        //     else *direction_linha = 3;
        // }

        //se for negativo, subir; se positivo tem que descer;
        *dist_colunas =  abs(target_Y - id_Y);  
        if(*dist_colunas == 0) {
            *direction_coluna = 1;
            *dist_colunas = 0;
        }  
        else if(*dist_colunas >= halfVector) {
            *direction_coluna = (target_Y < id_Y) ? 2 : 0;
            *dist_colunas = QTD_NODOS_Y - *dist_colunas;
        }
        else if(*dist_colunas < halfVector) {
            *direction_coluna = (target_Y < id_Y) ? 0 : 2;
        }
    #endif
}

// ----------------------------------------------------
// GET XY BY ID
void get_XY(int id, int *id_X, int *id_Y){
    *id_Y = id/QTD_NODOS_X;
    *id_X = id%QTD_NODOS_X;
}

// ----------------------------------------------------
// GET ID BY XY
int getID_by_XY(int x_source, int y_source){
    int parteY = x_source*QTD_NODOS_X;
    int parteX = y_source;

    return parteX + parteY;
}

// ----------------------------------------------------
// VERIFY DEFINES
int verify_defines(){
    printf("--------------------------------------------------------------------------------------------------\n");
    // Na topologia ANEL_1D, Y deve ser 1
    #if (defined TOPOLOGIA_ANEL_1D) && (QTD_NODOS_Y != 1)
        printf("Na topologia Anel, a quantidade de nodos de Y precisa obrigatoriamente ser 1!\n");
        return -1;
    #endif

    // A execução não pode ser feita somente com um nodo
    #if (QTD_NODOS_Y == 1) && (QTD_NODOS_X == 1)
        printf("Não é possível executar o programa com apenas 1 PE!\n");
        return -1;
    #endif

    // Verifica se os valores de X e Y são pares se a topologia não é a ANEL_1D
    #if ((QTD_NODOS_Y%2 == 1) || (QTD_NODOS_X%2 == 1)) && (!defined TOPOLOGIA_ANEL_1D)
        printf("Só é possível executar NoC com tamanhos de X e Y pares!\n");
        return -1;
    #endif

    // Verifica se os dois tipos de entradas no arquivo foram declaradas simultâneamente
    #if (defined FILE_READ_ID) && (defined FILE_READ_XY)
        printf("Só é possível ler o arquivo ou pelo tipo XY ou pelo tipo ID. Os dois juntos não é possível!\n");
        return -1;
    #endif

    // Verifica se duas ou mais topologias foram declaradas simultâneamente
    char cont_topologies = 0;
    #if (defined TOPOLOGIA_ANEL_1D)
        cont_topologies++;
    #endif
    #if (defined TOPOLOGIA_ANEL_2D)
        cont_topologies++;
    #endif
    #if (defined TOPOLOGIA_MESH)
        cont_topologies++;
    #endif
    if (cont_topologies > 1){
        printf("Só é possível usar uma topologia!\n");
        return -1;
    }

    // Alright, return 0!
    return 0;
}
