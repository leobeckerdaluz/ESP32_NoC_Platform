
#define FIREBASE_HOST "noc-react-js-app.firebaseio.com" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "20lXtCo3e6aKGuBjAltvR0KGJ7n0JbSI18auMBPb"
#define WIFI_SSID "Estrela-32"
#define WIFI_PASSWORD "AdC111729"

#define BIG_NUMBER 9999

// Declara como externo os métodos
extern void run_PEs(int source, int target, int message_id, int topologia, int qtde_nodos_X_Y);
extern void get_direction(int current_id, int target, int *distance, int *direction, int topologia_id, int qtd_nodos_X_Y);
extern void get_XY(int id, int *id_X, int *id_Y, int qtd_nodos_X_Y);
extern int getID_by_XY(int x_source, int y_source, int qtd_nodos_X_Y);
extern int get_next_by_dir(int id, int direction_mode, int topologia_local, int qtde_nodos_X_Y);