#include <iostream>
#include <fstream> // To read file
#include <vector>
#include "defines.h"

std::vector<std::string> split(std::string stringToBeSplitted, std::string delimeter);

// ----------------------------------------------------
// READ FILE AND RETURN THE MESSAGES
std::vector<MESSAGE> readFile() {
    std::ifstream myReadFile;
    std::vector<MESSAGE> allSplittedStrings;
    
    int actualLine = 0;
    
    // Abre o arquivo
    myReadFile.open(FILENAME);
    
    if (!myReadFile) {
        std::cout << "Unable to open file\n";
        exit(1);
    }

    for( std::string line; getline( myReadFile, line ); ){
        // // Mostra a linha inteira na tela
        // printf("--------------------------------------------------------------------------------------------------\n");
        // printf( "line%d -> %s\n", actualLine, line.c_str() );
            
        // Começa a processar a partir da 3ª linha
        if(actualLine >= START_LINE_TXT - 1){
            std::vector<std::string> splittedStrings;

            // Pra cada linha, dá um split nos componentes com o divisor "	"
            splittedStrings = split(line, "	");

            // Em quantas partes fez o split
            if(splittedStrings.size() < TXT_LINE_SPLIT_PARTS){
                std::cout << "---------------------------------------------------\n";
                std::cout << "DEU PAU em dividir a string \"" << line << "\" em 4 partes!!\n";
                std::cout << "---------------------------------------------------\n";
                exit(1);
            }

            MESSAGE msg;
            msg.message_id = (char)(actualLine - START_LINE_TXT + 1);
            msg.payload_message = splittedStrings[2];
            msg.trailer_end = stoi(splittedStrings[3]);
            #ifdef FILE_READ_ID
                msg.header_source = stoi(splittedStrings[0]);
                msg.header_target = stoi(splittedStrings[1]);
            #endif
            #ifdef FILE_READ_XY
                int x_source = (int)splittedStrings[0][0] - (int)48;
                int y_source = (int)splittedStrings[0][1] - (int)48;
                int id_source = getID_by_XY(x_source, y_source);
                
                int x_target = (int)splittedStrings[1][0] - (int)48;
                int y_target = (int)splittedStrings[1][1] - (int)48;
                int id_target = getID_by_XY(x_target, y_target);

                msg.header_source = id_source;
                msg.header_target = id_target;

                printf("SOURCE(%d): %d %d - TARGET(%d): %d %d\n", id_source, x_source, y_source, id_target, x_target, y_target);
            #endif

            if ( (msg.header_target<0) || (msg.header_target>=(QTD_NODOS_X_Y*QTD_NODOS_X_Y)) ) {
                printf("--------------------------------------------------------------------------------------------------\n");
                printf("Target da mensagem de id %d está fora dos ranges!\n", msg.message_id);
                std::vector<MESSAGE> empty;
                return empty; 
            }

            if ( (msg.header_source<0) || (msg.header_source>=(QTD_NODOS_X_Y*QTD_NODOS_X_Y)) ) {
                printf("--------------------------------------------------------------------------------------------------\n");
                printf("Source da mensagem de id %d está fora dos ranges!\n", msg.message_id);
                std::vector<MESSAGE> empty;
                return empty;
            }

            allSplittedStrings.push_back(msg);
        }
        actualLine++;
    }
    
    myReadFile.close();

    return allSplittedStrings;
}

// ----------------------------------------------------
// SPLIT STRING
std::vector<std::string> split(std::string stringToBeSplitted, std::string delimeter){
    std::vector<std::string> splittedString;
    int startIndex = 0;
    int  endIndex = 0;
    
    while( (endIndex = stringToBeSplitted.find(delimeter, startIndex)) < stringToBeSplitted.size() ){
        std::string val = stringToBeSplitted.substr(startIndex, endIndex - startIndex);
        splittedString.push_back(val);
        startIndex = endIndex + delimeter.size();
    }
    
    if(startIndex < stringToBeSplitted.size()){
        std::string val = stringToBeSplitted.substr(startIndex);
        splittedString.push_back(val);
    }
    
    return splittedString;
}