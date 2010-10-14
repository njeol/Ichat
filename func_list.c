#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ichat.h"

void		show_list(t_list l)
{
  t_list	*new;
  new = l.first;
  while (new)
    {
      printf("login:%s ", new->login);
      printf("fd:%d \n", new->fd);
      new = new->next;
    }
  printf("\n");

}

void		put_in_list_front(t_list *l, int fd_number, char *login)
{
  t_list	*new;

  new = malloc(sizeof(*new));
  new->fd = fd_number;
  new->login = login;
  new->next = l->first;
  new->prev = NULL;
  if (l->first)
    l->first->prev = new;
  else
    l->last = new;
  l->first = new;
}
