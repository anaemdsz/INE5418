#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "utils.h"

struct request {
  int type;
  int line_index;
  char *text;
};

int read_line_number() {
  char *user_input;
  int line_number = 0;
  while (line_number == 0) {
    printf("Insert line number (starting with 1):");
    scanf("%s", &user_input);
    fflush(stdin);
    line_number = atoi(user_input);
    if (line_number <= 0) {
      printf("Line number must be an integer bigger than zero.");
      continue;
    } else {
      break;
    }
  }
  return line_number;
}

void get_line(int sockfd, int line_no) {
  struct request req;
  req.type = GET_LINE;
  req.line_index = line_no;
  write(sockfd, &req, sizeof(req));
  //char text[LINE_SIZE];
	//read(sockfd, text, LINE_SIZE);
  char *text;
  read(sockfd, text, sizeof(text));
  printf("Line text %i: %s\n", line_no, text);
}

void add_line(int sockfd, int line_no, char *text) {
  struct request req;
  req.type = GET_LINE;
  req.line_index = line_no;
  req.text = text;
  write(sockfd, &req, sizeof(req));
  char *server_answer;
  read(sockfd, server_answer, sizeof(server_answer));
  printf("Server answer: %s\n", line_no, server_answer);
}

int main(int argc, char **argv) {

  // Socket connection
  int sockfd;
  int len;
  struct sockaddr_in address;
  int result;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  //address.sin_addr.s_addr = inet_addr("192.168.0.15");
  //address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(PORT);
  len = sizeof(address);
  result = connect(sockfd, (struct sockaddr *)&address, len);
  if(result == -1) {
		perror("Client could not connect to socket.");
		exit(1);
  }

  // Client operations
  char* user_input = "";
  while (1) {
    printf("Insert desired operation (get_line, add_line, exit):");
    scanf("%s", &user_input);
    fflush(stdin);
    if (user_input == "exit") {
      break;
    }
    if (user_input == "get_line") {
      int line_number = read_line_number();
      get_line(sockfd, line_number);
      continue;
    }
    if (user_input == "add_line") {
      int line_number = read_line_number();
      char *text_to_write;
      printf("Insert text to write:");
      scanf("%s", &text_to_write);
      add_line(sockfd, line_number, text_to_write);
      continue;
    }
    printf("Operation not supported. The options are get_line, add_line and exit.");
  }

  // Close socket connection
  close(sockfd);
	exit(0);close_socket_connection(sockfd);
}
