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

void close_client(int current_fd, fd_set master, int fdmax, t_list chat)
{
  char *disconnect = "Client disconnected\n";
  char *msg_disconnect_all_users = " has just disconneted !\n";
  char *user_disconnect;
  int j = 0;
  int ret = 0;
  
  user_disconnect = check_list_return_login(chat, current_fd);
  send(current_fd, disconnect, strlen(disconnect), 0);
  for (j = 0; j <= fdmax; j++)
    {
      if (j != current_fd)
        {
          send(j, user_disconnect, strlen(user_disconnect), 0);
          send(j, msg_disconnect_all_users, strlen(msg_disconnect_all_users), 0);
        }
    }
}

void check_cmd(char *str, t_list chat, int current_fd, fd_set master, int fdmax)
{
  char *check_login_in_list = NULL;
  int i = 1;
  int j = 0;
  int fd_valid = 0;
  char *private_msg = NULL;
  char *bad_login = "bad login's name\n";
  char *private_user_msg = "Private message from ";
  char *recup_name_bis;
  
    //     close_client(current_fd, master, fdmax, chat);
    //     check_list_delog(&chat, current_fd);
    {  
      /*recup login and message for a private message if login is in the list*/
      check_login_in_list = malloc(sizeof(char *) * 32);
      bzero(check_login_in_list, 0);
      private_msg = malloc(sizeof(char *) * 1024);
      bzero(private_msg, 0);
      while (str[i] != ' ' && str[i + 1] != ':')
      {
        check_login_in_list[i - 1] = str[i];
        i++;
      }
      i += 2;
      fd_valid = check_list_return_fd(chat, check_login_in_list);
      while (str[i])
        private_msg[j++] = str[i++];
      if (fd_valid != 0)
        {
          recup_name_bis = check_list_return_login(chat, current_fd);
          if (recup_name_bis != "unfind")
            {
              send(fd_valid, private_user_msg, strlen(private_user_msg), 0);
              send(fd_valid, recup_name_bis, strlen(recup_name_bis), 0);
              send(fd_valid, " : ", 3, 0);
            }
          send(fd_valid, private_msg, strlen(private_msg), 0);
        }
      else
        send(current_fd, bad_login, strlen(bad_login), 0);
       free(private_msg);
    }
}

char *send_msg(int listener, int fdmax, int i, fd_set master, fd_set *read_fds, fd_set *save_master, int flag_login, t_list chat)
{
  int nbytes = 0;
  char buf[1024];
  char login[128];
  char *login_str = NULL;
  char *stock_login;
  char *ret_login;
  char *connect_log = "login already in use!\n Enter a other Login: \n";
  int j = 0;
  int len_login = 0;
  char *connect = "Connected\n";
  char *msg_user = " wrote : ";
  char *recup_name;
  
  bzero(buf, 0);
  /* we got some data from a client*/
  if (flag_login == 1)
  {
    if ((nbytes = recv(i, login, sizeof(login), 0)) >= 0)
    {
      login[nbytes] = '\0';
      len_login = strlen(login);
      stock_login = malloc(sizeof(*stock_login) * 32);
      bzero(stock_login, 0);
      for (j = 0; j <= (len_login - 3); j++)
        stock_login[j] = login[j];
      ret_login = malloc(128 * sizeof(char*));
      bzero(ret_login, 0);
      strcpy(ret_login, stock_login);
      if (check_list_return_fd(chat, ret_login) != 0)
        {
          send(fdmax ,connect_log, strlen(connect_log), 0);
          ret_login = send_msg(listener, fdmax, i, master, read_fds, save_master, flag_login, chat);
          return (ret_login);
        }
      login_str = strcat(stock_login, " : is now connected\n");
      for (j = 0; j <= fdmax; j++)
        {
          if (j != listener)
            send(j, login_str, strlen(login_str), 0);
        }
      send(i, connect, strlen(connect), 0);
      return (ret_login);
    }
  }
  else
  {
    if ((nbytes = recv(i, buf, sizeof(buf), 0)) > 0)
    {
      buf[nbytes] = '\0'; 
      if (buf[0] == '/')
        check_cmd(buf, chat, i, master, fdmax); 
      else
      {
        for (j = 0; j <= fdmax; j++)
        {
        /* send to everyone! */
          if (FD_ISSET(j, &master))
          {
          /* except the listener and ourselves */
            if (j != listener && j != i)
            {
              recup_name = check_list_return_login(chat, i);
              if (recup_name != "unfind")
                {
                  send(j, recup_name, strlen(recup_name), 0);
                  send(j, msg_user, strlen(msg_user), 0);
                }  
              if (send(j, buf, nbytes, 0) == -1)
                perror("send() error ");
            }
          }
        }
      }  
    }
    else
    {
      /* got error or connection closed by client */
        close_client(i, master, fdmax, chat);
        if(nbytes == 0)
        /* connection closed */
          printf("socket %d hung up\n", i);
        else
          perror("recv() error ");

      /* close it... */
        close(i);

      /* remove from master set */
        FD_CLR(i, &master);
        FD_CLR(i, save_master);

        check_list_delog(&chat, i);
    }
  }
}

void get_init(char **argv, t_list chat)
{
  fd_set save_master;
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
  int test = 0;

  /* clear the master and temp sets */
  FD_ZERO(&master);
  // FD_ZERO(&read_fds);
  
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
  
  /* keep track of the biggest file descriptor */
  fdmax = listener;
  while(1)
  {
    /* add the listener to the master set */
    FD_ZERO(&save_master);
    FD_SET(listener, &master);
    
    FD_ZERO(&read_fds);
   
    read_fds = master;
    save_master = master;
    if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
      perror("Server-select() error");
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
          bzero(login, 0);
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
            flag_login = 1;
          }
        }
        else
          {
            login = send_msg(listener, fdmax, i, master, &read_fds, &save_master, flag_login, chat); 
            master = save_master;
            if (flag_login == 1)
              put_in_list_front(&chat, newfd, login);
           flag_login = 0;
          }
      //  send(i, msg_send, strlen(msg_send), 0);
      }
    }
  }
}

