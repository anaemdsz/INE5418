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

void get_line(int sockfd, int line_no) {
  struct request req;
  req.type = GET_LINE;
  req.line_index = line_no;
  write(sockfd, &req, sizeof(req));
  //char text[LINE_SIZE];
	//read(sockfd, text, LINE_SIZE);
  char *text;
  read(sockfd, text, sizeof(text));
  printf("Texto da linha %i: %s\n", line_no, text);
}

void add_line(int sockfd, int line_no, char *text) {
  struct request req;
  req.type = GET_LINE;
  req.line_index = line_no;
  req.text = text;
  write(sockfd, &req, sizeof(req));
  char *text;
  read(sockfd, text, sizeof(text));
  printf("Resposta do servidor: %s\n", line_no, text);
}

int main(int argc, char **argv)
{
  if (argc < 3) {
    perror("Not enough arguments. Try \"./client get_line {line_no}\" or \"./client add_line {line_no} \"{text}\" \" ");
		exit(1);
  }
  if (argv[2] != "get_line" && argv[2] != "write_line") {
    perror("Function not supported. Try \"./client get_line {line_no}\" or \"./client add_line {line_no} \"{text}\" \" ");
		exit(1);
  }
  if (atoi(argv[3]) == 0) {
    perror("Third argument must be a line number bigger than zero. Try \"./client get_line {line_no}\" or \"./client add_line {line_no} \"{text}\" \" ");
    exit(1);
  }
  if (argv[3] == "write_line" && argc != 4) {
    perror("Text to write was not supplied. Try \"./client add_line {line_no} \"{text}\" \" ");
    exit(1);
  }

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

  // Operations
  int line_number = atoi(argv[3]) - 1; // To index the file starting with 0
  
  if (argv[3] == "get_line") {
    get_line(sockfd, line_number);
  } else if (argv[3] == "write_line") {
    char *text_to_write = argv[4];
    write_line(sockfd, line_number, text_to_write);
  }

  // Close socket connection
	close(sockfd);
	exit(0);
}
