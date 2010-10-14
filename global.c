#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "init.h"

// numero de port 
#define PORT 2011

char *send_msg(int listener, int fdmax, int i, fd_set master, int flag_login)
{
  int nbytes = 0;
  char buf[1024];
  char login[128];
  char *login_str = NULL;
  char *stock_login;
  char *ret_login;
  int j = 0;
  int len_login = 0;
  
  /* we got some data from a client*/
  if (flag_login == 1)
  {
    if ((nbytes = recv(i, login, sizeof(login), 0)) >= 0)
    {
      len_login = strlen(login);
      stock_login = malloc(sizeof(*stock_login) * 32);
      for (j = 0; j <= (len_login - 3); j++)
        stock_login[j] = login[j];
      ret_login = malloc(128 * sizeof(char*));
      strcpy(ret_login, stock_login);
      login_str = strcat(stock_login, " : vient de se connecter\n");
      for (j = 0; j <= fdmax - 1; j++)
        send(j, login_str, strlen(login_str), 0);
      return (ret_login);
    }
  }
  else
  {
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
}

void get_login(int listener, int fdmax, int i, fd_set master)
{
  int ret = 0;
  int nbytes = 0;
  char login[128];
  int j = 0;
  char *login_connect;
  
  /* we got some data from a client*/
    if ((nbytes = recv(i, login, sizeof(login), 0)) >= 0)
    {
      login_connect = strcat(login, " : vient de se connecter\n");
      for (j = 0; j <= fdmax; j++)
      {
      /* send to everyone! */
        if (FD_ISSET(j, &master))
        {
        /* except the listener and ourselves */
          if (j != listener && j != i)
          {
            if (send(j, login_connect, nbytes, 0) == -1)
              perror("send() error ");
          }
        }
      }
        // ret = read(newfd, login, strlen(login));
        // printf("login : %s retour : %d\n", login, ret);
    }
}

int new_member(int flag_new_member, int fdmax, int flag_login_register, int i)
{
  int k = 0;
  int stock_flack;
  char *new_member = "New member connected\n";
  int nbytes = 0;
  char login[128];
  char *login_str = NULL;
  
  if (flag_new_member == 1 && flag_login_register == 1)
   {
      if ((nbytes = recv(i, login, sizeof(login), 0)) >= 0)
      {
        login_str = strcat(login, " : vient de se connecter");
        printf("###### %s #####", login_str);
        for (k = 0; k <= fdmax - 1; k++)
          send(k, login_str, strlen(login_str), 0);
        flag_new_member = 0;
      }
   }
  return flag_new_member;
}

void get_init(char **argv, t_list chat)
{
  fd_set master;                  /* master file descriptor list */
  fd_set read_fds;                /* temp file descriptor list for select() */
  struct sockaddr_in serveraddr;  /* server address */
  struct sockaddr_in clientaddr;  /* client address */
  int listener = 0;               /* listening socket descriptor */
  int fdmax = 0;                  /* maximum file descriptor number */
  int addrlen = 0;                /* for setsockopt() */
  int i = 0;
  int newfd = 0;                  /* newly accept()ed socket descriptor */
  char *connect = "Hello, Enter your login : \n";
  char *msg_send = "message send\n";
  int flag_login = 0;
  char *login;

  /* clear the master and temp sets */
  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  
  /* get the listener */
  listener = f_socket();

  /*"address already in use" error message */
  f_setsockopt(listener);
  
  /* bind */
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = INADDR_ANY;
  serveraddr.sin_port = htons(PORT);
  memset(&(serveraddr.sin_zero), '\0', 8);
  f_bind(listener, serveraddr);

  /* listen */
  listener = f_listen(listener);

  /* add the listener to the master set */
  FD_SET(listener, &master);
  /* keep track of the biggest file descriptor */
  fdmax = listener;
  while(1)
  {
    read_fds = master;
    //fdmax = f_select(fdmax, read_fds);
    if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
    {
        perror("Server-select() error");
        exit(1);
    }
    printf("Server-select() is OK...\n");
  
     /*run through the existing connections looking for data to be read*/
    for (i = 0; i <= fdmax; i++)
    {
      if (FD_ISSET(i, &read_fds))
      {
        /*there is one*/
        if (i == listener)
        {
          login = malloc(sizeof(char *) * 1024);
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
            // get_login(listener, fdmax, i, master, flag);
            flag_login = 1;
          }
          // flag_new_member = new_member(flag_new_member, fdmax, flag_login, i);
        }
        else
          {
            
            login = send_msg(listener, fdmax, i, master, flag_login); 
            if (flag_login == 1)
              put_in_list_front(&chat, newfd, login);
            show_list(chat);
            flag_login = 0;
          }
        send(i, msg_send, strlen(msg_send), 0);
      }
    }
  }
}

