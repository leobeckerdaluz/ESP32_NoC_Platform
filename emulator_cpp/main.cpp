#include <iostream>
#include <thread>
#include <vector>
#include "defines.h"

std::thread processors[(QTD_NODOS_X*QTD_NODOS_Y)];
std::thread routers[(QTD_NODOS_X*QTD_NODOS_Y)];

bool start_all = false;
bool all_messages_sent = false;
int new_message = BIG_NUMBER;
int turn_id = BIG_NUMBER;
int id_proc_consumidor = BIG_NUMBER;

int main() {
    // --------------------------------------------------------------
    // Verifica erros em diretivas
    // --------------------------------------------------------------
    if (verify_defines() == -1)
        return -1;

    // --------------------------------------------------------------
    // Lê o arquivo e mostra na tela
    // --------------------------------------------------------------
    std::vector<MESSAGE> allMessages = readFile();
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("Arquivo de mensagens lido!\n");
    
    // Se não há nenhuma mensagem, ou houve erro na leitura ou algum valor está fora do range
    if (allMessages.size() == 0){
        printf("ERROR: Algum source ou target está fora do range ou houve um problema na leitura!\n");
        return -2;
    }

    // for (int i = 0; i < allMessages.size(); i++) {
    //     printf("id: %d\n", allMessages[i].message_id);
    //     printf("source: %d\n", allMessages[i].header_source);
    //     printf("target: %d\n", allMessages[i].header_target);
    //     printf("msg: %s\n", allMessages[i].payload_message.c_str());
    //     printf("end: %d\n", allMessages[i].trailer_end);
    //     printf("-----------------------------\n");
    // }

    printf("--------------------------------------------------------------------------------------------------\n");
    printf("%d threads de processors e %d de routers serão criadas!\n", (QTD_NODOS_X*QTD_NODOS_Y), (QTD_NODOS_X*QTD_NODOS_Y));
    printf("--------------------------------------------------------------------------------------------------\n");
    
    // --------------------------------------------------------------
    // Inicializa as threads
    // --------------------------------------------------------------
    for (int id = 0; id < (QTD_NODOS_X*QTD_NODOS_Y); ++id) {
        // Obtém as mensagens que têm o source como esse id
        std::vector<MESSAGE> messages;
        for(int j=0; j<allMessages.size(); j++)
            if(allMessages[j].header_source == id)
                messages.push_back(allMessages[j]);

        // Cria a thread de router que irá enviar suas mensagens
        routers[id] = std::thread(router, id, messages);

        // Cria a thread do processador
        processors[id] = std::thread(processor, id);
    }


    // --------------------------------------------------------------
    // Aguarda a inicialização de todos os routers e processors
    // --------------------------------------------------------------
    while(!start_all){
        loop_start_all:

        for(int i=0; i<(QTD_NODOS_X*QTD_NODOS_Y); i++)
            if ( (!start_routers[i]) || (!start_processors[i]) )
                goto loop_start_all;

        printf("--------------------------------------------------------------------------------------------------\n");
        printf("--------------------------------- Começando a troca de mensagens ---------------------------------\n");
        printf("--------------------------------------------------------------------------------------------------\n");
        
        // Seta a flag de controle de iniciar a troca de mensagens
        start_all = true;

        // Seta um valor alto para dizer que o canal está liberado
        turn_id = BIG_NUMBER;
        id_proc_consumidor = BIG_NUMBER;
        
        // Libera para alguém enviar uma nova mensagem
        new_message = 1;
    }


    // --------------------------------------------------------------
    // Aguarda todas as mensagens serem enviadas
    // --------------------------------------------------------------
    while(!all_messages_sent){
        loop_end_all:

        for(int i=0; i<(QTD_NODOS_X*QTD_NODOS_Y); i++)
            if ( messages_to_send_yet[i] )
                goto loop_end_all;

        printf("--------------------------------------------------------------------------------------------------\n");
        printf("-------------- Acabou a troca de mensagens! Não há mais mensagens a serem enviadas! --------------\n");
        printf("--------------------------------------------------------------------------------------------------\n");
        
        // Seta a flag de controle de todas as mensagens enviadas
        all_messages_sent = true;
        
        // Seta um valor alto para dizer que o canal está liberado
        turn_id = BIG_NUMBER;
        id_proc_consumidor = BIG_NUMBER;

        // Proibe alguém enviar uma nova mensagem
        new_message = 0;
    }

    // --------------------------------------------------------------
    // Aguarda as threads encerrarem
    // --------------------------------------------------------------
    for (int i = 0; i < (QTD_NODOS_X*QTD_NODOS_Y); ++i) {
        processors[i].join();
        routers[i].join();
    }
    
    // --------------------------------------------------------------
    // Organiza as informações finais de execução
    // --------------------------------------------------------------
    AllStruct all_info;
    all_info.x = std::to_string(QTD_NODOS_X);
    all_info.y = std::to_string(QTD_NODOS_Y);
    all_info.topologia = "MESH";
    all_info.all_PEs_inside = all_PEs;
    all_info.PEs_communication_inside = PEs_communication;
    all_info.Processors_execution_inside = Processors_execution;

    generate_file(all_info);

    return 0;
}