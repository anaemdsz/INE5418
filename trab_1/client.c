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
  char user_input[8];
  int line_number = 0;
  while (line_number == 0)
  {
    printf("Insert line number (starting with 0001):\n");
    // fflush(stdin);
    fgets(user_input, 4, stdin);
    line_number = atoi(user_input);
    if (line_number <= 0)
    {
      printf("Line number must be an integer bigger than zero.\n");
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
  char filler[LINE_SIZE] = {0};
  char index[8];

  sprintf(index, "%04u", line_no);
  strcat(message, index);
  strcat(message, filler);

  write(sockfd, message, REQ_SIZE);

  char response[RESPONSE_SIZE];
  read(sockfd, response, RESPONSE_SIZE);
  printf("Text from line %i: %s\n\n", line_no, response);
}

void add_line(int sockfd, int line_no, char *text)
{
  char message[REQ_SIZE] = "2";
  char buff[REQ_SIZE];
  char index[8];

  sprintf(index, "%04u", line_no);
  strcat(message, index);
  strcat(message, text);

  sprintf(buff, "%-" STR(LINE_SIZE) "s", message);
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
    perror("Client could not connect to socket.\n");
    exit(1);
  }

  // Client operations
  char user_input[4];
  while (1)
  {
    printf("Insert desired operation (get, add, ext):\n");
    fgets(user_input, 4, stdin);
    char c;
    while ((c = getchar()) != '\n' && c != EOF && c)
      ;
    printf("Selected operation: %s\n", user_input);
    if (strcmp(user_input, "ext") == 0)
    {
      // Close socket connection
      write(sockfd, "EXITING", 8);
      write(sockfd, "0", 0);
      close(sockfd);
      break;
    }
    else if (strcmp(user_input, "get") == 0)
    {
      int line_number = read_line_number();
      get_line(sockfd, line_number);
      continue;
    }
    else if (strcmp(user_input, "add") == 0)
    {
      int line_number = read_line_number();
      char text_to_write[LINE_SIZE];
      printf("Insert text to write:");
      fgets(text_to_write, LINE_SIZE, stdin);
      add_line(sockfd, line_number, text_to_write);
      continue;
    }
    else
    {
      printf("Operation not supported. The options are {get, add, ext}\n");
    }
  }

  exit(0);
}
