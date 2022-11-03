#define MAX_CLIENTS 2


#define LINE_SIZE 10
#define MAX_LINES 1023
#define MAX_LINE_DIGITS 4


#define PORT 9666

#define EXIT 0
#define GET_LINE 1
#define WRITE_LINE 2

#define REQ_SIZE 1+LINE_SIZE+MAX_LINE_DIGITS+1
#define RESPONSE_SIZE 256

#define STR_(X) #X
#define STR(X) STR_(X)

#define DELAY 0