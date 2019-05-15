#include <iostream>
#include <vector>
#include <cstring>
#include "defines.h"

bool messages_to_send_yet[(QTD_NODOS_X*QTD_NODOS_Y)];
bool start_routers[(QTD_NODOS_X*QTD_NODOS_Y)];

bool receive_from_left = false;
bool receive_from_right = false;
bool receive_from_top = false;
bool receive_from_bottom = false;

std::vector<PE> all_PEs;
std::vector<Communication> PEs_communication;
std::vector<ProcExecution> Processors_execution;

MESSAGE global_msg;

void get_direction(int id, int target, int *msgs_dist, int *direcao);
void get_direction_2D(int id_X, int id_Y, int target_X, int target_Y, int *dist_linhas, int *direction_linha, int *dist_colunas, int *direction_coluna);
void get_XY(int id, int *id_X, int *id_Y);

void reset_flags(){
    receive_from_left = false;
    receive_from_right = false;
    receive_from_top = false;
    receive_from_bottom = false;
}

// ----------------------------------------------------
// THREAD ROUTER
void router(int id, std::vector<MESSAGE> messages) {
    // Inicializa as variáveis de estatística
    int cont_msgs_left = 0;
    int cont_msgs_right = 0;
    int cont_msgs_bottom = 0;
    int cont_msgs_top = 0;
    int cont_msgs_tantofaz = 0;
    int cont_msgs_proc = 0;
    
    // Direções de envio da Topologia Mesh são setadas
    #ifdef TOPOLOGIA_MESH
        // int left = (id == (0 + (int)(id/QTD_NODOS_X)*QTD_NODOS_X))) ? BIG_NUMBER : id-1;
        int left = (id%QTD_NODOS_X == 0) ? BIG_NUMBER : id-1;
        // int right = (id == (2 + (int)(id/QTD_NODOS_X*QTD_NODOS_X))) ? BIG_NUMBER : id+1;
        int right = (id%QTD_NODOS_X == QTD_NODOS_X-1) ? BIG_NUMBER : id+1;
        int top = (id >= QTD_NODOS_X*(QTD_NODOS_Y-1)) ? BIG_NUMBER : id + QTD_NODOS_X;
        int bottom = (id < QTD_NODOS_X) ? BIG_NUMBER : id - QTD_NODOS_X;
    #endif

    // Direções de envio da Topologia Anel 2D são setadas
    #ifdef TOPOLOGIA_ANEL_2D
        // int left = (id == (0 + (int)(id/QTD_NODOS_X)*QTD_NODOS_X)) ? id+(QTD_NODOS_X-1) : id-1;
        int left = (id%QTD_NODOS_X == 0) ? id+QTD_NODOS_X-1 : id-1;
        // int right = (id == (2 + (int)(id/QTD_NODOS_X)*QTD_NODOS_X)) ? id-QTD_NODOS_X+1 : id+1;
        int right = (id%QTD_NODOS_X == QTD_NODOS_X-1) ? id-QTD_NODOS_X+1 : id+1;
        int top = (id >= QTD_NODOS_X*(QTD_NODOS_Y-1)) ? id-(QTD_NODOS_X*QTD_NODOS_Y)+QTD_NODOS_X : id + QTD_NODOS_X;
        int bottom = (id < QTD_NODOS_X) ? (QTD_NODOS_X*QTD_NODOS_Y)-QTD_NODOS_Y+id : id - QTD_NODOS_X;
    #endif

    // Direções de envio da Topologia Anel 1D são setadas
    #ifdef TOPOLOGIA_ANEL_1D
        int left = (id==0) ? (QTD_NODOS_X*QTD_NODOS_Y)-1 : id-1;
        int right = (id==(QTD_NODOS_X*QTD_NODOS_Y)-1) ? 0 : id+1;
    #endif

    // Contador e iterador de mensagens a serem enviadas
    int iterator_messages_vector = 0;
    // Informações sobre direções são apresentadas
    #if defined TOPOLOGIA_ANEL_1D
        printf("Router%d -> R&A -> Estou pronto para começar! Esquerda:%d, Direita:%d.\n", id, left, right);
    #endif
    #if (defined TOPOLOGIA_ANEL_2D) || (defined TOPOLOGIA_MESH)
        printf("Router%d -> R&A -> Estou pronto para começar! Esquerda:%d, Direita:%d, Cima:%d, Baixo:%d.\n", id, left, right, top, bottom);
    #endif

    // Cria o PE e adiciona na lista global
    PE pe;
    pe.id = id;
    pe.right = right;
    pe.left = left;
    pe.top = top;
    pe.bottom = bottom;
    all_PEs.push_back(pe);

    printf(". Tenho %d msgs para enviar!\n", messages.size());
    // Seta que esse router está pronto para começar
    start_routers[id] = true;

    // Verifica as condições de mensagens desse roteador
    if(messages.size() == 0){
        printf("Router%d -> R&A -> Não tenho mensagens a enviar!\n", id);
        messages_to_send_yet[id] = false;
    }
    else{
        messages_to_send_yet[id] = true;
    }

    // Aguarda até que todos os processadores e threads inicializem
    while(!start_all);
    
    while(1){
        loop:

        // Se é minha vez e não há nenhuma msg rolando e tenho mensagens para enviar, envio uma nova
        if(messages_to_send_yet[id]){
            // Tá liberado pra alguém enviar mensagem? Então vou!
            if ( (turn_id == BIG_NUMBER) && (new_message == 1) ) {
                // Minha vez de enviar!
                turn_id = id;
                
                if ( iterator_messages_vector < messages.size() ) {
                    global_msg = messages[iterator_messages_vector];
                    
                    printf("Router%d -> R&A -> Vou enviar a mensagem %d para o target %d!\n", id, global_msg.message_id, global_msg.header_target);
                    iterator_messages_vector++;
                    new_message = 0;

                    // Hora de encaminhar uma mensagem
                    goto get_directions;
                }
                else{
                    printf("Router%d -> Não tenho mensagens a enviar!\n", id);
                    printf("------------------------------------------\n");
                    
                    // Seta um valor alto para dizer que o canal está liberado
                    turn_id = BIG_NUMBER;
                    id_proc_consumidor = BIG_NUMBER;
                    
                    // Libera para alguém enviar uma nova mensagem
                    new_message = 1;
                    
                    messages_to_send_yet[id] = false;
                }
            }
        }
        
        // Se é minha vez, chegou algo. Se flag de mensagem está zerada, é porque é mensagem em trânsito
        if ( (turn_id == id) && (new_message == 0) ) {
            printf("Router%d -> Switch -> Chegou uma mensagem. R&A vai processar se encaminha ou consome!\n", id);
            // Contabiliza as mensagens recebidas
            if (receive_from_left)  cont_msgs_left++;
            if (receive_from_right)  cont_msgs_right++;
            if (receive_from_top)  cont_msgs_top++;
            if (receive_from_bottom)  cont_msgs_bottom++;
            // Hora de encaminhar uma mensagem
            goto get_directions;
        }

        // Se todos acabaram suas execuções, retorna
        if (all_messages_sent){
            if (messages.size() > 0)    
                printf("Router%d -> R&A -> Enviei todas as minhas %d mensagens!\n", id, messages.size());
            else
                printf("Router%d -> R&A -> Não tinha nenhuma mensagem para enviar!\n", id);

            // Cria a estatística de roteador e adiciona no vetor global
            Communication PE_commun;
            PE_commun.id = id;
            PE_commun.cont_msgs_bottom = cont_msgs_bottom;
            PE_commun.cont_msgs_left = cont_msgs_left;
            PE_commun.cont_msgs_right = cont_msgs_right;
            PE_commun.cont_msgs_top = cont_msgs_top;
            PE_commun.cont_msgs_tantofaz = cont_msgs_tantofaz;
            PEs_communication.push_back(PE_commun);

            // Cria a estatística de processador e adiciona no vetor global
            ProcExecution Proc_ex;
            Proc_ex.id = id;
            Proc_ex.cont_msgs_proc = cont_msgs_proc;
            Processors_execution.push_back(Proc_ex);
            
            return;
        }

        // Se chegou aqui é porque não pode mandar uma mensagem e porque não é a vez de encaminhar, então volta no loop
        goto loop;


        // Encaminha uma mensagem própria ou de outro router
        get_directions:

        // Obtém o id da mensagem e o id do target
        int message_id = global_msg.message_id;
        int target = global_msg.header_target;

        int direction_linha;
        int dist_linhas;
        
        int direction_coluna;
        int dist_colunas;

        int target_X;
        int target_Y;
        get_XY(target, &target_X, &target_Y);

        int id_X;
        int id_Y;
        get_XY(id, &id_X, &id_Y);
        
        #ifdef TOPOLOGIA_ANEL_1D
            get_direction(id, target, &dist_linhas, &direction_linha);
        #endif
        #if (!defined TOPOLOGIA_ANEL_1D)
            get_direction_2D(id_X, id_Y, target_X, target_Y, &dist_linhas, &direction_linha, &dist_colunas, &direction_coluna);
            // printf("[%d][%d] to [%d][%d], dist_linhas:%d, dir_linhas:%d, dist_colunas:%d, dir_coluna:%d\n", id_Y, id_X, target_Y, target_X, dist_linhas, direction_linha, dist_colunas, direction_coluna);
        #endif

        if(direction_linha == 0) {
            // Vai para esquerda
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições à esquerda!\n", id, message_id, target, dist_linhas);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", id, global_msg.message_id, left);
            // Reseta as flags
            reset_flags();
            // Mensagem será enviada pela esquerda, então chegará pela direita
            receive_from_left = true;
            // Incrementa o contador de mensagens pela esquerda 
            cont_msgs_left++;
            // Agora é a vez do nodo da direita
            turn_id = left;
        }
        else if (direction_linha == 2) {
            // Vai para direita
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições à direita!\n", id, message_id, target, dist_linhas);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", id, global_msg.message_id, right);
            // Reseta as flags
            reset_flags();
            // Mensagem será enviada pela direita, então chegará pela esquerda
            receive_from_left = true;
            // Incrementa o contador de mensagens pela direita 
            cont_msgs_right++;
            // Agora é a vez do nodo da direita
            turn_id = right;
        }
        #if (defined TOPOLOGIA_ANEL_1D)
            else if (direction_linha == 1) {
                printf("Router%d -> R&A -> A msg %d em trânsito chegou no destino em X(%d) e em Y(%d)!\n", id, message_id, target_Y, target_X);
                // Mensagem será enviada para o processador, então somente reseta as flags
                reset_flags();
                // Seta o id do processador a consumir
                id_proc_consumidor = target;
                // Incrementa o contador de mensagens enviadas para o processador
                cont_msgs_proc++;
                // Agora é a vez do INFINITO
                turn_id = BIG_NUMBER;
            }
            else if (direction_linha == 3){
                // Tanto faz
                printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Está à %d posições tanto à direita como à esquerda. Enviando para a direita!\n", id, message_id, target, dist_linhas);
                printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", id, global_msg.message_id, right);
                // Reseta as flags
                reset_flags();
                // Mensagem será enviada pela direita, então chegará pela esquerda
                receive_from_left = true;
                // Incrementa o contador de mensagens pela direita (Tanto Faz)
                cont_msgs_tantofaz++;
                // Agora é a vez do nodo da direita
                turn_id = right;
            }
        #endif
        #if (!defined TOPOLOGIA_ANEL_1D )
            else if (direction_linha == 1) {
                // Chegou no destino em X
                printf("Router%d -> R&A -> A msg %d em trânsito chegou no destino em X!\n", id, message_id);
                if (direction_coluna == 0){
                    printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições pra baixo!\n", id, message_id, target, dist_linhas);
                    // Reseta as flags
                    reset_flags();
                    // Mensagem será enviada para baixo, então será recebida por cima
                    receive_from_bottom = true;
                    // Incrementa o contador de mensagens por baixo
                    cont_msgs_bottom++;
                    // Agora é a vez do nodo de baixo
                    turn_id = bottom;
                }
                else if (direction_coluna == 2){
                    printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições pra cima!\n", id, message_id, target, dist_linhas);
                    // Reseta as flags
                    reset_flags();
                    // Mensagem será enviada para cima, então será recebida por baixo
                    receive_from_bottom = true;
                    // Incrementa o contador de mensagens por cima
                    cont_msgs_top++;
                    // Agora é a vez do nodo de cima
                    turn_id = top;
                }
                else if (direction_coluna == 1){
                    printf("Router%d -> R&A -> A msg %d em trânsito chegou no destino em X(%d) e em Y(%d)!\n", id, message_id, target_Y, target_X);
                    // Mensagem será enviada para o processador, então somente reseta as flags
                    reset_flags();
                    // Seta o id do processador a consumir
                    id_proc_consumidor = target;
                    // Incrementa o contador de mensagens enviadas para o processador
                    cont_msgs_proc++;
                    // Agora é a vez do INFINITO
                    turn_id = BIG_NUMBER;
                }
            }
        #endif
    }

    // Aqui a thread já encerrou sem isso
    

    printf("--------------------------------------------------------------------------------------------------\n");
}