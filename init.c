#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "init.h"

#define PORT 2011

int f_socket()
{
  int listener = 0;
  
  if((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("Server-socket() error");
    exit(1);
  }
  printf("Server-socket() is OK...\n");
  return listener;
}

int f_listen(int listener)
{
  if (listen(listener, 10) == -1)
  {
    perror("Server-listen() error");
    exit(1);
  }
  printf("Server-listen() is OK...\n");
  return listener;
}

void f_bind(int listener, struct sockaddr_in serveraddr)
{
  if(bind(listener, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
  {
    perror("Server-bind() error");
    exit(1);
  }
  printf("Server-bind() is OK...\n");
}

void f_setsockopt(int listener)
{
  int yes = 1;
  
  if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
  {
    perror("Server-setsockopt() error");
    exit(1);
  }
  printf("Server-setsockopt() is OK...\n");
}

int f_select(int fdmax, fd_set read_fds)
{
  if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
  {
    perror("Server-select() error");
    exit(1);
  }
  printf("Server-select() is OK...\n");
  return fdmax;
}