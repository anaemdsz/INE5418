#define LINE_SIZE 255
#define MAX_LINES 1023
#define MAX_CLIENTS 3

#define MAX_LINE_DIGITS 4

#define PORT 9666
#define GET_LINE 1
#define WRITE_LINE 2

struct request {
  int type;
  int line_index;
  char text[LINE_SIZE];
};