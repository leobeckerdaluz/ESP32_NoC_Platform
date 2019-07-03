#include <iostream>
#include <vector>
#include <cstring>
#include "defines.h"

bool messages_to_send_yet[(QTD_NODOS_X_Y*QTD_NODOS_X_Y)];
bool start_routers[(QTD_NODOS_X_Y*QTD_NODOS_X_Y)];

bool receive_from_left = false;
bool receive_from_right = false;
bool receive_from_top = false;
bool receive_from_bottom = false;

std::vector<PE> all_PEs;
std::vector<Communication> PEs_communication;
std::vector<ProcExecution> Processors_execution;

MESSAGE global_msg;

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
    
    int topologia;

    // Contador e iterador de mensagens a serem enviadas
    int iterator_messages_vector = 0;
    // Informações sobre direções são apresentadas
    #if defined TOPOLOGIA_ANEL_1D
        topologia = ANEL_1D;
    #elif (defined TOPOLOGIA_ANEL_2D)
        topologia = ANEL_2D;
    #elif (defined TOPOLOGIA_MESH)
        topologia = MESH;
    #endif

    // Cria o PE e adiciona na lista global
    PE pe;
    pe.id = id;
    pe.right = get_next_by_dir(id, RIGHT, topologia, QTD_NODOS_X_Y);
    pe.left = get_next_by_dir(id, LEFT, topologia, QTD_NODOS_X_Y);
    pe.top = get_next_by_dir(id, TOP, topologia, QTD_NODOS_X_Y);
    pe.bottom = get_next_by_dir(id, BOTTOM, topologia, QTD_NODOS_X_Y);
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

        int direction;
        int distance;
        
        // Obtém a direção e a distância do próximo router
        get_direction(id, target, &distance, &direction, topologia, QTD_NODOS_X_Y);
        printf("[%d] to [%d] | distance:%d | direction:%d\n", id, target, distance, direction);

        // Verifica a direção retornada
        if(direction == LEFT) {
            // Vai para esquerda
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições à esquerda!\n", id, message_id, target, distance);
            // Obtém o id do router da esquerda
            int left = get_next_by_dir(id, LEFT, topologia, QTD_NODOS_X_Y);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", id, message_id, left);
            // Reseta as flags
            reset_flags();
            // Mensagem será enviada pela esquerda, então chegará pela direita
            receive_from_left = true;
            // Incrementa o contador de mensagens pela esquerda 
            cont_msgs_left++;
            // Agora é a vez do nodo da esquerda
            turn_id = left;
        }
        else if ((direction == RIGHT) || (direction == ANY_X)) {
            // Vai para direita
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. ", id, message_id, target);
            if (direction == ANY_X)
                printf("Está à %d posições pra esquerda ou direita. Enviando para a direita!\n", distance);
            else
                printf("Ainda precisa andar %d posições à direita!\n", distance);
            // Obtém o id do router da direita
            int right = get_next_by_dir(id, RIGHT, topologia, QTD_NODOS_X_Y);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", id, message_id, right);
            // Reseta as flags
            reset_flags();
            // Mensagem será enviada pela direita, então chegará pela esquerda
            receive_from_right = true;
            // Incrementa o contador de mensagens pela direita 
            cont_msgs_right++;
            // Agora é a vez do nodo da direita
            turn_id = right;
        }
        else if ((direction == TOP) || (direction == ANY_Y)) {
            // Vai para cima
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições pra cima!\n", id, message_id, target, distance);
            // Obtém o id do router de cima
            int top = get_next_by_dir(id, TOP, topologia, QTD_NODOS_X_Y);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", id, message_id, top);
            // Reseta as flags
            reset_flags();
            // Mensagem será enviada por cima, então chegará por baixo
            receive_from_top = true;
            // Incrementa o contador de mensagens por cima 
            cont_msgs_top++;
            // Agora é a vez do nodo de cima
            turn_id = top;
        }
        else if (direction == BOTTOM) {
            // Vai para baixo
            printf("Router%d -> R&A -> A msg %d em trânsito é para o target=%d. Ainda precisa andar %d posições pra baixo!\n", id, message_id, target, distance);
            // Obtém o id do router de cima
            int bottom = get_next_by_dir(id, BOTTOM, topologia, QTD_NODOS_X_Y);
            printf("Router%d -> Switch -> Enviando a mensagem %d para o router %d!\n", id, message_id, bottom);
            // Reseta as flags
            reset_flags();
            // Mensagem será enviada por baixo, então chegará por cima
            receive_from_bottom = true;
            // Incrementa o contador de mensagens por baixo
            cont_msgs_bottom++;
            // Agora é a vez do nodo de baixo
            turn_id = bottom;
        }
        else if (direction == ARRIVED) {
            printf("Router%d -> R&A -> A msg %d em trânsito chegou no destino em [%d]!\n", id, message_id, target);
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

    // Aqui a thread já encerrou sem isso
    

    printf("--------------------------------------------------------------------------------------------------\n");
}