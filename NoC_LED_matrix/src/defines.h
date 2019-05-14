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

// Declara como externo os métodos
extern int getID_by_XY(int x_source, int y_source);


extern void get_direction(int id, int target, int *dist_linhas, int *direction_linha);
extern void get_direction_2D(int id_X, int id_Y, int target_X, int target_Y, int *dist_linhas, int *direction_linha, int *dist_colunas, int *direction_coluna);
extern void get_XY(int id, int *id_X, int *id_Y);
extern int getID_by_XY(int x_source, int y_source);