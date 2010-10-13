#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

void send_msg(int listener, int fdmax, int i, fd_set master)
{
  int nbytes = 0;
  char buf[1024];
  int j = 0;
  
  if ((nbytes = recv(i, buf, sizeof(buf), 0)) >= 0)
    {
      for (j = 0; j <= fdmax; j++)
      {
      /* send to everyone! */
        if (FD_ISSET(j, &master))
        {
        /* except the listener and ourselves */
          if (j != listener && j != i)
          {
            if (send(j, buf, nbytes, 0) == -1)
              perror("send() error ");
          }
        }
      }
    }
}

int new_member(int flag, int fdmax)
{
  int k = 0;
  char *new_member = "New member connected\n";
  
  if (flag == 1)
   {
     for (k = 0; k <= fdmax - 1; k++)
       send(k, new_member, strlen(new_member), 0);
     flag = 0;
   }
   return flag;
}

void get_init(char **argv)
{
  fd_set master;
  fd_set read_fds;
  struct sockaddr_in serveraddr;
  struct sockaddr_in clientaddr;
  int listener, listen = 0;
  int fdmax = 0;
  int addrlen = 0;
  int i = 0;
  int newfd = 0;
  char *connect = "Hello World\n";
  char *msg_send = "message send\n";
  int flag = 0;
  
  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  
  listener = f_socket();
  f_setsockopt(listener);
  
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = INADDR_ANY;
  serveraddr.sin_port = htons(PORT);
  memset(&(serveraddr.sin_zero), '\0', 8);
  
  f_bind(listener, serveraddr);
  listen = f_listen(listener);

  FD_SET(listener, &master);
  fdmax = listener;
  while(1)
  {
    read_fds = master;
    // fdmax = f_select(fdmax, read_fds);
    if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
    {
      perror("Server-select() error");
      exit(1);
    }
    printf("Server-select() is OK...\n");
        
    for (i = 0; i <= fdmax; i++)
    {
      if (FD_ISSET(i, &read_fds))
      {
        if (i == listener)
        {
          flag = 1;
          addrlen = sizeof(clientaddr);
          if((newfd = accept(listener, (struct sockaddr *)&clientaddr, &addrlen)) == -1)
            perror("Server-accept() error");
          else
          {
            printf("Server-accept() is OK...\n");
            FD_SET(newfd, &master);
            if(newfd > fdmax)
              fdmax = newfd;
            printf("%s: New connection from %s on socket %d\n", argv[0], inet_ntoa(clientaddr.sin_addr), newfd);
            send(newfd, connect, strlen(connect), 0);
          }
          flag = new_member(flag, fdmax);
        }
        else
          send_msg(listener, fdmax, i, master);
        send(i, msg_send, strlen(msg_send), 0);
      }
    }
  }
}

int main(int argc, char **argv)
{
  get_init(argv);  
  return 0;
} 