#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

enum all_topologies { ANEL_1D, ANEL_2D, MESH };
enum all_directions { LEFT, RIGHT, TOP, BOTTOM, ANY_X, ANY_Y, ARRIVED };



// --------------------------
// ------- Topologias -------
// --------------------------

#define TOPOLOGIA_MESH
// #define TOPOLOGIA_ANEL_2D
// #define TOPOLOGIA_ANEL_1D

// -------------------------
// --------- Gerais --------
// -------------------------

// Qual o tamanho da NoC?
#define QTD_NODOS_X_Y 4
//// Defina se deseja que o programa printe alguns processamentos
#define DEBUG_CONSOLE true
// Defina um número aleatório alto para que o programa 
#define BIG_NUMBER 9999

// -------------------------
// ------- File Read -------
// -------------------------

// Na leitura do txt, iniciar em qual linha?
#define START_LINE_TXT 3
// Fazer o split do texto em quantas partes?
#define TXT_LINE_SPLIT_PARTS 4

// Nome do arquivo de mensagens
#define FILENAME "messages/listamensagens"
// #define FILENAME "messages/listamensagens2"
// #define FILENAME "messages/testemesh2d"

//// Defina se deseja ler arquivo com base num id (Ex:. 1, 8, 7, 3)
#define FILE_READ_ID
//// Defina se deseja ler arquivo com base num XY (Ex:. 00, 01, 12, 11)
// #define FILE_READ_XY


// -------------------------
// ------ Output File ------
// -------------------------

//// Defina se deseja que o arquivo de saída seja um XML
#define XML
//// Defina se deseja que o arquivo de saída seja um JSON
// #define JSON

#ifdef XML
    #define OUTPUT_FILE "saida.xml"
#endif
#ifdef JSON
    #define OUTPUT_FILE "saida.json"
#endif


// -------------------------
// --- Classes & Structs ---
// -------------------------

class PE { 
    public: int id;
    public: int right;
    public: int left;
    public: int top;
    public: int bottom;
};

class ProcExecution { 
    public: int id;
    public: int cont_msgs_proc;
};

class Communication { 
    public: int id;
    public: int cont_msgs_left;
    public: int cont_msgs_right;
    public: int cont_msgs_top;
    public: int cont_msgs_bottom;
    public: int cont_msgs_tantofaz;
};

class AllStruct { 
    public: std::string x;
    public: std::string y;
    public: std::string topologia;
    public: std::vector<PE> all_PEs_inside;
    public: std::vector<Communication> PEs_communication_inside;
    public: std::vector<ProcExecution> Processors_execution_inside;
};

struct MESSAGE{
    int message_id;
    int header_source;
    int header_target;
    std::string payload_message;
    int trailer_end;

    bool operator() (MESSAGE i, MESSAGE j) { return (i.message_id < j.message_id); } 
};



// Declara como externo os vetores estatísticos
extern std::vector<PE> all_PEs;
extern std::vector<Communication> PEs_communication;
extern std::vector<ProcExecution> Processors_execution;

// Declara como externo as variáveis
extern MESSAGE global_msg;
extern int turn_id;
extern int id_proc_consumidor;
extern int new_message;
extern bool messages_to_send_yet[(QTD_NODOS_X_Y*QTD_NODOS_X_Y)];
extern bool start_routers[(QTD_NODOS_X_Y*QTD_NODOS_X_Y)];
extern bool start_processors[(QTD_NODOS_X_Y*QTD_NODOS_X_Y)];
extern bool all_messages_sent;
extern bool start_all;

// Declara como externo os métodos
extern void generate_file(AllStruct all_info);
extern void processor(int id);
extern void router(int id, std::vector<MESSAGE> messages);
extern std::vector<MESSAGE> readFile();

// UTILS
extern int get_next_by_dir(int id, int direction_mode, int topologia_local, int qtde_nodos_X_Y);
extern void get_direction(int current_id, int target, int *distance, int *direction, int topologia_id, int qtde_nodos_X_Y);
int verify_defines();