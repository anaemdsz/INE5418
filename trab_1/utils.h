#define MAX_SIZE 255
#define MAX_LINES 1023
#define MAX_CLIENTS 3 

#define PORT 9666

struct request {
  int type;
  int line_index;
  char text[MAX_SIZE];
};