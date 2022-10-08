#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "utils.h"

#define FILENAME "file.txt"

FILE *shared_file;
char text[MAX_LINES][LINE_SIZE];
pthread_mutex_t line_lock[MAX_LINES];

void read_file_matrix()
{
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int i = 0;
  shared_file = fopen(FILENAME, "r");

  while (((read = getline(&line, &len, shared_file)) != -1) && (i <= MAX_LINES))
  {
    strcpy(text[i], line);
    i++;
  }
  fclose(shared_file);
  if (line)
    free(line);
}

char *get_line(int index)
{
  char *line;
  strcpy(line, text[index]);
  return line;
}

int add_line(int index, char *content)
{
  if (sizeof(content) > LINE_SIZE)
  {
    return 2;
  }
  else if (index < 0 || index >= MAX_LINES)
  {
    return 1;
  }
  else
  {
    pthread_mutex_lock(&line_lock[index]);
    strcpy(text[index], content);
    pthread_mutex_unlock(&line_lock[index]);
  }
}

int main()
{
  read_file_matrix();
  int server_sockfd, client_sockfd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  char str_in[1024];
  char str_out[1024];

  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family = AF_INET;
  //  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(PORT);
  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
  listen(server_sockfd, MAX_CLIENTS);
  while (1)
  {
    printf("server waiting\n");
    client_len = sizeof(client_address);
    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
    read(client_sockfd, &str_in, 1024);
    printf(str_in);
    // sprintf(str_out, "%s cruel\n", str_in);
    sprintf(str_out, "Processing req.");
    write(client_sockfd, &str_out, 1024);


    close(client_sockfd);
  }
}
