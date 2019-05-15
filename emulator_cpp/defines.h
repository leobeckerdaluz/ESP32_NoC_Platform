#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

// --------------------------
// ------- Topologias -------
// --------------------------

#define TOPOLOGIA_MESH
// #define TOPOLOGIA_ANEL_2D
// #define TOPOLOGIA_ANEL_1D

// -------------------------
// --------- Nodos ---------
// -----------centoevintidoze--------------

#define QTD_NODOS_X 2
#define QTD_NODOS_Y 2


#define BIG_NUMBER 9999

// -------------------------
// ------- File Read -------
// -------------------------

/* File Read Paremeters */
#define START_LINE_TXT 3
#define TXT_LINE_SPLIT_PARTS 4

/* File Name */
#define FILENAME "messages/listamensagens"
// #define FILENAME "messages/listamensagens2"
// #define FILENAME "messages/testemesh2d"

/* Source/Target type */
#define FILE_READ_ID
// #define FILE_READ_XY


// -------------------------
// ------ Output File ------
// -------------------------




#define XML
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
extern bool messages_to_send_yet[(QTD_NODOS_X*QTD_NODOS_Y)];
extern bool start_routers[(QTD_NODOS_X*QTD_NODOS_Y)];
extern bool start_processors[(QTD_NODOS_X*QTD_NODOS_Y)];
extern bool all_messages_sent;
extern bool start_all;

// Declara como externo os métodos
extern int verify_defines();
extern int getID_by_XY(int x_source, int y_source);
extern void generate_file(AllStruct all_info);
extern void processor(int id);
extern void router(int id, std::vector<MESSAGE> messages);
extern std::vector<MESSAGE> readFile();

