#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "../include/defines.h"

enum all_topologies { anel_1D, anel_2D, mesh };
#define BIG_NUMBER 9999

int main() {
    int qtde_nodos_X_Y = 3;
    int source = 1;
    int target = 6;
    int topologia = mesh;
    int message_id = 12345;

    run_PEs(source, target, message_id, topologia, qtde_nodos_X_Y);

    return 0;
}