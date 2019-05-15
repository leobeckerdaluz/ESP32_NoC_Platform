#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <mutex>
#include "defines.h"

std::mutex mutex_print;

bool start_processors[(QTD_NODOS_X*QTD_NODOS_Y)];

void show_messages(int id, std::vector<MESSAGE> messages_arrived);

// ----------------------------------------------------
// THREAD PROCESSOR
void processor(int id) {
    std::vector<MESSAGE> messages_arrived;
    
    printf("Processor%d -> Estou pronto para começar!\n", id);
    start_processors[id] = true;

    // Aguarda até que todos os processadores e threads inicializem
    while(!start_all);

    while(1) {
        if ((turn_id == BIG_NUMBER) && (id == id_proc_consumidor)) {
            printf("Processor%d -> Sou o processador do router %d e vou consumir\n", id, id_proc_consumidor);
            printf("Processor%d -> Mensagem é %s! Vou liberar para a próxima mensagem!\n", id, global_msg.payload_message.c_str());
            printf("--------------------------------------------------------------------------------------------------\n");
            
            messages_arrived.push_back(global_msg);
            
            // Seta um valor alto para dizer que o canal está liberado
            id_proc_consumidor = BIG_NUMBER;
            turn_id = BIG_NUMBER;
            
            // Libera para alguém enviar uma nova mensagem
            new_message = 1;
        }

        if (all_messages_sent){
            if ( messages_arrived.size() > 0 ){
                show_messages(id, messages_arrived);
            }
            else
                printf("Processor%d -> Não recebi nenhuma mensagem!\n", id);
            
            return;
        }
    }
    
}

// ----------------------------------------------------
// SHOW ARRIVED MESSAGES
void show_messages(int id, std::vector<MESSAGE> messages_arrived) {
    std::vector<int> sources;
    int cont_completed_msgs = 0;

    mutex_print.lock();

    printf("Processor%d -> Recebi %d pacotes!\n", id, messages_arrived.size());
    
    for(int message_id=0; message_id<messages_arrived.size(); message_id++){

        int actual_source = messages_arrived[message_id].header_source;
        // printf("Actual_source: %d\n", actual_source);

        if (std::count(sources.begin(), sources.end(), actual_source) != 0){
            // printf("Continue porque o %d já tá no sources!\n", actual_source);
            continue;
        }
        else{
            sources.push_back(actual_source);
            // printf("Pushed back o %d porque a sources não tem esse valor!\n", actual_source);
        }

        std::vector<MESSAGE> temp;
        for(int i=0; i<messages_arrived.size(); i++)
            if ( messages_arrived[i].header_source == actual_source )
                temp.push_back(messages_arrived[i]);

        // Em temp tem todas msgs do mesmo source!

        // for(int i=0; i<temp.size(); i++)
        //     printf("source %d | message_id %d | msg %s\n", temp[i].header_source, temp[i].message_id, temp[i].payload_message.c_str());
            
        // Sort struct
        for(int i=0; i<temp.size(); i++)
            for(int j=0; j<temp.size(); j++)
                if (temp[i].message_id < temp[j].message_id) {
                    MESSAGE tempzao = temp[i];
                    temp[i] = temp[j];
                    temp[j] = tempzao;
                }

        // Em temp as msgs estão feitas o sort com base no id

        std::string buffer_actual_complete_message = "";
        
        for(int i=0; i<temp.size(); i++){
            buffer_actual_complete_message += temp[i].payload_message;
            
            if(temp[i].trailer_end == 1){
                cont_completed_msgs++;
                printf("Processor%d -> Minha %dª msg completa:  %s\n", id, cont_completed_msgs, buffer_actual_complete_message.c_str());
                buffer_actual_complete_message = "";
            }
        }
    }

    mutex_print.unlock();
}