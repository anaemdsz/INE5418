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
char text[MAX_LINES + 1][LINE_SIZE];
pthread_mutex_t line_lock[MAX_LINES];

char req_type;
char line_index[MAX_LINE_DIGITS];
char new_line[LINE_SIZE];
char response[RESPONSE_SIZE] = {0};
char line[LINE_SIZE] = {0};

void read_file_matrix()
{
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int i = 1;
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

void save_file()
{
  shared_file = fopen(FILENAME, "wb");
  fwrite(text, sizeof(char), sizeof(text), shared_file);
  fclose(shared_file);
}

char *get_line(int index)
{
  strcpy(line, text[index]);
  return line;
}

char *add_line(int index, char *content)
{
  if (sizeof(content) > LINE_SIZE)
  {
    return "ERR0";
  }
  else if (index < 0 || index >= MAX_LINES)
  {
    return "ERR1";
  }
  else
  {
    pthread_mutex_lock(&line_lock[index]);
    strcpy(text[index], content);
    pthread_mutex_unlock(&line_lock[index]);
    return "DONE";
  }
  return "ERR?";
}

void *client_req_handler(void *params)
{
  int client_sockfd = *(int *)params;
  char str_in[REQ_SIZE];

  while (read(client_sockfd, &str_in, REQ_SIZE) > 0)
  {
    {

      req_type = str_in[0];
      strncpy(line_index, &str_in[1], 4);
      strncpy(new_line, &str_in[5], LINE_SIZE);

      printf("ReqType: %c\nLineIndex: %s\nNewLine: %s", req_type, line_index, new_line);

      // handle_request(req_type, line_index, new_line);
      if (req_type == '1')
      {
        printf("Processing get.\n");
        strncpy(response, get_line(atoi(line_index)), LINE_SIZE);
      }
      else if (req_type == '2')
      {
        printf("Processing add.\n");
        strcpy(response, "");
        sprintf(response, "%-" STR(LINE_SIZE) "s", add_line(atoi(line_index), new_line));
      }
      else
      {
        save_file();
        close(client_sockfd);
        printf("DIDNT.\n");
      }

      printf("Response:%s\n", response);
      write(client_sockfd, &response, RESPONSE_SIZE);
    }
  }
}

int main()
{
  read_file_matrix();
  int server_sockfd, client_sockfd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  // server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(PORT);
  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
  listen(server_sockfd, MAX_CLIENTS);
  while (1)
  {
    printf("server waiting\n");
    client_len = sizeof(client_address);
    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

    pthread_t thread_id;
    int thread_status = pthread_create(&thread_id, NULL, client_req_handler, &client_sockfd);
    if (thread_status != 0)
    {
      printf("Thread error");
      exit(1);
    }
  }
}
