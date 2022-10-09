#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "utils.h"

int read_line_number()
{
  char user_input[4];
  int line_number = 0;
  while (line_number == 0)
  {
    printf("Insert line number (starting with 1):");
    // fflush(stdin);
    scanf("%s", &user_input);
    line_number = atoi(user_input);
    if (line_number <= 0)
    {
      printf("Line number must be an integer bigger than zero.");
      continue;
    }
    else
    {
      break;
    }
  }
  return line_number;
}

void get_line(int sockfd, int line_no)
{
  char message[REQ_SIZE] = "1";
  char index[4];
  char filler[LINE_SIZE] = {0};

  sprintf(index, "%04u", line_no);
  strcat(message, index);
  strcat(message, filler);

  write(sockfd, message, REQ_SIZE);

  char response[LINE_SIZE];
  read(sockfd, response, LINE_SIZE);
  printf("Line text %i: %s\n", line_no, response);
}

void add_line(int sockfd, int line_no, char *text)
{
  char message[REQ_SIZE] = "2";
  char buff[REQ_SIZE];
  char index[4];

  sprintf(index, "%04u", line_no);
  strcat(message, index);
  strcat(message, text);
  printf("%s\n", message);

  sprintf(buff, "%-" STR(LINE_SIZE) "s", message);
  printf("%s\n", buff);
  write(sockfd, buff, REQ_SIZE);

  char response[LINE_SIZE];
  read(sockfd, response, LINE_SIZE);
  printf("Server answer: %s\n", response);
}

int main(int argc, char **argv)
{

  // Socket connection
  int sockfd;
  int len;
  struct sockaddr_in address;
  int result;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  // address.sin_addr.s_addr = inet_addr("192.168.0.15");
  // address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(PORT);
  len = sizeof(address);
  result = connect(sockfd, (struct sockaddr *)&address, len);
  if (result == -1)
  {
    perror("Client could not connect to socket.");
    exit(1);
  }

  // Client operations
  char user_input[3];
  while (1)
  {
    strcpy(user_input, "");
    printf("Insert desired operation (get, add, ext):");
    scanf("%[^\n]+", user_input);
    if (strcmp(user_input, "ext") == 0)
    {
      break;
    }
    if (strcmp(user_input, "get") == 0)
    {
      int line_number = read_line_number();
      get_line(sockfd, line_number);
      continue;
    }
    if (strcmp(user_input, "add") == 0)
    {
      int line_number = read_line_number();
      char text_to_write[LINE_SIZE];
      printf("Insert text to write:");
      scanf("%s", &text_to_write);
      add_line(sockfd, line_number, text_to_write);
      continue;
    }
    printf("%s\n", user_input);
    printf("Operation not supported. The options are {get, add, ext}\n");
  }

  // Close socket connection
  close(sockfd);
  exit(0);
}
