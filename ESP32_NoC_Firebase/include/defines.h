
#define FIREBASE_HOST "noc-react-js-app.firebaseio.com"
#define FIREBASE_AUTH "20lXtCo3e6aKGuBjAltvR0KGJ7n0JbSI18auMBPb"
#define WIFI_SSID "CS6"
#define WIFI_PASSWORD "luisdaluz6"

// #define DEBUG_CONSOLE1

#define BIG_NUMBER 9999

enum all_topologies { ANEL_1D, ANEL_2D, MESH };
enum all_directions { LEFT, RIGHT, TOP, BOTTOM, ANY_X, ANY_Y, ARRIVED };

// UTILS
extern int get_next_by_dir(int id, int direction_mode, int topologia_local, int qtde_nodos_X_Y);
extern void get_direction(int current_id, int target, int *distance, int *direction, int topologia_id, int qtde_nodos_X_Y);

// ESP32_DRIVER
extern void BlinkXY(int qtde_nodos_X_Y);
extern void ShowMessageDisplay(int source, int target, int message_id);
extern void ShowCurrentID(int current_id, int target, int message_id, int qtde_nodos_X_Y);
extern void StartAll(int qtde_nodos_X_Y);